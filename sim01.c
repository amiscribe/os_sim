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
#include "util/fiohelpers.h"
#include "util/SimpleTimer.h"
#include "lib/oslib.h"


//Global Constants & Structs//////////////////////////////////////
//////////////////////////////////////////////////////////////////
const int STATICSIZE = 500;    /*For static allocation*/
const int NUMCONFIGLINES = 12; /*For static allocation*/

const char START[] = "Start Simulator Configuration File";
const char END[] = "End Simulator Configuration File";
const char LOG[] = "Log";
const char LOGTOBOTH[] = "Log To Both";
const char LOGTOMONITOR[] = "Log To Monitor";

const char SPACE = ' ';



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
int loadProgData(char* filepath, insQueue* destQ);



//Main Program Driver/////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
   {
    //variables
    OS opSys;
    insQueue tempIns;
    SimpleTimer sysTime;

    
    if(argc < 2){
        puts("Missing Config File.");
        return 0;
    }
    
    //take in config file
        //store necessary data in OS struct and variables
    constructOS(&opSys);
    configOS(&opSys, argv[1]);  

    
    puts(opSys.logFile);
    
  
    //load the metadata file into a queue    
//     constructQueue(&tempIns);    
//     loadProgData(opSys.metaDatFile, &tempIns);
    
    //begin os sim
   // start(&sysTime);

    //set up PCB
    

    //load PCB

    //run PCB 
        //make capable of being interrupted
        //log data in array or queue or whatever

    //output final data

    return 0;
   }




//Function Defitions////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
