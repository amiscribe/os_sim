//Program Header Information /////////////////////////////
/**
  * @file pcbqueue.h
  *
  * @brief queue of pcb structs and associated functions
  *
  * @details Specifies PCB Queue data struct, constructors,
  *          accessors and an heapsort algorithim 
  * 
  * @version 1.00
  *          Masters Student (19 March, 2016) 
  *          Initial Implementation
  *
  */





#include <stdlib.h>
#include "oslib.h"


#ifndef PCBQUEUE_H
#define PCBQUEUE_H
typedef struct pcbQueue pcbQueue;


struct pcbQueue
   {
    PCB* vect;  //vector holding PCBs
    int max;    //present max of queue
    int size;   //present number of items in queue
   };

//print the queue for debug purposes
void print_pcb_queue(pcbQueue* self)
   {
    //variable
    int pndx;
    
    printf("%c", '[');
    for(pndx = 0; pndx < self->size; pndx++)
       {
        printf("%s", " Process ");
        printf("%i", (self->vect[pndx]).pid);
        printf("%c", ',' );
       }
    printf("%c", ']');

   }


//allocate an array of PCB structs
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


//construct the PCB queue
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
    self->size++;
   }

//remove from pcbQueue at ndx
PCB rmPcb(pcbQueue *self, int ndx)
  {
   PCB temp;

   temp = pcbCopy((&(self->vect)[ndx]));

   self->size--;

   return temp;

  }
//copy the full PCB queue
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

//enqueue one PCB
void pcbq_enqueue(pcbQueue *self, const PCB* src)
   {
    
    //use insert at ndx and iterate size member
    insPcbQ(self, src, self->size);
    
   }

//dequeue one PCB
// returns 0 when empty
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

//checks if PCBqueue is empty
int pcbq_isEmpty(const pcbQueue *self)
   {
    if(self->size == 0)
       {
        return 1;
       }

    return 0;
   }


//for heapsort and priority queue functionality

//tree accessors
//returns index of parent
int getParent(int child)
   {
    return (int) (child-1)/2; //returns floor 
   }

//returns index of left child
int getLeft(int parent)
   {
    return (2*parent) + 1;
   }

//returns index of right child
int getRight(int parent)
   {
    return (2*parent) + 2;
   }

//swap two PCBs 
void swap(PCB *one, PCB *other)
   {
    PCB temp;
    
    constructPCB(&temp, NULL, 0);

    temp = pcbCopy(one);
    *one = pcbCopy(other);
    *other = pcbCopy(&temp);
   }

//get the index of the PCB wit the longest runtime
int getMaxChild(const pcbQueue *self, int parent, int varSize)
   {
    //variables
    int lc = getLeft(parent);           //left child node
    int rc = getRight(parent);          //right child node
    PCB* leftChild = &(self->vect[lc]);
    PCB* rightChild = &(self->vect[rc]);

    if(rc >= varSize || leftChild->time > rightChild->time)
       {
        return lc;
       }

    return rc;
   }


//heapsort with accessors

//recursively sinks a given PCB to its appropriate place
// in the heap
void sink(pcbQueue* self, int root, int varSize)
   {
    //variables
    int lc = getLeft(root);
    int mc;                   //max value child

    PCB* parent = &(self->vect[root]);
    PCB* child;

    if(lc >= varSize)
      {
       return;
      }

    mc = getMaxChild(self, root, varSize);
    child = &(self->vect[mc]);


    if(parent->time >= child->time)
      {
       return;
      }

    swap(parent, child);
   
    sink(self, mc, varSize);    

   }

//transforms standard queue into a heap
void heapify(pcbQueue* self)
   {
    //variables
    int pNdx;
    int size = self->size;
    

    for(pNdx = ((size/2)-1); pNdx > -1; pNdx--)
       {
        sink(self, pNdx, size);
       }

   }

//gets the runtimes of each PCB and stores them
// as a member of each PCB
void getRuntimes(OS* sysNfo, pcbQueue *self)
  {
   //vairables
   int pNdx;
   PCB* readyQueue = self->vect;

   for(pNdx = 0; pNdx < self->size; pNdx++)
      {
       readyQueue[pNdx].time = sumInsTime(sysNfo, &(readyQueue[pNdx].instructions));
      }
  }

//standard heapsort used to emulate SJF and SRTF-N
// scheduling algorithims
void heapsort(OS* sysNfo, pcbQueue *self)
  {
   //variable
   const int head = 0;
   int pNdx;
   int size = self->size;
   PCB* vector = self->vect;

   //get runtimes every time function is called for SRJF
   getRuntimes(sysNfo, self);
   
   heapify(self);
   
   if(size > 1) //account for edge case of one item
     {
      for(pNdx = 0; pNdx < size; pNdx++)
         {
          swap(&(vector[head]), &(vector[((size-pNdx)-1)]));
          sink(self, head, ((size-pNdx)-1));
         }
     }
  }

#endif
