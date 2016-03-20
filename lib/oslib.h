
//Program Header Information /////////////////////////////
/**
  * @file pcblib.h
  *
  * @brief PCB Data Type and associated processes
  *
  * @details Specifies Process Control Block Data Struct,
  *          a State type Enum, & various functions for maniplating and handling
  *          these data types
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of Process Control Block and Associacted Data Types and Functions
  *    
  * @note Rquires "stringlib.h",  
  */


#include "./stringlib.h"
#include <stdio.h>
#include "./c_ins_queue.h"
#include "./stringvect.h"
#include <pthread.h>
#include "../util/SimpleTimer.h"

#ifndef OSLIB_H
#define OSLIB_H

const char ENDCONFIG[] = "End Simulator Configuration File"; 

/*For derefrencing of specific indicies
with OS config*/
const int MDFILENAME = 1;      
const int SCHED = 2;
const int PCYCTIME = 3;
const int MDTIME = 4;
const int HDTIME = 5;
const int PRCYCTIME = 6;
const int KBCYCTIME = 7;
const int LOGTO = 8;
const int LOGPATH = 9;

//forward declarations
typedef enum state_t state_t;
typedef enum trilog trilog;
typedef struct PCB PCB;
typedef struct OS OS;

///////////////////////////////////////////////////////////////////////////
//Data Type Defintions/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

//state enumeration
//represents current process state
enum state_t
   {
    NEW,
    READY,
    RUNNING,
    WAITING,
    TERMINATED
   };

enum trilog
   {
    MONITOR,
    FOUT,
    BOTH 
   };


//operating system
struct OS
   {
    char* metaDatFile;  /* Meta Data Filename */
    char* schedule;
    int pCycTime;       /* Processor Cycle Time (msec)*/
    int mDispTime;      /* Monitor Display Time (msec)*/
    int hdCycTime;      /* Hard Drive Cycle Time (msec)*/
    int printCycTime;   /* Printer Cycle Time (msec)*/
    int kbCycTime;      /* Keyboard Cycle Time (msec)*/
    float timeAcc;
    char* logFile;
    trilog logTo;
   };


//process control block struct
struct PCB
   {
    state_t pState;               //process state
    int pid;                      //process id number
    insQueue instructions;       //instruction queue
    float time;                   //time active
    strVect processLog;            //log of process activity
   };


//Constructors//////////////////////////////////////////
////////////////////////////////////////////////////////



int constructPCB(PCB *self, const insQueue *instr, int pid)
   {    
    self->pState = NEW;
    self->pid = pid;
    self->instructions = queueCopy(instr);
    self->time = 0.0;
    constructVect(&(self->processLog), 10);

    return 1;
   }


void constructOS(OS* self)
   {
    self->metaDatFile = NULL;  
    self->schedule = NULL;
    self->pCycTime = 0;     
    self->mDispTime = 0;    
    self->hdCycTime = 0;    
    self->printCycTime = 0;     
    self->kbCycTime = 0;
    self->timeAcc = 0.0;
    self->logFile = NULL;
    self->logTo = MONITOR; 
   }


//process to be called by pthread
// create()
void* runner(void* param)
   {
    //variables
    int *secWait;
    clock_t begin;

    //set up secWait
    secWait = (int*) param;
    *secWait *= 1000;
    
    //set up clock
    begin = clock();

    //wait for required amount of time
    while(clock()-begin < *secWait){};
    
    //exit
    pthread_exit(0);

   }



