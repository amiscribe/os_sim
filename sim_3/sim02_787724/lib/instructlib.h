
//Program Header Information /////////////////////////////
/**
  * @file instructlib.h
  *
  * @brief Instruction Data Type and associated processes
  *
  * @details Specifies Instruction Struct,
  *          & various functions for maniplating and handling
  *          this data type
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of Instructon Library
  *    
  * @note Rquires "stringlib.h" 
  */

//External Libraries///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#include "./stringlib.h"
#include <string.h>
#include <stdio.h> 


//DataTypes////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#ifndef INSTRUCTLIB_H
#define INSTRUCTLIB_H

typedef struct instruction instruction;

//instruction struct
//represents a metadata instruction
struct instruction
   {
     char component;
     char* descriptor;
     int cycles;
   };




///////////////////////////////////////////////////////////////////////
//Related Subroutines//////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

//construct instruction
void constructIns(instruction* self)
   {
    self->component = '\0';
    self->descriptor = NULL;
    self->cycles = 0;
   }



//load an instruction with data
instruction loadInstruction(char comp, char* descript, int cyc)
   {
    instruction tIns;
    constructIns(&tIns);

    tIns.component = comp;
    strCpy(&(tIns.descriptor), descript);
    tIns.cycles = cyc;

    return tIns;
   };

//assign value of one instruction to another
void copyInstruction(instruction *destIns, instruction srcIns)
    {
     constructIns(destIns);

     destIns->component = srcIns.component;
     strCpy(&(destIns->descriptor), srcIns.descriptor);
     destIns->cycles = srcIns.cycles;
    };

//print instruction for debugging purposes
void printInstruction(instruction prntIns)
   {
    printf("%c", prntIns.component); 
    printf("%s", "[" ); 
    printf("%s", prntIns.descriptor); 
    printf("%s", "]" ); 
    printf("%i", prntIns.cycles); 
    printf("%c", ' '); 
   }


#endif
