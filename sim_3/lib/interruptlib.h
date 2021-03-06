#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stringlib.h"

#ifndef INTERRUPTLIB_H 
#define INTERRUPTLIB_H

const int DEF = -1;

typedef enum
   {
    LOW,
    HIGH,
    CHECK
   } interruptBus;

typedef struct
   { 
    int register_one; //what process threw int (-1 for os)
    char* register_two; //why it threw interrupt
   } interrupt;

typedef struct
   {
    interrupt* vect;
    int max;
    int size;
   } ntrupt_queue;



int setCheckBus(interruptBus interruptStatus)
   {
    static interruptBus bus = LOW;
    
    if(interruptStatus != CHECK)
       {
        bus = interruptStatus;        
       }

    return bus;
   }


void construct_interrupt(interrupt* self)
   {
    self->register_one = DEF;
    self->register_two = malloc(60 * sizeof(char));
   }

void load_interrupt(interrupt* self, int where, char* what)
   {
    self->register_one = where;
    strcpy(self->register_two, what);
   }


void ntrupt_copy(interrupt* dest, const interrupt* src)
   {
    if(src == NULL)
       {
        dest == NULL;
        return;
       }
    else if(dest == NULL)
       {
        construct_interrupt(dest);
       }

    dest->register_one = src->register_one;
    strcpy(dest->register_two, src->register_two);

   }


//allocate an array of interrupts
interrupt* construct_ntrupt_arr(int size)
   {
    interrupt* newArr;
    int strCnt;

    newArr = (interrupt*) calloc(size, sizeof(interrupt));
    
    if(newArr == NULL)
       {
        return 0;
       }    

    for(strCnt = 0; strCnt < size; strCnt++)
       {
        construct_interrupt(&newArr[strCnt]);
       }    

    return newArr;
   }


//construct the queue
void construct_ntrupt_queue(ntrupt_queue *self, int seed)
   {
    self->max = seed;
    self->vect = construct_ntrupt_arr(seed);
    self->size = 0;
   }


//safe bounds checking insert for string vector
void ntrupt_ins_at_ndx(ntrupt_queue *self, const interrupt* src, int ndx)
   {
    int cleanNdx = self->max;

    //check bounds
    while(ndx >= self->max)
       {
        self->vect = realloc(self->vect, (self->max+5) * sizeof(interrupt));
        self->max = self->max + 5;

        while(cleanNdx < self->max)
           {
            construct_interrupt( &(self->vect[cleanNdx]) );
            cleanNdx++; 
           }
       }

    ntrupt_copy(&(self->vect[ndx]), src);
   }


void copy_ntrupt_vect(ntrupt_queue* dest, const ntrupt_queue* source)
   {
    int srcMax = source->max;
    int cpyNdx;    
    

    //safe copy all data over to new struct
    for(cpyNdx = 0; cpyNdx < srcMax; cpyNdx++)
       {
        ntrupt_ins_at_ndx(dest, &(source->vect[cpyNdx]), cpyNdx);
       }
   }


//queue functionality
void ntrupt_enqueue(ntrupt_queue *self, const interrupt* src)
   {
    
    //use insert at ndx and iterate size member
    ntrupt_ins_at_ndx(self, src, self->size);
    self->size++;
    
   }

int ntrupt_dequeue(ntrupt_queue *self, interrupt* dest)
   {
    //variable
    int shiftNdx;

    //empty queue
    if(self->size == 0)
       {
        return 0;
       }
  
    //remove first item
    ntrupt_copy(dest, &(self->vect[0]));
    
    //walk array forward
    for(shiftNdx = 0; shiftNdx + 1 < self->size; shiftNdx++)
       {
        ntrupt_copy(&(self->vect[shiftNdx]), &(self->vect[shiftNdx+1]));
       }

    self->size--;
    return 1;
  
   }

char* format_ntrupt_output(interrupt ntrupt, float time)
      {
       //variables
       char* formatBuff;
       char* pid;
       char* timeStamp;
       
       //construct
       alloStr(&formatBuff, 100);
       alloStr(&pid, 5);
       alloStr(&timeStamp, 10);

       sprintf(pid, "%d", ntrupt.register_one);
       sprintf(timeStamp, "%f", time);
       
       strcat(formatBuff, timeStamp);
       strcat(formatBuff, " - Interrupt: ");

       strcat(formatBuff, "Process "); 
       strcat(formatBuff, pid);

       strcat(formatBuff, " - ");

       strcat(formatBuff, ntrupt.register_two);

       return formatBuff;
      }

#endif
