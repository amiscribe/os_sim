#include <stdlib.h>
#include <string.h>
#include "stringlib.h"



#ifndef STRINGVECT_H
#define STRINGVECT_H
typedef struct strVect strVect;


struct strVect
   {
    char** vect;
    int max;
    int size;
   };



char** allocateEmpStrArr(int size)
   {
    char** newArr;
    int strCnt;

    newArr = (char**) calloc(size, sizeof(char*));
    
    if(newArr == NULL)
       {
        return 0;
       }    

    for(strCnt = 0; strCnt < size; strCnt++)
       {
        newArr[strCnt] = NULL;
       }    

    return newArr;
   }



void constructVect(strVect *self, int seed)
   {
    self->max = seed;
    self->vect = allocateEmpStrArr(seed);
    self->size = 0;
   }


//safe bounds checking insert for string vector
void insAtNdx(strVect *self, const char* string, int ndx)
   {
    int cleanNdx = self->max;

    //check bounds
    while(ndx >= self->max)
       {
        self->vect = realloc(self->vect, (self->max+5) * sizeof(char*));
        self->max = self->max + 5;

        while(cleanNdx < self->max)
           {
            self->vect[cleanNdx] = NULL;
            cleanNdx++; 
           }
       }

    strCpy(&(self->vect[ndx]), string);
   }

void copyStrVect(strVect* dest, const strVect* source)
   {
    int srcMax = source->max;
    int cpyNdx;    
    

    //safe copy all data over to new struct
    for(cpyNdx = 0; cpyNdx < srcMax; cpyNdx++)
       {
        insAtNdx(dest, source->vect[cpyNdx], cpyNdx);
       }
   }

//queue functionality

void sv_enqueue(strVect *self, const char* string)
   {
    
    //use insert at ndx and iterate size member
    insAtNdx(self, string, self->size);
    self->size++;
    
   }

int sv_dequeue(strVect *self, char** returnStr)
   {
    //variable
    int shiftNdx;

    //empty queue
    if(self->size == 0)
       {
        return 0;
       }
  
    //remove first item
    strcpy(*returnStr, self->vect[0]);
    
    //walk array forward
    for(shiftNdx = 0; shiftNdx + 1 < self->size; shiftNdx++)
       {
        strCpy(&(self->vect[shiftNdx]), self->vect[shiftNdx+1]);
       }

    self->size--;
    return 1;
   }

#endif
