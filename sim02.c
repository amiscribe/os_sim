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
   * M.S. Student (03 Feb, 2015)
   * Initial Design and Implementation of Main Driver and relevant subroutines
   *
   * @note Requires stdio.h, string.h
   */


//Include Files///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//personal libraries
#include "util/SimpleTimer.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"


//Global Constants & Structs//////////////////////////////////////
//////////////////////////////////////////////////////////////////
const int BEGIN = 1;
const int DONE = 0;
const int SELECTING = 2;

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

float getTime(SimpleTimer *clock);



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
    configOS(&opSys, argv[1]);  

    //run operating system
    runOS(&opSys, &clock);

    return 0;
   }




//Function Defitions////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

int runOS(OS* opSys, SimpleTimer* sysTime){
    //variables
    pcbQueue readyQ;
    PCB running;
    insQueue q;
    char* elapTime;
    float totalTime = 0.0;

    //constructions
    constructQueue(&q);
    constructPcbQueue(&readyQ, 10);
    alloStr(&elapTime, 10);

    //output program begin
    stop(sysTime);
    getElapsedTime(&elapTime, sysTime);
    totalTime += atof(elapTime);
    
    tellOSStatus(opSys, &totalTime, BEGIN);

    //get the time of input operations
    start(sysTime);
    
    
    processmdf(opSys, &readyQ); 
    

    stop(sysTime);
    getElapsedTime(&elapTime, sysTime);
    totalTime += atof(elapTime);
    
    
    
    start(sysTime);
    //arrange the queue to be ordered by shortest job
    //under circumstances of Shortest Job First and Shortest Remaining...
    if(!strcmp(opSys->schedule, "SJF") || !strcmp(opSys->schedule, "SRTF-N") )
       {
        heapsort(opSys, &readyQ);
       }

    stop(sysTime);
    getElapsedTime(&elapTime, sysTime);
    totalTime += atof(elapTime);
    //output process selection

    tellOSStatus(opSys, &totalTime, SELECTING);

    start(sysTime);
    
    //run each process
    while(pcbq_dequeue(&readyQ, &running))
       {
        stop(sysTime);
        getElapsedTime(&elapTime, sysTime);
        totalTime += atof(elapTime);
        
        runPCB(opSys, &running, &totalTime);
        

        if(!pcbq_isEmpty(&readyQ))
          {
           tellOSStatus(opSys, &totalTime, SELECTING);
           start(sysTime);
           //Shortest Remaining Task First
           if(!strcmp(opSys->schedule,"SRTF-N") )
              {
               heapsort(opSys, &readyQ);
              }
           stop(sysTime);
           getElapsedTime(&elapTime, sysTime);
           totalTime += atof(elapTime);
          }

        start(sysTime);
       }

    //output sim end
    stop(sysTime);
    getElapsedTime(&elapTime, sysTime);
    totalTime += atof(elapTime);
    
    tellOSStatus(opSys, &totalTime, DONE);

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

