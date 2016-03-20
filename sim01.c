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
#include "stringlib.h"
#include "c_ins_queue.h"
#include "fiohelpers.h"
#include "SimpleTimer.h"
#include "oslib.h"


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
/*
int loadProgData(char* filepath, insQueue* destQ)
   {
    //variables
    FILE* mdatF = NULL; //meta-data file ptr
    char* buffer;

    char* line = NULL;
    char* token = NULL;
    char* insTok = NULL;
    char** tokens; //make these string vectors
    char** lines;  //make this a string vector

    char comp;
    char descrip[STATICSIZE];
    int cycles;    

    int maxSize;
    int lineCnt = 0;
    int insCnt = 0;
    int tCnt;    

    instruction tmpIns; //temporary instruction buffer for loading ins into q    
    insQueue tmpQ;
    


    //allocate space to all strings requiring it
    alloStr(&buffer, STATICSIZE);
    
    constructQueue(&tmpQ);




    mdatF = fopen(filepath, "r");
    if(mdatF == NULL)
       {
        perror("The following error occurred");
        return ERROR;
       }


 
    //load up beginning line as garbage
    fGetToDelim(mdatF, &buffer, "\n");
    clearStr(&buffer);

    //load up all subseuent lines
    fGetToDelim(mdatF, &buffer, ".");    
    
    //priming loop to parse all instuctions 
    // into individual lines
    line = strtok(buffer, "\n");
    while(line != NULL)
       {
        strcpy(lines[lineCnt], line);
        
        line = strtok(NULL, "\n");
        lineCnt++;
       }
 

    //dellocate buffer
    free(buffer);

    //parse lines into idividual intructions
    // by tokenizing up to ";" and remove leading spaces    
    for(tCnt = 0; tCnt < lineCnt; tCnt++)
       {
        insTok = strtok(lines[tCnt],";");
        while(insTok != NULL)
           {
            if(insTok[0] == ' ')
               {
                removeLeadingSpace(&insTok);
               }

            //detects if our current token is a valid instruction
            // we want to store
            if( isalpha(insTok[0]) )
               {
                strcpy(tokens[insCnt], insTok);
                insCnt++;
               }

            insTok = strtok(NULL,";");
           }
       }

    //final pass of parsing

    // remove each instruction from its token array
    // and parse into it's component parts to load into
    // a temporary instruction

    for(tCnt = 0; tCnt < insCnt; tCnt++)
       {

        //parse out component char
        token = strtok(tokens[tCnt], "(");
        comp = token[0];
        
        //parse out descriptor
        token = strtok(NULL, ")");
        strcpy(descrip, token);

        //parse out cycles
        token = strtok(NULL, " ");
        
        cycles = atoi(token);       
        
        tmpIns = loadInstruction(comp, descrip, cycles);
        
        enqueue(&tmpQ, tmpIns); 
       }

    //assign our destination queue
    // to our temporary queue
    *destQ = tmpQ;

   }
*/
