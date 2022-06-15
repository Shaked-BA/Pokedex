#include "MaxHeap.h"

typedef struct MaxHeap_m{
	element *heapArr;
	int maxCapacity;
	int size;
	char *heapName;
	copyFunction copyElement;
	freeFunction freeElement;
	equalFunction compare;
	printFunction printElement;
} MaxHeap_m;

MaxHeap CreateHeap(copyFunction copyfunc, freeFunction freefunc, equalFunction equalfunc, printFunction printfunc, int maxcap, char* name){
	if (copyfunc == NULL || freefunc == NULL || equalfunc == NULL || printfunc == NULL || name == NULL)
		return NULL;
	MaxHeap newHeap = (MaxHeap)malloc(sizeof(MaxHeap_m));
	if (newHeap == NULL)
		return NULL;
	newHeap->heapArr = (element*)malloc(0);
	if (newHeap->heapArr == NULL){
		free(newHeap);
		newHeap = NULL;
		return NULL;
	}
	newHeap->maxCapacity = maxcap;
	newHeap->size = 0;
	char *newName = (char*)malloc(strlen(name)+1);
	if (newName == NULL){
		free(newHeap->heapArr);
		newHeap->heapArr = NULL;
		free(newHeap);
		newHeap = NULL;
		return NULL;
	}
	strcpy(newName, name);
	newHeap->heapName = newName;
	newHeap->copyElement = copyfunc;
	newHeap->freeElement = freefunc;
	newHeap->compare = equalfunc;
	newHeap->printElement = printfunc;
	return newHeap;
}

void heapify(MaxHeap heap, int i){ //left(i)=2i+1, right(i)=2i+2
	int largest;
	if (2*i+1 < heap->size && heap->compare(heap->heapArr[2*i+1], heap->heapArr[i]) == 1)
		largest = 2*i+1;
	else
		largest = i;
	if (2*i+2 < heap->size && heap->compare(heap->heapArr[2*i+2], heap->heapArr[largest]) == 1)
		largest = 2*i+2;
	if (largest != i){
		element tmp = heap->heapArr[i];
		heap->heapArr[i] = heap->heapArr[largest];
		heap->heapArr[largest] = tmp;
		heapify(heap, largest);
	}
}

status destroyHeap(MaxHeap heap){
	if (heap == NULL)
		return failure;
	for (int i = 0; i < heap->size; i++){
		if( heap->freeElement(heap->heapArr[i]) == failure)
			return failure;
	}
	free(heap->heapArr);
	heap->heapArr = NULL;
	free(heap->heapName);
	heap->heapName = NULL;
	free(heap);
	heap = NULL;
	return success;
}

status insertToHeap(MaxHeap heap, element elem){ //parent(i)=i/2-1
	if (heap == NULL || elem == NULL || heap->size == heap->maxCapacity)
		return failure;
	element newElement = heap->copyElement(elem);
	if (newElement == NULL)
		return memoryProblem;
	heap->size++;
	heap->heapArr = (element*)realloc(heap->heapArr, (heap->size)*sizeof(element));
	if (heap->heapArr == NULL)
		return memoryProblem;
	heap->heapArr[heap->size-1] = newElement;
	int i = heap->size-1;
	int p = i/2-1;
	if (p < 0)
		p = 0;
	while (i > 0 && heap->compare(heap->heapArr[p], heap->heapArr[i]) == -1){
		element tmp = heap->heapArr[i];
		heap->heapArr[i] = heap->heapArr[p];
		heap->heapArr[p] = tmp;
		i = p;
		p = i/2;
		if (p < 0)
			p = 0;
	}
	return success;
}

status printHeap(MaxHeap heap){
	if (heap == NULL)
		return failure;
	printf("%s:\n", heap->heapName);
	if (heap->size == 0){
		printf("No elements.\n\n");
		return success;
	}
	MaxHeap heapCopy = CreateHeap(heap->copyElement, heap->freeElement, heap->compare, heap->printElement, heap->maxCapacity, heap->heapName);
	heapCopy->size = heap->size;
	heapCopy->heapArr = (element*)realloc(heapCopy->heapArr, heapCopy->size*sizeof(element));
	if (heapCopy->heapArr == NULL)
		return memoryProblem;
	for (int i = 0; i < heap->size; i++)
		heapCopy->heapArr[i] =  heap->heapArr[i];
	int j = 0;
	while (heapCopy->size > 0){
		printf("%d. ", ++j);
		element top = TopMaxHeap(heapCopy);
		heap->printElement(top);
		heap->freeElement(top);
		heapCopy->heapArr[0] = heapCopy->heapArr[heapCopy->size-1];
		heapCopy->size--;
		heapify(heapCopy, 0);
	}
	destroyHeap(heapCopy);
	return success;
}

element PopMaxHeap(MaxHeap heap){
	if (heap == NULL)
		return NULL;
	if (heap->size < 1)
		return NULL;
	element max = heap->heapArr[0];
	heap->size--;
	if (heap->size == 0){
		free(heap->heapArr);
		heap->heapArr = (element*)malloc(0);
		if (heap->heapArr == NULL){
			return NULL;
		}
	}
	else{
		heap->heapArr[0] = heap->heapArr[heap->size];
		heap->heapArr = (element*)realloc(heap->heapArr, heap->size*sizeof(element));
		if (heap->heapArr == NULL){
			return NULL;
		}
		heapify(heap, 0);
	}
	return max;
}

element TopMaxHeap(MaxHeap heap){
	if (heap == NULL)
		return NULL;
	return heap->copyElement(heap->heapArr[0]);
}

char* getHeapId(MaxHeap heap){
	if (heap == NULL)
		return NULL;
	return heap->heapName;
}

int getHeapCurrentSize(MaxHeap heap){
	if (heap == NULL)
		return 0;
	return heap->size;
}
