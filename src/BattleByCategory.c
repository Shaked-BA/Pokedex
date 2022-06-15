#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"

typedef struct battle_s{
	LinkedList categories;
	int categoriesAmount;
	char** categoriesNames;
	int categoryMaxCapacity;
	getCategoryFunction getCategory;
	getAttackFunction getAttack;
	printFunction printFighter;
	freeFunction freeFighter;
}battle_s;

element copyCategory(element elem){
	return elem;
}

status freeCategory(element elem){
	return destroyHeap((MaxHeap)elem);
}

int comapreTwoCategories(element first, element second){
	if (first != NULL && second != NULL){
		int result = strcmp(getHeapId((MaxHeap)first), getHeapId((MaxHeap)second));
		if (result == 0)
			return 0;
	}
	return 1;
}

status printCategory(element elem){
	if (printHeap((MaxHeap)elem) == success)
		return success;
	return failure;
}

int comapreCategoryName(element cat, element name){
	if (cat != NULL || name != NULL){
		int result = strcmp(getHeapId((MaxHeap)cat), (char*)name);
		if (result == 0)
			return 0;
	}
	return 1;
}

status destroyBattleByCategory(Battle b){
	if (b == NULL || destroyList(b->categories) == failure)
		return failure;
	b->categories = NULL;
	free(b->categoriesNames);
	b->categoriesNames = NULL;
	free(b);
	b = NULL;
	return success;
}

Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement){
	if (categories != NULL || equalElement != NULL || copyElement != NULL || freeElement != NULL || getCategory != NULL || getAttack != NULL || printElement != NULL){
		Battle newBattle = (Battle)malloc(sizeof(battle_s));
		if (newBattle == NULL)
			return NULL;
		LinkedList newCatrgories = createLinkedList(copyCategory, freeCategory, comapreTwoCategories, printCategory, comapreCategoryName);
		if (newCatrgories != NULL){
			newBattle->categories = newCatrgories;

			newBattle->categoriesNames = (char**)malloc(sizeof(char*)*numberOfCategories);
			if (newBattle->categoriesNames == NULL){
				destroyBattleByCategory(newBattle);
				return NULL;
			}
			newBattle->categoriesAmount = numberOfCategories;
			newBattle->categoryMaxCapacity = capacity;
			newBattle->getAttack = getAttack;
			newBattle->getCategory = getCategory;
			newBattle->printFighter = printElement;
			newBattle->freeFighter = freeElement;
			int i = 0;
			char *cats = (char*)malloc(strlen(categories)+1);
			if (cats == NULL){
				destroyBattleByCategory(newBattle);
				return NULL;
			}
			strcpy(cats, categories);
			char *cat = strtok(cats, ",");
			while (cat != NULL){
				if (getSize(newBattle->categories) < newBattle->categoriesAmount){
					MaxHeap newCategory = CreateHeap(copyElement, freeElement, equalElement, newBattle->printFighter, capacity, cat);
					if (newCategory != NULL && appendNode(newBattle->categories, (element)newCategory) == success){
						newBattle->categoriesNames[i++] = getHeapId(newCategory);
						cat = strtok(NULL, ",");
						continue;
					}
				destroyBattleByCategory(newBattle);
				return NULL;
				}
			}
			free(cats);
			return newBattle;
		}
	}
	return NULL;
}

status insertObject(Battle b, element elem){
	if (b != NULL){
		MaxHeap category = (MaxHeap)searchByKeyInList(b->categories, (element)b->getCategory(elem));
		if (category != NULL){
			return insertToHeap(category, elem);
		}
	}
	return failure;
}

void displayObjectsByCategories(Battle b){
	if (b != NULL)
		displayList(b->categories);
}

element removeMaxByCategory(Battle b,char* category){
	if (b != NULL || category != NULL){
		MaxHeap cat = (MaxHeap)searchByKeyInList(b->categories, (element)category);
		if (cat != NULL)
			return(PopMaxHeap(cat));
	}
	return NULL;
}

int getNumberOfObjectsInCategory(Battle b,char* category){
	if (b != NULL || category != NULL){
		MaxHeap cat = (MaxHeap)searchByKeyInList(b->categories, (element)category);
		if (cat != NULL)
			return getHeapCurrentSize(cat);
	}
	return 0;
}

element chooseFighter(Battle b,element challenger, int *attackOption, int *attackChallenger){
	MaxHeap cat = NULL;
	element option = NULL;
	element chozen = NULL;
	int maxSubAttack = 0;
	int subAttack = 0;
	for (int i = 0; i < b->categoriesAmount; i++){
		cat = (MaxHeap)searchByKeyInList(b->categories, (element)b->categoriesNames[i]);
		if (cat == NULL)
			return NULL;
		if (getHeapCurrentSize(cat) == 0)
			continue;
		option = TopMaxHeap(cat);
		if (option == NULL)
			return NULL;
		subAttack = b->getAttack(option, challenger, attackOption, attackChallenger);
		if (chozen == NULL){
			maxSubAttack = subAttack;
			chozen = option;
		}
		else if (subAttack > maxSubAttack){
			maxSubAttack = subAttack;
			b->freeFighter(chozen);
			chozen = option;
		}
		else
			b->freeFighter(option);
	}
	return chozen;
}

element fight(Battle b,element elem){
	if (b != NULL || elem != NULL){
		int attackChozen;
		int attackChallenger;
		element chozen = chooseFighter(b, elem, &attackChozen, &attackChallenger);
		if (chozen != NULL){
			int score = b->getAttack(chozen, elem, &attackChozen, &attackChallenger);
			printf("The final battle between:\n");
			b->printFighter(elem);
			printf("In this battle his attack is :%d\n\n", attackChallenger);
			printf("against ");
			b->printFighter(chozen);
			printf("In this battle his attack is :%d\n\n", attackChozen);
			if (score == 0){
				printf("IT IS A DRAW.\n");
				return chozen;
			}
			printf("THE WINNER IS:\n");
			if (score > 0){
				b->printFighter(chozen);
				return chozen;
			}
			else{
				b->printFighter(elem);
				b->freeFighter(chozen);
				return elem;
			}
		}
	}
	return NULL;
}
