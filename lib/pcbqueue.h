#include <stdlib.h>
#include "oslib.h"
#include <tgmath.h>

#ifndef PCBQUEUE_H
#define PCBQUEUE_H
typedef struct pcbQueue pcbQueue;


struct pcbQueue
   {
    PCB* vect;
    int max;
    int size;
   };

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

int pcbq_isEmpty(const pcbQueue *self)
   {
    if(self->size == 0)
       {
        return 1;
       }

    return 0;
   }

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

void swap(PCB *one, PCB *other)
   {
    PCB temp;
    
    constructPCB(&temp, NULL, 0);

    temp = pcbCopy(one);
    *one = pcbCopy(other);
    *other = pcbCopy(&temp);
   }

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
void sink(pcbQueue* self, int root, int varSize)
   {
    //variables
    int lc = getLeft(root);
    int rc = getRight(root);
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
