
//Program Header Information /////////////////////////////
/**
  * @file c_ins_queue.h
  *
  * @brief Library for dynamically allocated queue to hold process instructions
  *
  * @details Specifies dynamically allocated queue data type and various functions for
  *          operating on queue.
  * 
  * @version 1.00
  *          Masters Student (26 Febuary, 2016) 
  *          Initial develolpment of c instruction queue
  *    
  * @note Requires pcblib.h for "instruction" data type
  */

#include "./instructlib.h"
#include <stdlib.h> /*for NULL*/

//put in header guards here
#ifndef C_INS_QUEUE_H
#define C_INS_QUEUE_H

//////////////////////////////////////////
//Global Constants////////////////////////
//////////////////////////////////////////
const int TRUE = 1;
const int FALSE = 0;
const int ERROR = -1;



////////////////////////////////////////////
//Forward Declarations//////////////////////
////////////////////////////////////////////
typedef struct node node;
typedef struct insQueue insQueue;






////////////////////////////////////////////
//Struct Definitions////////////////////////
////////////////////////////////////////////

//node containing a single instruction
struct node
   {
    struct instruction ins;
    node *next;
   };

struct insQueue
   {
    node *front;
    node *rear;
   };






/////////////////////////////////////////////////
//Library Functions//////////////////////////////
/////////////////////////////////////////////////

//initialize queue
void constructQueue(insQueue *queue)
   {
    queue->front = NULL;
    queue->rear = NULL;
   }



//is full
int isFull(insQueue queue)
   {
    return FALSE;
   }



//is empty
int isEmpty(insQueue queue)
   {
    if(queue.front == NULL)
       {
        return TRUE;
       }
    return FALSE;
   }

//push instruction back to front of queue
int push(insQueue *self, instruction data)
   {
    //variables
    node *temp = NULL; 

    if( isEmpty(*self) )
       {
        //allocate data for a new front node
        temp = (node *) malloc( sizeof(node) );
        
        //error checking in case allocate failed
        if(temp == NULL)
           {
            return ERROR;
           }
        
        
        copyInstruction( &(temp->ins), data);
        temp->next = NULL;    
 
        self->front = temp;
        self->rear = temp;        
       }

    else
       {
        //allocate data for a new front node
        temp = (node *) malloc( sizeof(node) );
        
        //error checking in case allocate failed
        if(temp == NULL)
           {
            return ERROR;
           }
        
        copyInstruction( &(temp->ins), data);
        temp->next = self->front;

        self->front = temp;    
       }
    
   }

//enqueue instruction at rear of queue
// queue must be constructed
int enqueue(insQueue *self, instruction data)
   {
    //variables
    node *temp = NULL; 

    if( isEmpty(*self) )
       {
        //allocate data for a new front node
        temp = (node *) malloc( sizeof(node) );
        
        //error checking in case allocate failed
        if(temp == NULL)
           {
            return ERROR;
           }
        
        
        copyInstruction( &(temp->ins), data);
        temp->next = NULL;    
 
        self->front = temp;
        self->rear = temp;        

       }

    else
       {
        temp = (node *) malloc( sizeof(node) ); //allocate new node
        copyInstruction(&(temp->ins), data);    //copy instruction into this new node
        
        (self->rear)->next = temp;              //the node after 
        self->rear = (self->rear)->next;
        (self->rear)->next = NULL;
       }

    
    return 1;
   }

//dequeue instruction
//queue must be constructed
int dequeue(insQueue *self, instruction *returned)
   {
    //variables
    node* temp = NULL;    



    if(isEmpty(*self))
       {
        return FALSE;
       }
    
    else if(self->front == self->rear)
       {
        copyInstruction(returned, (self->front)->ins);
        free(self->front);
        self->front = self->rear = NULL;
       }    
    
    else
       {
        copyInstruction(returned, (self->front)->ins);
        temp = self->front;
        self->front = (self->front)->next;
        free(temp);
        temp = NULL;
       }
    
    return TRUE;  
   }


insQueue queueCopy(const insQueue* self)
   {
    //variables
    insQueue tQ;
    node* origin = NULL;

    //construct and allocate
    constructQueue(&tQ);
    
    //copying empty queue condition
    if(isEmpty(*self))
       {
        return tQ;
       }
    
    //copy under other conditions
    else
       {
        //I feel like I can just push instructions here
        origin = self->front;      
        

        enqueue(&tQ, origin->ins);
        origin = origin->next;        

        while(origin != NULL)
           {
            enqueue(&tQ, origin->ins);
            origin = origin->next;        
           }
       }
    
    return tQ; 
   }


int getQueueLength(const insQueue* queue)
   {
    //variables
    node* temp = queue->front;
    int counter = 0;

    if(temp == NULL)
       {

        counter = 0;
       }
    else
       {
        while(temp != NULL)
           {
            counter++;
            temp = temp->next; 
           }
       }


    return counter;
   }


void printQueue(const insQueue* queue)
   {
    //declare variables
    node* temp = queue->front;
    
    if(temp == NULL)
       {
        puts("Queue is empty");
       }
    else
       {
        while(temp != NULL){
            printInstruction(temp->ins);
            temp=temp->next;
        }
       }
    
   }

void clearQueue(insQueue *self)
   {
    instruction garbage;
    constructIns(&garbage);

    while(dequeue(self, &garbage) != FALSE){};
    
    return;
   }




#endif /*c_ins_queue.h*/

