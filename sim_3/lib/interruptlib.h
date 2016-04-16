#include "stdlib.h"


typedef enum
   {
    LOW,
    HIGH,
    CHECK
   } interruptBus;

typedef struct
   { 
    char* register_one; //what threw interrupt 
    char* register_two; //why it threw interrupt
   } interrupt;

void construct_interrupt(interrupt* self)
   {
    self->register_one = malloc(20 * sizeof(char));
    self->register_one = malloc(60 * sizeof(char));
   }

int setCheckBus(interruptBus interruptStatus)
   {
    static interruptBus bus = LOW;
    
    if(interruptStatus != CHECK)
       {
        bus = interruptStatus;
       }

    return bus;
   }


