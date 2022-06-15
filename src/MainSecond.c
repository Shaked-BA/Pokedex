#include "Pokemon.h"
#include "BattleByCategory.h"

Type **typesDiscovered = NULL; //An array of pointers to the types stored from the file received.
Battle battle = NULL; //BattleByCategory object that'll keep the collected Pokemon's data.
int numberOfTypes = 0; //Number of categories.
int capacityInType = 0; //Max amount of pokemons.

/*Frees every alloc'd memory created through the program.
Using freeType from Pokemon destroyBattleByCategory from BattleByCategory.
Parameters - a status indicating whether the exit is because of a memory problem.
returns- a status.*/
status exitPokedex(status status){
	if (status == memoryProblem)
		printf("No memory available.\n");
	if (destroyBattleByCategory(battle) == success){
		battle = NULL;
		for (int t = 0; t < numberOfTypes; t++)
			freeType(typesDiscovered[t]);
		free(typesDiscovered);
		typesDiscovered = NULL;
		return success;
	}
	return failure;
}

/*equalFunction that's sent to the BattleByCategory ADT.
Parameters - two elements to compare.
returns- 1 if first is stronger, -1 if the second, 0 if equal or if the input is not valid.*/
int isEqualPokemon(element first, element second){
	if (first != NULL && second != NULL){
		Pokemon *firstPokemon = (Pokemon*)first;
		Pokemon *secondPokemon = (Pokemon*)second;
		if (firstPokemon->bioInfo->attack > secondPokemon->bioInfo->attack)
			return 1;
		else if (firstPokemon->bioInfo->attack < secondPokemon->bioInfo->attack)
			return -1;
	}
	return 0;
}

/*copyFunction that's sent to the BattleByCategory ADT.
Parameters - an elements to copy.
returns- the element's copy.*/
element copyPokemon(element toCopy){
	if (toCopy == NULL)
		return NULL;
	Pokemon *source = (Pokemon*)toCopy;
	Pokemon *copied = createPokemon(source->pokemon_name, source->species, source->bioInfo->height, source->bioInfo->weight, source->bioInfo->attack, source->type);
	return (element)copied;
}

/*getCategory function that's sent to the BattleByCategory ADT.
Parameters - an element with category.
returns- the category (type)'s name.*/
char* getType(element pokemon){
	if (pokemon == NULL)
		return NULL;
	return ((Pokemon*)pokemon)->type->type_name;
}

/*freeFunction that's sent to the BattleByCategory ADT.
Using freePokemon from Pokemon.
Parameters - an element to free.*/
status deletePokemon(element toFree){
	if (toFree == NULL)
		return failure;
	return freePokemon((Pokemon*)toFree);
}

/*Checks if a type is is an affectiveAgainstMe list of another type.
Parameters - 2 pointers to pokemons' types. the second searched in the first's list.
returns- 1 if found. 0 if not.*/
int isAffetiveAgainstMe(Type *me, Type *other){
	for (int t = 0; t < me->eamSize; t++){
		if (strcmp(me->effective_against_me[t]->type_name, other->type_name) == 0)
			return 1;
	}
	return 0;
}

/*Checks if a type is is an affectiveAgainstOthers list of another type.
Parameters - 2 pointers to pokemons' types. the second searched in the first's list.
returns- 1 if found. 0 if not.*/
int isAffetiveAgaintOthers(Type *me, Type *other){
	for (int t = 0; t < me->eaoSize; t++){
		if (strcmp(me->effective_against_others[t]->type_name, other->type_name) == 0)
			return 1;
	}
	return 0;
}

/*getAttackFunction sent to the BattleByCategory ADT.
Using the isAffetiveAgaintMe and isAffetiveAgaintOthers functions.
Parameters - 2 elements with attack and to pointers to numbers.
returns- the function calculates the elements' attacks. store them in the received pointers and returns the subtraction of them.*/
int getAttack(element first, element second, int* attackFirst, int* attackSecond){
	if (first == NULL || second == NULL)
		return 0;
	Pokemon *firstPokemon = (Pokemon*)first;
	Pokemon *secondPokemon = (Pokemon*)second;
	*attackFirst = firstPokemon->bioInfo->attack;
	*attackSecond = secondPokemon->bioInfo->attack;
	if (isAffetiveAgainstMe(firstPokemon->type, secondPokemon->type) == 1 || isAffetiveAgaintOthers(secondPokemon->type, firstPokemon->type) == 1)
		*attackFirst -= 10;
	if (isAffetiveAgainstMe(secondPokemon->type, firstPokemon->type) == 1 || isAffetiveAgaintOthers(firstPokemon->type, secondPokemon->type) == 1)
		*attackSecond -= 10;
	return *attackFirst-*attackSecond;
}

/*printFunction sent to the BattleByCategory ADT.
Parameters - an element to print.
returns- a status.*/

