
//Program Header Information /////////////////////////////
/**
  * @file fio_util.h
  *
  * @brief various utilities for File I/O related to the OS sim
  *
  * @details Defines various functions to handle Input and Output tasks
  *          in the operating system sim
  * 
  * @version 1.00
  *          Masters Student (14 March, 2016) 
  *          Initial Implementation
  *
  */



#include <stdio.h>
#include <ctype.h>           //isalpha()
#include "../lib/pcbqueue.h"  //pcbqueue struct and enqueue
#include "../lib/oslib.h"     //OS struct and others


const char* ENDOFSET = "A(end)0";
const char* ENDOFSETS = "S(end)0";


void getInstruction(char** insStr, FILE* fin)
   {
    //variables
    char buffer[1];

    //seed loop
    buffer[0] = fgetc(fin);

    //while input char != ; or '.' 
    while((buffer[0] != ';') && (buffer[0] != '.'))
       {
        //strcat
        appendChar(insStr, buffer[0]);        

        //get char
        buffer[0] = fgetc(fin);
       }
   }


instruction parseIns(char* strIns)
   {
    //variables
    char* tok;
    char  comp;
    char* descrip;
    int   cycles;
    instruction rins;    

    //construct
    alloStr(&tok, 12);
    alloStr(&descrip, 12);

    tok = strtok(strIns, "(");
    comp = tok[0];

    tok = strtok(NULL, ")");
    strcpy(descrip, tok);

    tok = strtok(NULL, " \n");
    cycles = atoi(tok);

    rins = loadInstruction(comp, descrip, cycles);

    return rins;
   }

//retrieve and load a whole program from the
// metadata file
int getProgram(insQueue *programQ, FILE* fin)
   {
    //variables
    char* buffer;
    instruction tins;

    //construct
    alloStr(&buffer, 20);
    constructIns(&tins);

    //read in instructions until returned value is "A(end)0"
    do
       {
        clearStr(&buffer);
        getInstruction(&buffer, fin);
        
        //remove any leading endlines or spaces from instruction       
        while(!isalpha(buffer[0]))
          {
           removeLeadingChar(&buffer);
          }

        if(buffer[0] == 'S')
           {
            return 0;
           }    
    
        tins = parseIns(buffer);

        enqueue(programQ, tins);
       } while(strcmp(tins.descriptor, "end"));

    return 1;
    
   }

//handle the input of the metadata file
int processmdf(const OS* sysNfo, pcbQueue* readyQueue)
   {
    //variables
    FILE* mdfFile;
    PCB pcbuff;    
    int pid = 1;
    char* garbage; 
    insQueue buff_q;
    char* fpath = sysNfo->metaDatFile;    


    //construction et al
    alloStr(&garbage, 50);
    constructQueue(&buff_q);
    constructPCB(&pcbuff, &buff_q, -1);

    mdfFile = fopen(fpath, "r");

    //file open error
    if(mdfFile == NULL){
       return 0;
    }
    
    //get first line as garbage
    fgets(garbage, 50, mdfFile);
    clearStr(&garbage);
 
    //get first instruction as garbage
    getInstruction(&garbage, mdfFile);


    while(getProgram(&buff_q, mdfFile))
       {
        //linking instruction queue with new pcb
        constructPCB(&pcbuff, &buff_q, pid);
        pcbuff.time = sumInsTime(sysNfo, &buff_q); 
        
        //set PCB state as ready
        pcbuff.pState = READY;

        pcbq_enqueue(readyQueue, &pcbuff);
        
        pid++;
        
        //clear queue for next iteration
        clearQueue(&buff_q);    
             
       }

    fclose(mdfFile);
    
    return 1;
   }

//outputs log held by os
int logToFile(OS* opSys)
   {
    //variables
    FILE* fout;
    char* outString;

    alloStr(&outString, 100);

    fout = fopen(opSys->logFile, "w");
    
    //file write error
    if(fout == NULL)
       {
        return 0;
       }

    //dequeue and put lines
    while( sv_dequeue(&(opSys->runtimeLog), &outString) )
       {
        fputs(outString, fout);
        fputc('\n', fout);
       }

    fclose(fout);

    free(outString);
  
    return 1;
   }