//returns int for possible error conditions
int configOS(OS* self, char* configF)
   { 
    //variables
    char* buffer = NULL;
    FILE* fin = NULL;
    int lineCtr = 0;
    strVect lineHolder;

    //constructions
    constructVect(&lineHolder, 10);
    alloStr(&buffer, 500);    

    //open file
    fin = fopen(configF,"r");

    //read in garbage first line
    fgets(buffer, 100, fin);

    //read all subsequent lines into line holder
    while(!feof(fin)){
        fgets(buffer, 500, fin);
        insAtNdx(&lineHolder, buffer, lineCtr);
        lineCtr++;
    }    

    lineCtr = 0;

    //parse each line (strtok)
    while(lineCtr < 10)
       {
        buffer = strtok(lineHolder.vect[lineCtr], ":");
 
        if(strcmp(buffer, "Log"))
           {
            lineHolder.vect[lineCtr] = strtok(NULL, "\n");
            removeLeadingChar(&(lineHolder.vect[lineCtr]));
           } 
        else
           {
            lineHolder.vect[lineCtr] = strtok(NULL, "\n");
           }

        lineCtr++;
       }


    //load into OS
    strCpy(&(self->metaDatFile), lineHolder.vect[MDFILENAME]);
    strCpy(&(self->schedule), lineHolder.vect[SCHED]);
    self->pCycTime = atoi(lineHolder.vect[PCYCTIME]);      
    self->mDispTime = atoi(lineHolder.vect[MDTIME]);    
    self->hdCycTime = atoi(lineHolder.vect[HDTIME]);
    self->printCycTime = atoi(lineHolder.vect[PRCYCTIME]); 
    self->kbCycTime = atoi(lineHolder.vect[KBCYCTIME]);  
    strCpy(&(self->logFile), lineHolder.vect[LOGPATH]);
    
    if(strCmp(lineHolder.vect[LOGTO], "Log to File"))
       {
        self->logTo = FOUT;
       }
    if(strCmp(lineHolder.vect[LOGTO], "Log to Monitor"))
       {
        self->logTo = MONITOR;
       }
    else if(strCmp(lineHolder.vect[LOGTO], "Log to Both"))
       {
        self->logTo = BOTH;
       }

   }

PCB pcbCopy(const PCB *origin)
   {
    //declare variables
    PCB newPCB;

    constructPCB(&newPCB, &(origin->instructions), origin->pid);

    
    newPCB.pState = origin->pState;
    newPCB.time = origin->time;
    copyStrVect(&(newPCB.processLog), &(origin->processLog));

    return newPCB;
   }





//PROCESS HANDLERS//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

int getWaitTime(const OS* sysNfo, const instruction *ins)
   {
    if( strCmp(ins->descriptor, "run") )
       {
        return ins->cycles * sysNfo->pCycTime; 
       }
    
    else if( strCmp(ins->descriptor, "hard drive") )
       {
        return ins->cycles * sysNfo->hdCycTime; 
       }

    else if( strCmp(ins->descriptor, "keyboard") )
       {
        return ins->cycles * sysNfo->kbCycTime; 
       }
    
    else if( strCmp(ins->descriptor, "monitor") )
       {
        return ins->cycles * sysNfo->mDispTime; 
       }

    else if( strCmp(ins->descriptor, "printer") )
       {
        return ins->cycles * sysNfo->printCycTime; 
       }

    else //start and end cases
       {
        return 0; 
       }
   }


/*
int processInstruction(const OS* sysNfo, const instruction* pIns, float *runTime)
   {
    //variables
    int waitTime = getWaitTime(sysNfo, pIns);
    pthread_t tid;
    pthread_attr_t attr;
    SimpleTimer runTimer;
    char* timeStr;
    

    //construct variables
    makeTimer(&runTimer);
    alloStr(&timeStr, 10);
    


    if(pIns->component == 'I' || pIns->component == 'O')
       {
        //setup thread with default attributes
        pthread_attr_init(&attr);
        
        //create our thread
        pthread_create(&tid, &attr, runner, &waitTime);

        start(&runTimer);
        
        //wait for thread to exit
        pthread_join(tid, NULL);

        stop(&runTimer);        

        getElapsedTime(&timeStr ,&runTimer);

        puts(timeStr);
       }
   }

*/
#endif /*pcblib.h*/
