#ifndef MaxHeap_H
#define MaxHeap_H

#include "Defs.h"

//Pointer to a max heap struct.
typedef struct MaxHeap_m* MaxHeap;

/*Initializing a MaxHeap and returns it.
Parameters- copyFunction for the element set in the heap, freeFunction for the element, equalFunction
for 2 elements, printFunction for the element, the max capacity of the heap and the heap's name.
Returns- the new MaxHeap.*/
MaxHeap CreateHeap(copyFunction, freeFunction, equalFunction, printFunction, int, char*);

/*Frees all the heap's alloc'd memory and deletes it.
Parameters- MaxHeap to delete.
returns- a status.*/
status destroyHeap(MaxHeap);

/*Adds an element to the heap.
Parameters- MaxHeap to add to.
returns- a status.*/
status insertToHeap(MaxHeap, element);

/*Prints the heap and its elements.
Parameters- MaxHeap to print.
returns- a status.*/
status printHeap(MaxHeap);

/*Removes a the largest element from the heap (not freeing it).
Parameters- MaxHeap to remove from.
returns- the removed element.*/
element PopMaxHeap(MaxHeap);

/*Getter function for the largest element of the heap.
Parameters- MaxHeap to get from.
returns- the heap's largest element.*/
element TopMaxHeap(MaxHeap);

/*Getter function for the heap's unique name.
Parameters- MaxHeap to get from.
returns- the heap's name.*/
char* getHeapId(MaxHeap);

/*Getter function for the heap's size.
Parameters- MaxHeap.
returns- the heap's size (number of elements).*/
int getHeapCurrentSize(MaxHeap);

#endif