status displayPokemon(element pokemon){
	return printPokemon((Pokemon*)pokemon);
}
/*Adds the types received from the file to typesDiscovered using createType function from the Pokemon module.
Parameters - the line of the file containing the type's names.
returns- a status.*/
status addTypes(char *line){
	int i = 0;
	for (char *type = strtok(line, ","); type != NULL; type = strtok(NULL, ","), i++){
		typesDiscovered[i] = createType(type);
		if (typesDiscovered[i] == NULL)
			return memoryProblem;
	}
	return success;
}

/*Finding a type stored in typesDiscovered.
Parameters - the type's name.
Returns- a pointer to the type' if exists.*/
Type* findType(char *typeName){
	for (int i = 0; i < numberOfTypes; i++){
		if (strcmp(typesDiscovered[i]->type_name, typeName) == 0)
			return typesDiscovered[i];
	}
	return NULL;
}

/*Adds types to a type's effective against me list according to the file received.
Using addEAme from Pokemon and findType and exitPokedex functions.
Parameters - a type's name to add to and a type's name to add.*/
void addVSme(char *myName, char *otherName){
	if (myName[0] == '\t'){
		for (int c = 0; c < strlen(myName) - 1; myName[c] = myName[c + 1], c++);
		myName[strlen(myName) - 1] = '\0';
	}
	Type *me = findType(myName);
	if (me){
		Type *other = NULL;
		for (otherName = strtok(otherName, ","); otherName != NULL; otherName = strtok(NULL, ",")){
			other = findType(otherName);
			if (addEAme(me, other) == memoryProblem)
				exitPokedex(memoryProblem);
		}
	}
}

/*Adds types to a type's effective against others list according to the file received
Using addEAme from Pokemon and findType function and exitPokedex functions.
Parameters - a type's name to add to and a type's name to add.*/
void addVSothers(char *myName, char *otherName){
	if (myName[0] == '\t'){
		for (int c = 0; c < strlen(myName) - 1; myName[c] = myName[c + 1], c++);
		myName[strlen(myName) - 1] = '\0';
	}
	Type *me = findType(myName);
	if (me){
		Type *other = NULL;
		for (otherName = strtok(otherName, ","); otherName != NULL; otherName = strtok(NULL, ",")){
			other = findType(otherName);
			if (addEAothers(me, other) == memoryProblem)
				exitPokedex(memoryProblem);
		}
	}
}

/*Adds a pokemon received from the file to pokemonsDiscovered.
Using createPokemon function from the Pokemon and findType and exitPokedex functions.
Parameters - the line of the file containing the pokemon's details.
returns- a status.*/
status addPokemon(char *line, int pokemonIndex){
	char *name = strtok(line, ",");
	char *species = strtok(NULL, ",");
	float height = atof(strtok(NULL, ","));
	float weight = atof(strtok(NULL, ","));
	int attack = atoi(strtok(NULL, ","));
	char *typeName = strtok(NULL, "\r\n");
	Type *type = findType(typeName);
	Pokemon *newPokemon = createPokemon(name, species, height, weight, attack, type);
	if (newPokemon == NULL)
		exitPokedex(memoryProblem);
	if (insertObject(battle, (element)newPokemon) == success){
		freePokemon(newPokemon);
		return success;
	}
	freePokemon(newPokemon);
	return failure;
}

/*Reading and storing the data from the file.
Using battle, typesDiscovered and helping functions of this program.
Parameters- a file.*/
void updatePokedex(FILE *pokeData){
	if (pokeData == NULL)
		return;
	char data[300];
	fgets(data, 300, pokeData);

	if (strstr(data, "Types")){
		fgets(data, 300, pokeData);
		if (strstr(data, "Pokemons")){
			battle = createBattleByCategory(capacityInType, numberOfTypes, "", isEqualPokemon, copyPokemon, deletePokemon, getType, getAttack, displayPokemon);
			if (battle == NULL)
				exitPokedex(memoryProblem);
			return;
		}
		battle = createBattleByCategory(capacityInType, numberOfTypes, strtok(data, "\r\n"), isEqualPokemon, copyPokemon, deletePokemon, getType, getAttack, displayPokemon);
		if (battle == NULL || addTypes(data) == memoryProblem)
			exitPokedex(memoryProblem);
		while (fgets(data, 300, pokeData)){
			if (strstr(data, "Pokemons"))
				break;
			else {
				char *myName = strtok(data, " ");
				char *meOrOthers = strtok(NULL, ":");
				char *otherNames = strtok(NULL, "\r\n");
				if (strstr(meOrOthers, "me"))
					addVSme(myName, otherNames);
				else
					addVSothers(myName, otherNames);
			}
		}
	}
	for (int p = 0; fgets(data, 300, pokeData) != NULL; p++){
		if (addPokemon(data, p) == failure)
			exitPokedex(memoryProblem);
	}
}

