
//Program Header Information /////////////////////////////
/**
  * @file oslib.h
  *
  * @brief PCB Data Struct, OS Data Struct and associated processes
  *
  * @details Specifies OS Data Struct, Process Control Block Data Struct,
  *          a State type Enum, & various functions for maniplating and handling
  *          these data types
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of Process Control Block and Associacted Data Types and Functions
  *    
  * @version 1.10
  *          Masters Student (15 March, 2016) 
  *          Heavy modifications and additions of utility functions realted to OS management, output and processing
  *
  */


///////////////////////////////////////////////////////////////////////////
//Include Files////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <stdio.h>

#include "./stringlib.h"
#include "./c_ins_queue.h"
#include "./stringvect.h"
#include "../util/SimpleTimer.h"

#ifndef OSLIB_H
#define OSLIB_H

//////////////////////////////////////////////////////////////////////////
//Global Constants////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


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



///////////////////////////////////////////////////////////////////////////
//Data Type Defintions/////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

//forward declarations
typedef enum state_t state_t;
typedef enum trilog trilog;
typedef struct PCB PCB;
typedef struct OS OS;

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

//enum for easy comparision of
//log options
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
    int time;                     //total runtime for SJF and SRJF scheduling
   };



/////////////////////////////////////////////////////////////////
//Constructors///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


int constructPCB(PCB *self, const insQueue *instr, int pid)
   {    
    //variables
    insQueue blankQ;
    
    //set state and pid
    self->pState = NEW;
    self->pid = pid;
    
    //null option for blank intialization
    if(instr == NULL)
       {
        constructQueue(&blankQ);
        self->instructions = queueCopy(&blankQ);
       }
    else //for initilization with instructions
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







///////////////////////////////////////////////////////
//Utilities////////////////////////////////////////////
///////////////////////////////////////////////////////


/**
 * @brief custom defined sleep function to simulate processing time
 * 
 * @param int msec time to sleep in milliseconds
 *
 */
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


/**
 * @brief process to be called by pthread create
 *
 * @param void* param generic parameter passed from pthread create
 *
 */
void* runner(void* param)
   {
    //variables
    int *msecWait;

    //set up secWait
    msecWait = (int*) param;
   
    //call mysleep
    mysleep(*msecWait);

    //exit
    pthread_exit(0);

   }


/**
 * @brief loads in configuration file and stores all information into OS struct
 *
 * @detailed takes in info one line at a time into vector and then parses and sanitizes
 *            the lines into raw data to be stored in OS struct
 *
 * @param OS* self the operating system to be configured
 *
 * @param char* configF the name of the configuration file
 *
 * @return int error or sucess file open status
 */
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

    //fopen error
    if(fin == NULL)
       {
        return 0;
       }

    //read in garbage first line
    fgets(buffer, 100, fin);

    //read all subsequent lines into line holder
    while(!feof(fin))
       {
        fgets(buffer, 500, fin);
        insAtNdx(&lineHolder, buffer, lineCtr);
        lineCtr++;
       }    
  
    fclose(fin);

    lineCtr = 0;

    //parse each line
    //getting info to right of ':'
    while(lineCtr < 10)
       {
        buffer = strtok(lineHolder.vect[lineCtr], ":");
 
        if(strcmp(buffer, "Log") || strcmp(buffer, "CPU Scheduling"))
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


    //load configuration components into OS
    strCpy(&(self->metaDatFile), lineHolder.vect[MDFILENAME]);
    strCpy(&(self->schedule), lineHolder.vect[SCHED]);
    self->pCycTime = atoi(lineHolder.vect[PCYCTIME]);      
    self->mDispTime = atoi(lineHolder.vect[MDTIME]);    
    self->hdCycTime = atoi(lineHolder.vect[HDTIME]);
    self->printCycTime = atoi(lineHolder.vect[PRCYCTIME]); 
    self->kbCycTime = atoi(lineHolder.vect[KBCYCTIME]);  
    strCpy(&(self->logFile), lineHolder.vect[LOGPATH]);
    
    //set log configuration to enum for later checking
    if(strCmp(lineHolder.vect[LOGTO], "Log to File"))
       {
        self->logTo = FOUT;
       }
    else if(strCmp(lineHolder.vect[LOGTO], "Log to Monitor"))
       {
        self->logTo = MONITOR;
       }
    else if(strCmp(lineHolder.vect[LOGTO], "Log to Both"))
       {
        self->logTo = BOTH;
       }
    
    free(buffer);
    return 1;
   }


/**
 * @brief Copy processing control block struct and return
 * 
 * @param const PCB* origin  the PCB to be copied
 *
 * @return PCB  newly constructed PCB, copy of origin PCB
 */

PCB pcbCopy(const PCB *origin)
   {
    //variables
    PCB newPCB;

    //copy instruction of origin and pid in construction
    constructPCB(&newPCB, &(origin->instructions), origin->pid);
    newPCB.pState = origin->pState;
    newPCB.time = origin->time;

    return newPCB;
   }




/*
 * @brief get current total runtime of process
 *
 * @param float* runtime  the present total runtime - modified in function
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

    free(timeStr);
   }





/**
 * @brief calculates the required sleep time for a given instruction
 *
 * @detailed multiplies cycle times supplied by OS with the number of cycles
 *           supplied by instruction
 * 
 * @param OS* sysNfo the information of the operating system for cycle times
 *
 * @param instruction* ins the instruction to be calculated 
 *
 * @return the wait time in msec
 */
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

    else //start and end instructions
       {
        return 0; 
       }
   }



