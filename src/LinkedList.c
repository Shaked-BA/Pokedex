#include "LinkedList.h"

typedef struct Node_n{
	element value;
	struct Node_n *next;
}Node_n;

typedef struct Node_n* Node;

typedef struct LinkedList_l{
	Node head;
	Node tail;
	int size;
	copyFunction copyValue;
	freeFunction freeValue;
	equalFunction compare;
	printFunction printValue;
	equalFunction compareKey;
}LinkedList_l;

LinkedList createLinkedList(copyFunction copyfunc, freeFunction freefunc, equalFunction equalfunc, printFunction printfunc, equalFunction equalkeyfunc){
	if (copyfunc == NULL || freefunc == NULL || equalfunc == NULL || printfunc == NULL)
		return NULL;
	LinkedList newList = (LinkedList)malloc(sizeof(LinkedList_l));
	if (newList == NULL)
		return NULL;
	newList->head = NULL;
	newList->tail = NULL;
	newList->size = 0;
	newList->copyValue = copyfunc;
	newList->freeValue = freefunc;
	newList->compare = equalfunc;
	newList->printValue = printfunc;
	newList->compareKey = equalkeyfunc;
	return newList;
}

status deleteNode(LinkedList list, element elem){
	if (list == NULL || elem == NULL || list->head == NULL)
		return failure;
	Node pre = list->head;
	if (list->compare(pre->value, elem) == 0){
		list->head = pre->next;
		list->freeValue(pre->value);
		free(pre);
		pre = NULL;
		list->size--;
		return success;
	}
	Node cur = pre->next;
	while (cur != NULL){
		if (list->compare(cur->value, elem) == 0){
			pre->next = cur->next;
			list->freeValue(cur->value);
			free(cur);
			cur = NULL;
			list->size--;
			return success;
		}
		pre = cur;
		cur = cur->next;
	}
	return failure;
}

status destroyList(LinkedList list){
	if (list == NULL)
		return failure;
	Node cur = list->head;
	Node tmp = list->head;
	while (cur != NULL){
		tmp = cur->next;
		if (deleteNode(list, cur->value) == failure)
			return failure;
		cur = tmp;
	}
	free(list);
	list = NULL;
	return success;
}

element searchByKeyInList(LinkedList list, element elem){
	if (list == NULL || elem == NULL)
		return NULL;
	Node cur = list->head;
	while (cur != NULL){
		if (list->compareKey(cur->value, elem) == 0)
			return list->copyValue(cur->value);
		cur = cur->next;
	}
	return NULL;
}

status appendNode(LinkedList list, element elem){
	if (list != NULL || elem != NULL){
		Node cur = list->head;
		while (cur != NULL){
			if (list->compare(cur->value, elem) == 0){
				return failure;
			}
			cur = cur->next;
		}
		Node newNode = (Node)malloc(sizeof(Node_n));
		if (newNode == NULL){
			return memoryProblem;
		}
		element newValue = list->copyValue(elem);
		if (newValue != NULL){
			newNode->value = newValue;
			if (list->size == 0){
				list->head = newNode;
				list->tail = newNode;
			}
			else{
				list->tail->next = newNode;
				list->tail = newNode;
			}
			newNode->next = NULL;
			list->size++;
			return success;
		}
		free(newNode);
		newNode = NULL;
	}
	return failure;
}

status displayList(LinkedList list){
	if (list == NULL)
		return failure;
	Node cur = list->head;
	while (cur != NULL){
		list->printValue(cur->value);
		cur = cur->next;
	}
	return success;
}

int getSize(LinkedList list){
	if (list == NULL)
		return 0;
	return list->size;
}
