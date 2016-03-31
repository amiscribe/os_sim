
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

const int START = 1;
const int END = 0;

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
    strVect runtimeLog;            //log of simulator activity
   };


//process control block struct
struct PCB
   {
    state_t pState;               //process state
    int pid;                      //process id number
    insQueue instructions;        //instruction queue
    int time;                     //total runtime for SJF and SRJF org
   };


//Constructors//////////////////////////////////////////
////////////////////////////////////////////////////////



int constructPCB(PCB *self, const insQueue *instr, int pid)
   {    
    //variables
    insQueue blankQ;
    
    self->pState = NEW;
    self->pid = pid;
    
    if(instr == NULL)
       {
        constructQueue(&blankQ);
        self->instructions = queueCopy(&blankQ);
       }
    else
       {
       self->instructions = queueCopy(instr);
       }
    self->time = 0;

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
    constructVect(&(self->runtimeLog), 10);
   }

//Destructors/////////////////////////////////////////
/////////////////////////////////////////////////////








//Utilities////////////////////////////////////////////
//////////////////////////////////////////////////////


//mysleep
void mysleep(int msec)
   {
    //variables
    int seconds;
    clock_t begin;

    //set up 
    seconds = msec * 1000;
    begin = clock();

    //sleep
    while(clock()-begin < seconds){};

   }



//process to be called by pthread
// create()
void* runner(void* param)
   {
    //variables
    int *msecWait;
    clock_t begin;

    //set up secWait
    msecWait = (int*) param;
   
    //call mysleep
    mysleep(*msecWait);

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

    return newPCB;
   }



/*
 * @brief get current total runtime of process
 *
 *
 *
 *
 *
 *
 *
 */
void getPresentRuntime(float *runtime)
   {
    //variables
    SimpleTimer timer;
    char* timeStr;

    alloStr(&timeStr, 10);

    start(&timer);
    stop(&timer);

    getElapsedTime(&timeStr, &timer);

    *runtime += atof(timeStr);
   }

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

int sumInsTime(const OS* sysNfo, const insQueue* estimQ)
   {
    int acc = 0;
    node* temp = estimQ->front;
    
    if(isEmpty(*estimQ))
       {
        return 0;
       }
    else
       {
        while(temp != NULL)
           {
            acc += getWaitTime(sysNfo, &(temp->ins));
            temp = temp->next;
           }
       }

    return acc;
   }


char* formatInstruction(int processId, float runTime, const instruction* insNfo, int start)
   {
    //variables
    char* formatBuff;
    char* pid;
    int progAppFlag;

    //allocate string space
    alloStr(&formatBuff, 60);
    alloStr(&pid, 5);

    //setup
    sprintf(pid,"%d",processId);
    
    //start and end of program case
    if(insNfo->component == 'A')
       {
        strcat(formatBuff, ftoa(runTime));
        strcat(formatBuff, " - OS: ");

        if(!strcmp(insNfo->descriptor, "start") && (start == START))
           {
            strcat(formatBuff, "preparing process ");
            strcat(formatBuff, pid);
           }
        
        else if(!strcmp(insNfo->descriptor, "start") && (start == END))
           {
            strcat(formatBuff, "starting process ");
            strcat(formatBuff, pid);
           }
        
        else if(!strcmp(insNfo->descriptor, "end") && (start == START))
           {
            strcat(formatBuff, "removing process ");
            strcat(formatBuff, pid);
           }
        else if(!strcmp(insNfo->descriptor, "end") && (start == END))
           {
            free(pid);
            return NULL;
           }

       } 
    else //standard case
       { 
        //do strcats
        strcat(formatBuff, ftoa(runTime));
        
        
        strcat(formatBuff, " - Process ");
        
        strcat(formatBuff, pid);
        strcat(formatBuff, ": ");
      
        if(start == START)
           {
            strcat(formatBuff, "start ");
           }
        else  
           {
            strcat(formatBuff, "end ");
           }


        if(insNfo->component == 'P')
           {
            strcat(formatBuff, "processing");
           }
        else
           {
            strcat(formatBuff, insNfo->descriptor);
           }
        strcat(formatBuff, " ");

        switch(insNfo->component)
           {
            case 'P': //processing
              strcat(formatBuff, "action");
              break;
            case 'I': //input
              strcat(formatBuff, "input");
              break;
            case 'O': //output
              strcat(formatBuff, "output");
              break;
           }
       }
    free(pid);

    return formatBuff;
   
   }

void outputHandler(OS* opSys, char* output)
   {
    //variables
     
    
    //output based on three conditions
    switch(opSys->logTo)
       {
        case MONITOR:
        //output to monitor
            puts(output);
            break;
        case FOUT:
        //insert to log
            sv_enqueue(&(opSys->runtimeLog), output);
            break;
        case BOTH:
            puts(output);
            sv_enqueue(&(opSys->runtimeLog), output);
            break;
       }
   }
//Runtime Handlers//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


/*
 * @brief Process one instruction
 *
 *
 *
 *
 *
 */

int processInstruction(const OS* sysNfo, const instruction* pIns, float *runTime)
   {
    //variables
    int waitTime = getWaitTime(sysNfo, pIns);
    pthread_t tid;
    pthread_attr_t attr;
    SimpleTimer runTimer;
    char* timeStr;
    

    //construct variables
    makeSimpleTimer(&runTimer);
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

        *runTime += atof(timeStr);

        timeStr = ftoa(*runTime);
       }
    
    else if(pIns->component == 'P')
      {
       //start timer
       start(&runTimer);
       
       //sleep
       mysleep(waitTime);

       //cleanup
       stop(&runTimer);
       getElapsedTime(&timeStr, &runTimer);

       *runTime += atof(timeStr);

       timeStr = ftoa(*runTime);
      }

    else if(pIns->component == 'A')
     {
      getPresentRuntime(runTime);  
     }   

   }

/*
 * @brief handles the instructions of one PCB
 *
 *
 *
 *
 *
 */
void runPCB(OS* opSys, PCB* loadedPCB, float *runTime)
   {
    //variables
    char* formatOut;
    instruction buffer;
    
    //construct
    constructIns(&buffer);

    //while we still have instructions to process
    while( dequeue(&(loadedPCB->instructions), &buffer)  )
       { 
        //log start instruction
        getPresentRuntime(runTime);
        formatOut = formatInstruction(loadedPCB->pid, *runTime, &buffer, START);
        outputHandler(opSys, formatOut);

        //process insctuction
        processInstruction(opSys, &buffer, runTime);
        
        //log stop instruction if not program
        // start end notification
        formatOut = formatInstruction(loadedPCB->pid, *runTime, &buffer, END);
        
        if(formatOut != NULL)
           {
            outputHandler(opSys, formatOut);
           }
       }
  }



#endif /*pcblib.h*/
