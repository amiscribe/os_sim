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
   * @note Requires stdio.h, syscall.h, sys/types.h, sys/wait.h, unistd.h & string.h
   */


//Include Files///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


//personal libraries
#include "lib/stringlib.h"
#include "lib/c_ins_queue.h"
#include "util/SimpleTimer.h"
#include "lib/oslib.h"
#include "util/fio_util.h"
#include "lib/stringvect.h"


//Global Constants & Structs//////////////////////////////////////
//////////////////////////////////////////////////////////////////
const int STATICSIZE = 500;    /*For static allocation*/
const int NUMCONFIGLINES = 12; /*For static allocation*/


const char SPACE = ' ';

const int B = 1;
const int DONE = 0;


//Function Delarations////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

    



/**
 * @brief Load Meta-Data File
 *
 * @details Downloads Meta-Data from external file, parses and loads into queue
 *          
 * @param char* filepath: path to Meta-Data file
 *          
 * @param insQueue* destQ: queue where our instructions will be loaded
 *
 * @return int notification of sucessful or failed upload
 *
 * @note insQueue* must be passed by refrence
 */
int runOS(OS* opSys, SimpleTimer* sysTime);


void outputSimStatus(OS* opSys, float *runTime, SimpleTimer* sysTime, int status);


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
//    outputSimStatus(opSys, &totalTime, sysTime, B);

    //get the time of input operations
    start(sysTime);
    
    processmdf(opSys, &readyQ); 
    
    stop(sysTime);
    
    getElapsedTime(&elapTime, sysTime);
    
    totalTime += atof(elapTime);
    
    start(sysTime);


    
    //arrange the queue to be ordered by shortest job
    //under circumstances of Shortest Job First and Shortest Remaining...
    if(opSys->schedule == "SJF" || opSys->schedule == "SRTF-N" )
      {
        heapsort(opSys, &readyQ);
       }

    //run each process
    while(pcbq_dequeue(&readyQ, &running))
       {
        runPCB(opSys, &running, &totalTime);
        
        //Shortest Remaining Task First
        //scheduling alogrithim
        if(opSys->schedule == "SRTF-N" )
           {
            heapsort(opSys, &readyQ);
           }

       }

    //output sim end
    stop(sysTime);
  //  outputSimStatus(opSys, &totalTime, sysTime, DONE);

}

void outputSimStatus(OS* opSys, float *runTime, SimpleTimer* sTime, int status)
   {
    //variables
    char* elapTime;
    char* outBuff; 

    alloStr(&outBuff, 50);

    getElapsedTime(&elapTime, sTime);
    *runTime += atof(elapTime);

    strcat(outBuff, elapTime);
    if(status == B)
       {
        strcat(outBuff, " - Simulator Program Starting");
       }
    else
       {
        strcat(outBuff, " - Simulator Program Ending");
       }

    outputHandler(opSys, outBuff);

    free(outBuff);
   }
