#ifndef Defs_H
#define Defs_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum e_bool {false, true} bool;

typedef enum e_status {success, failure, memoryProblem} status;

typedef void* element;

typedef element(*copyFunction)(element);

typedef status(*freeFunction)(element);

typedef status(*printFunction)(element);

typedef int(*equalFunction)(element, element);
//1 first bigger, 0 - same, -1 second bigger

typedef char*(*getCategoryFunction)(element);

typedef int(*getAttackFunction)(element firstElem, element secondElem, int* attackFirst, int* attackSecond);
//Return the first attack - second attack.
//Also produce the attack of each element, the function transmits the information through the pointers received in the function.

#endif
