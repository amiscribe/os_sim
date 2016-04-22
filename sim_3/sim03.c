// Program Information ////////////////////////////////////////////////
/**
   * @file sim01.c
   *
   * @brief Program which simulates the functions of an operating system
   *
   * @details Program uses functions fork(), wait(), and exit() in addition to various
   * outher functions to produce a family tree simulation and illusitrate the ideas
   * of linux system calls and process operations
   *
   * @version 1.00
   * M.S. Student (03 Feb, 2016)
   * Initial Design and Implementation of Main Driver and relevant subroutines
   *
   * @version 1.10
   * M.S. Student (15 Mar, 2016)
   * Extensive modification of main driver and main subroutines
   *
   * @note Requires stdio.h, string.h
   */


//Include Files///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//system libraries
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//personal libraries
#include "util/SimpleTimer.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"
#include "lib/interruptlib.h"


//Global Constants & Structs//////////////////////////////////////
//////////////////////////////////////////////////////////////////
const int BEGIN = 1;
const int DONE = 0;
const int SELECTING = 2;
const int INITIAL = 3;

//Function Delarations////////////////////////////////////////////
//////////////////////////////////////////////////////////////////



/**
 * @brief main driver function for simulation
 *
 * @details loads, organizes and runs PCBs according to configuration file
 *          
 * @param OS* opSys struct containing all info about the operating system
 * 
 * @param SimpleTimer* sysTime the timer initialized in main for logging the sim start time          
 *
 * @return int notification of sucessful or failed upload
 *
 * @note insQueue* must be passed by refrence
 */
int runOS(OS* opSys, SimpleTimer* sysTime);

/**
 * @brief Output the status of the OS
 * 
 * @details Output operationg system actions
 *
 * @param OS* opSys: struct containing all the operating system info
 *
 * @param float* runTime: contains the current runtime of the simulation
 *
 * @param int status: an int flagging the type of output 
 *
 *
 */

void tellOSStatus(OS* opSys, float *runTime, int status);

/**
 * @brief stop the timer and return runtime in float form
 *
 * @param SimpleTimer* clock: the current runtime clock of the simulations
 *
 * @return the runtime from the most recent timer run
 *
 */

void getTime(SimpleTimer *clock, float *sysTime);

/**
 * @brief reorganize the ready queue for scheduling purposes
 *
 * @deatils Run a heapsort and time the sort for runtime info
 *
 * @param OS* opSys carries the information about the operating system
 *
 * @param pcbQueue* readyQ the queue of processes to be scheduled 
 *
 * @param float* totalTime the running simulation runtime
 *
 * @return the runtime from the most recent timer run
 *
 */
void schedule(OS* opSys, pcbQueue* readyQ, float* totalTime);


//Main Program Driver/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
   {
    //variables
    OS opSys;
    SimpleTimer clock;

    //construct timer
    makeSimpleTimer(&clock);

    if(argc < 2){
        puts("Missing Config File.");
        return 0;
    }
    
    //start timer
    start(&clock);


    //take in config file
        //store necessary data in OS struct and variables
    constructOS(&opSys);
   
    if(!configOS(&opSys, argv[1]))
       {
        puts("Error: Config File Not Found");
        return 0;
       }

    //run operating system
    runOS(&opSys, &clock);

    if(opSys.logTo == FOUT || opSys.logTo == BOTH)
      {
       if(!logToFile(&opSys))
          {
           puts("Error: Write File Error");
           return 0;
          }
      }
    return 0;
   }




//Function Defitions////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

