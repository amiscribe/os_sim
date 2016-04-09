#include <stdio.h>

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

int main(){

int i[] = {4,6,7,3,2,1,100,15,17,32,47,8,12};
int size = 13;

heapsort(i, 13);

prntArr(i, 13);


return 0;
}