/**
 * @brief returns the sum total of processing time for a single instruction
 *
 * @detailed calls getWaitTime() on every instruction in a programs instruction queue
 *           and sums all the resulting times
 *
 * @param OS* sysNfo the Operating System information used for cycle times
 *
 * @param insQueue* estimQ the instructions we are getting the runtime of
 *
 * @return int the sum time of all instructions in a given process
 *
 */
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


/**
 * @brief formats the processing of instructions for output
 * 
 * @detailed using a series of control statments generic instructions are formatted
 *           as strings for output
 *
 * @param int processId  the id of the instruction's process
 *
 * @param float runTime  the current runtime of the simulation
 *
 * @instruction* insNfo  the info of the instruction to be output
 *
 * @int start  flag indicating start or end of instruction processing
 *
 * @return char* the formatted string 
 *
 *
 */

char* formatInstruction(int processId, float runTime, const instruction* insNfo, int start)
   {
    //variables
    char* formatBuff;
    char* pid;

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
            strcat(formatBuff, "starting process ");
            strcat(formatBuff, pid);
           }
        else if(!strcmp(insNfo->descriptor, "end") && (start == START))
           {
            strcat(formatBuff, "removing process ");
            strcat(formatBuff, pid);
           }
        else
           {
            free(pid);
            return NULL;
           }

       } 

    else //standard case
       { 
        //build output string
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

/**
 * @brief handles the output as defined by the user
 *
 * @detailed sorts all output to the monitor, buffer for file I/O or both
 *
 * @param OS* opSys the operating system for output configuaraion and
 *            hold file output
 *
 * @param char* output the formatted output string to be stored or printed
 *
 */
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


////////////////////////////////////////////////////////////////////////////////////////////////
//Runtime Handlers//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


/*
 * @brief Process one instruction
 *
 * @detailed sleeps for requeisite amout of time dictated by instruction and OS
 *
 * @param OS* sysNfo the Operating System configuration information (for cycle times)
 *
 * @param instruction* pIns the instruction to be processed
 *
 * @param float* runTime the present runtime of the simulation
 *
 * @return int sucessful process
 */

int processInstruction(const OS* sysNfo, const instruction* pIns, float *runTime)
   {
    //variables
    int waitTime = getWaitTime(sysNfo, pIns);
    int pWaitTime = sysNfo->pCycTime;
    int waitCycles;
    pthread_t tid;
    pthread_attr_t attr;
    SimpleTimer runTimer;
    char* timeStr;
    

    //construct variables
    makeSimpleTimer(&runTimer);
    alloStr(&timeStr, 10);
    
    //make unique sleep thread for IO operations
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
    //processing case
    else if(pIns->component == 'P') 
      {
       //start timer
       start(&runTimer);
       
       //sleep with cycles
       for(waitCycles = 0; waitCycles < pIns->cycles; waitCycles++)
          {
           mysleep(pWaitTime);
          }

       //cleanup
       stop(&runTimer);
       getElapsedTime(&timeStr, &runTimer);
       *runTime += atof(timeStr);
       timeStr = ftoa(*runTime);
      }
    //start and end program case
    else if(pIns->component == 'A')
     {
      getPresentRuntime(runTime);  
     }   
    
    return 1;
   }



/*
 * @brief handles the instructions of one PCB
 *
 * @detailed unloads and runs all instructions contained in a given PCB
 *
 * @param OS* opSys operating system configuration information
 *
 * @param PCB* loadedPCB  the PCB loaded and run by the OS 
 *
 * @param float *runtime  the current runtime of the simulation
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