int runOS(OS* opSys, SimpleTimer* sysTime)
   {
    //variables
    pcbQueue readyQ;
    pcbQueue blockVect;
    PCB running;
    PCB transfer;
    interrupt temp;
    insQueue q;
    char* elapTime;
    char* outBuff;
    float totalTime = 0.0;
    state_t processState = NEW;
    ioArgs* argBundle; 

    int argNdx;

    //constructions
    constructQueue(&q);
    constructPcbQueue(&readyQ, 10);
    constructPcbQueue(&blockVect, 10);
    alloStr(&elapTime, 10);
    construct_interrupt(&temp);
    argBundle = (ioArgs*) malloc(10*sizeof(ioArgs));

    //bind global interrupt queue to os int queue
    for(argNdx = 0; argNdx < 10; argNdx++)
       {
        argBundle[argNdx].interruptQ = &(opSys->interrupts);
       }

    //output program begin
    getTime(sysTime, &totalTime);
    tellOSStatus(opSys, &totalTime, BEGIN);

    //get and log the time of input operationsgit push to a remote branch
    start(sysTime);
     
    if(!processmdf(opSys, &readyQ))
       {
        puts("Error: Meta Data File Not Found");
        return 0;
       } 

    getTime(sysTime, &totalTime);
    
    
    
    tellOSStatus(opSys, &totalTime, INITIAL);
    
    //arrange the queue to be ordered by shortest job
    //under circumstances of Shortest Job First and Shortest Remaining...
    if(!strcmp(opSys->schedule, "SJF") || !strcmp(opSys->schedule, "SRTF-N") )
       {
        schedule(opSys, &readyQ, &totalTime);
       }
    
    
    //output process selection
    tellOSStatus(opSys, &totalTime, SELECTING);

    start(sysTime);
    
    //run each process
    //while the pcbqueue isn't empty or the blocked queue is not empty
    while(pcbq_dequeue(&readyQ, &running) || !pcbq_isEmpty(&blockVect))
       {
        getTime(sysTime, &totalTime);
        
        processState = runPCB(opSys, &running, &totalTime, argBundle);
        

        //if runPCB returns blocked status
        if(processState == BLOCKED)
           {
            //declare blocked for IO
            puts("Blocked for IO");                                                  //temporary

            //update pcb state to blocked
            running.pState = BLOCKED;

            //push blocked PCB onto blocked vector
            //organize by pid
            insPcbQ(&blockVect, &running, running.pid);
           }
        
        else if(processState == READY)
           {
            pcbq_enqueue(&readyQ, &running);
            
            while(ntrupt_dequeue(&(opSys->interrupts), &temp) != 0)
               {
                outBuff = format_ntrupt_output(temp, totalTime);
                outputHandler(opSys, outBuff);
                
            //    transfer = rmPcb(&blockVect, temp.register_one);
             //   pcbq_enqueue(&readyQ, &transfer);
               }
            
            setCheckBus(LOW);  
           }
        
        //case where all processes are blocked
        if(pcbq_isEmpty(&readyQ) && !pcbq_isEmpty(&blockVect))
           {
            puts("All processes blocked");                                                                //temporary
           
            start(sysTime);

            //sleep until interrupt comes
            //reset interrupt bus
            while(setCheckBus(CHECK) != HIGH){};
            

            getTime(sysTime, &totalTime);

            //read interrupt from OS and load pcb back into ready queue
            while(ntrupt_dequeue(&(opSys->interrupts), &temp) != 0)
               {
                outBuff = format_ntrupt_output(temp, totalTime);
                outputHandler(opSys, outBuff);
                
                transfer = rmPcb(&blockVect, temp.register_one);
                pcbq_enqueue(&readyQ, &transfer);
               }
            
            setCheckBus(LOW);         
           }


        //reschedule
        if(!pcbq_isEmpty(&readyQ))
          {
           tellOSStatus(opSys, &totalTime, SELECTING);
           
           //Shortest Remaining Task First
           if(!strcmp(opSys->schedule,"SRTF-N") )
              {
               schedule(opSys, &readyQ, &totalTime);
              }
          }

        start(sysTime);
       }

    //output sim end
    getTime(sysTime, &totalTime);
    
    tellOSStatus(opSys, &totalTime, DONE);

    return 1;
   }


void tellOSStatus(OS* opSys, float *runTime, int status)
   {
    //variables
    char* elapTime;
    char* outBuff; 

    //constructions
    alloStr(&outBuff, 50);
    alloStr(&elapTime, 10);


    elapTime = ftoa(*runTime);
    strcat(outBuff, elapTime);
    
    if(status == BEGIN)
       {
        strcat(outBuff, " - Simulator Program Starting");
       }
    else if(status == INITIAL)
       {
        strcat(outBuff, " - OS: Preparing All Processes");
       }
    else if(status == SELECTING)
       {
        strcat(outBuff, " - OS: Scheduling Next Process");
       }
    else if(status == DONE)
       {
        strcat(outBuff, " - Simulator Program Ending");
       }

    outputHandler(opSys, outBuff);

   }

void schedule(OS* opSys, pcbQueue* readyQ, float* totalTime )
   {
    //variables
    SimpleTimer sortTime;
    char* elapTime; 

    //setup
    alloStr(&elapTime, 10);
    makeSimpleTimer(&sortTime);
    start(&sortTime);

    //Shortest Remaining Task First
    heapsort(opSys, readyQ);
    
    stop(&sortTime);
    getElapsedTime(&elapTime, &sortTime);
    *totalTime += atof(elapTime);

    free(elapTime);
   }


void getTime(SimpleTimer *clock, float* totalTime)
    {
     //variables
     char* elapTime;

     alloStr(&elapTime, 10);

     stop(clock);
     getElapsedTime(&elapTime, clock);
     *totalTime += atof(elapTime);

    }
