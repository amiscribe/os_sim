#include <stdio.h>
#include "interruptlib.h"
#include "string.h"
/*
void prntArr(int arr[], int size){

   for(int i = 0; i < size; i++){
      printf("%i", arr[i]);
      printf("%c", ',');
      printf("%c", ' ');
    }
    puts("");
}

int left(int parent){
    return (2*parent) + 1;
}

int right(int parent){
    return left(parent) + 1;
}

int parent(int child){
    return (child-1)/2;
}

int swap(int arr[], int one, int other){
    int temp;

    temp = arr[one];
    arr[one] = arr[other];
    arr[other] = temp;
}

int getmaxchild(int arr[], int parent, int size){
  int lc = left(parent);
  int rc = right(parent);

  if(rc >= size || arr[lc] > arr[rc]){
      return lc;
  }

  return rc;
}

void sink(int arr[], int root, int size){
  int lc = left(root);
  int rc = right(root);
  int mc; //min child

  if(lc >= size){ 
    return;
  }

  mc = getmaxchild(arr, root, size);

  if(arr[root] >= arr[mc]){
    return;
  }

  swap(arr, root, mc);

  sink(arr, mc, size);

}

void heapify(int arr[], int size){
  int i;
  
  for(i = ((size-1)/2); i > -1; i--){
      sink(arr, i, size);
  }
}

void heapsort(int arr[], int size){
  int i;

  heapify(arr, size);  
  prntArr(arr, size);

  for(i = 0; i < size; i++){
      swap(arr, 0, (size-i)-1);
      prntArr(arr, size);
      sink(arr, 0, (size-i)-1);
  }

}
*/

int main(){

interrupt rupt;
interrupt rupt2;
ntrupt_queue q; 

construct_interrupt( &rupt );
construct_interrupt( &rupt2 );
construct_ntrupt_queue(&q, 5);



strcpy(rupt.register_one, "io");

strcpy(rupt.register_two, "from keyboard");

ntrupt_enqueue(&q, &rupt);

strcpy(rupt.register_one, "blerk");

strcpy(rupt.register_two, "bork");

ntrupt_enqueue(&q, &rupt);


ntrupt_dequeue(&q, &rupt2);
puts(rupt2.register_one);
puts(rupt2.register_two);

ntrupt_dequeue(&q, &rupt2);
puts(rupt2.register_one);
puts(rupt2.register_two);

if(setCheckBus(CHECK) == 0)
{
 puts("no interrupt");
}

setCheckBus(HIGH);

if(setCheckBus(CHECK) == 1)
{
 puts("interrupt");
 setCheckBus(LOW);
}

if(setCheckBus(CHECK) == 0)
{
 puts("no interrupt");
}

if(setCheckBus(CHECK) == 1)
{
 puts("interrupt");
 setCheckBus(LOW);
}

return 0;
}
