#include <stdlib.h>
#include "oslib.h"

#ifndef PCBQUEUE_H
#define PCBQUEUE_H
typedef struct pcbQueue pcbQueue;


struct pcbQueue
   {
    PCB* vect;
    int max;
    int size;
   };



PCB* allocatePcbArr(int newSize)
   {
    PCB* newArr;
    int pcbCnt;
    insQueue buffer;

    newArr = (PCB*) calloc(newSize, sizeof(PCB));
    
    if(newArr == NULL)
       {
        return NULL;
       }    

    for(pcbCnt = 0; pcbCnt < newSize; pcbCnt++)
       {
        constructQueue(&buffer);
        constructPCB(&(newArr[pcbCnt]), &buffer, -1);
       }    

    return newArr;
   }



void constructPcbQueue(pcbQueue *self, int seed)
   {
    self->max = seed;
    self->vect = allocatePcbArr(seed);
    self->size = 0;
   }


//safe bounds checking insert for string vector
void insPcbQ(pcbQueue *self, const PCB* src, int ndx)
   {
    int cleanNdx = self->max;
    insQueue temp;


    //check bounds
    while(ndx >= self->max)
       {
        self->vect = realloc(self->vect, (self->max+5) * sizeof(PCB));
        self->max = self->max + 5;

        while(cleanNdx < self->max)
           {
            constructQueue(&temp);
            constructPCB(&(self->vect[cleanNdx]), &temp, -1);
            cleanNdx++; 
           }
       }

    self->vect[ndx] = pcbCopy(src);
   }

void copyPcbQueue(pcbQueue* dest, const pcbQueue* source)
   {
    int srcMax = source->max;
    int cpyNdx;    
    

    //safe copy all data over to new struct
    for(cpyNdx = 0; cpyNdx < srcMax; cpyNdx++)
       {
        insPcbQ(dest, &(source->vect[cpyNdx]), cpyNdx);
       }
   }

//queue functionality

void pcbq_enqueue(pcbQueue *self, const PCB* src)
   {
    
    //use insert at ndx and iterate size member
    insPcbQ(self, src, self->size);
    self->size++;
    
   }

int pcbq_dequeue(pcbQueue *self, PCB* retPCB)
   {
    //variable
    int shiftNdx;

    //empty queue
    if(self->size == 0)
       {
        return 0;
       }
  
    //remove first item
    *retPCB =  pcbCopy(&(self->vect[0]));
    
    //walk array forward
    for(shiftNdx = 0; shiftNdx + 1 < self->size; shiftNdx++)
       {
        self->vect[shiftNdx] = pcbCopy(&(self->vect[shiftNdx+1]));
       }

    (self->size)--;
    return 1;
   }

//heapsort


#endif