void printMenu() {
	printf("Please choose one of the following numbers:\n"
			"1 : Print all Pokemons by types\n"
			"2 : Print all Pokemons types\n"
			"3 : Insert Pokemon to battles training camp\n"
			"4 : Remove strongest Pokemon by type\n"
			"5 : Fight\n"
			"6 : Exit\n");
}

/*Storing the data from the file and enable the user to manipulate it.
Using updatePokedex, and printMenu functions.
Parameters- number of types, max number of pokemons of each type and a configuration file.*/
int main(int argc, char* argv[]){
	numberOfTypes = atoi(argv[1]);
	capacityInType = atoi(argv[2]);
	typesDiscovered = (Type**)malloc(numberOfTypes*sizeof(Type*));
	if (typesDiscovered == NULL)
		exitPokedex(memoryProblem);
	FILE *pokeData=fopen(argv[3], "r"); //The file received
	updatePokedex(pokeData);
	fclose(pokeData);

	char input = '0';
    while(input != '6'){
    	printMenu();
    	scanf(" %c", &input);
	    switch(input){
			case '1': //Print all Pokemons by types.
				displayObjectsByCategories(battle);
				break;

			case '2': //Print all Pokemons types.
				for (int t = 0; t < numberOfTypes; t++){
					printType(typesDiscovered[t]);
				}
				break;

			case '3': //Insert Pokemon to battles training camp.
				printf("Please enter Pokemon type name:\n");
				char typeName3[300];
				scanf("%s", typeName3);
				Type *type3 = findType(typeName3);
				if (type3 != NULL){
					if (type3->type_amount == capacityInType){
						printf("Type at full capacity.\n");
						break;
					}
					printf("Please enter Pokemon name:\n");
					char name[300];
					scanf("%s", name);
					printf("Please enter Pokemon species:\n");
					char species[300];
					scanf("%s", species);
					printf("Please enter Pokemon height:\n");
					float height;
					scanf("%f.2", &height);
					printf("Please enter Pokemon weight:\n");
					float weight;
					scanf("%f.2", &weight);
					printf("Please enter Pokemon attack:\n");
					int attack;
					scanf("%d", &attack);
					Pokemon *newPokemon = createPokemon(name, species, height, weight, attack, type3);
					if (newPokemon == NULL || insertObject(battle, (element)newPokemon) == memoryProblem){
						freePokemon(newPokemon);
						exitPokedex(memoryProblem);
					}
					printf("The Pokemon was successfully added.\n");
					printPokemon(newPokemon);
					freePokemon(newPokemon);
					break;
				}
				printf("Type name doesn't exist.\n");
				break;

			case '4': //Remove strongest Pokemon by type.
				printf("Please enter type name:\n");
				char typeName4[300];
				scanf("%s", typeName4);
				Type *type4 = findType(typeName4);
				if (type4 != NULL){
					Pokemon *removed = removeMaxByCategory(battle, type4->type_name);
					if (removed == NULL){
						printf("There is no Pokemon to remove.\n");
						break;
					}
					printf("The strongest Pokemon was removed:\n");
					printPokemon(removed);
					freePokemon(removed);
					break;
				}
				printf("Type name doesn't exist.\n");
				break;

			case '5': //Fight.
				printf("Please enter Pokemon type name:\n");
				char challengerTypeName[300];
				scanf("%s", challengerTypeName);
				Type* challengerType = findType(challengerTypeName);
				if (challengerType != NULL){
					printf("Please enter Pokemon name:\n");
					char name[300];
					scanf("%s", name);
					printf("Please enter Pokemon species:\n");
					char species[300];
					scanf("%s", species);
					printf("Please enter Pokemon height:\n");
					float height;
					scanf("%f.2", &height);
					printf("Please enter Pokemon weight:\n");
					float weight;
					scanf("%f.2", &weight);
					printf("Please enter Pokemon attack:\n");
					int attack;
					scanf("%d", &attack);
					Pokemon *challenger = createPokemon(name, species, height, weight, attack, challengerType);
					printf("You choose to fight with:\n");
					printPokemon(challenger);
					Pokemon *winner = (Pokemon*)fight(battle, challenger);
					if (winner == NULL){
						printf("There are no Pokemons to fight against you.\n");
						freePokemon(challenger);
						break;
					}
					if (strcmp(winner->type->type_name, challenger->type->type_name) == 0 && winner->bioInfo->attack == challenger->bioInfo->attack)
						freePokemon(winner);
					else{
						freePokemon(winner);
						freePokemon(challenger);
					}
					break;
				}
				printf("Type name doesn't exist.\n");
				break;

			case '6': //Exit.
				exitPokedex(success);
				printf("All the memory cleaned and the program is safely closed.\n");
				break;

            default:
                printf("Please choose a valid number.\n");
                break;
	    }
	}
    exit(0);
}
