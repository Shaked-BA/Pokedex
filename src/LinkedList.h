#ifndef LinkedList_H
#define LinkedList_H

#include "Defs.h"

//Pointer to a linked list struct.
typedef struct LinkedList_l* LinkedList;

/*Initializing a new linked list.
Parameters- copyFunction for the element set in the list, freeFunction for the element, equalFunction to compare
2 elements' keys, printFunction for the element and an equalFunction to compare a key to an element's key.
returns- the new LinkedList.*/
LinkedList createLinkedList(copyFunction, freeFunction, equalFunction, printFunction, equalFunction);

/*Frees all the list's alloc'd memory and deletes it.
Parameters- LinkedList to delete.
returns- a status.*/
status destroyList(LinkedList);

/*Adds a node to the list.
Parameters- LinkedList and a element to insert the new node in the list.
returns- a status.*/
status appendNode(LinkedList, element);

/*Removes a node from the list.
Parameters- LinkedList and an element of the node to remove from it.
returns- a status.*/
status deleteNode(LinkedList, element);

/*Prints the list and the elements in it.
Parameters- LinkedList to print.
returns- a status.*/
status displayList(LinkedList);

/*Searches for a node with a specific key in the list.
Parameters- LinkedList and an element (key) to search in it.
returns- the element with the key if found or NULL if not.*/
element searchByKeyInList(LinkedList, element);

/*Getter function for the list's size.
Parameters- LinkedList.
returns- the list's size (number of nodes).*/
int getSize(LinkedList);

#endif
