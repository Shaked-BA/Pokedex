#include "Pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Pokemon* createPokemon(char *pokemon_name, char *species, float height, float weight, int attack, Type* type){
	if (pokemon_name == NULL || species == NULL || type == NULL)
		return NULL;
	Pokemon *newPokemon = (Pokemon*)malloc(sizeof(Pokemon));
	if (newPokemon == NULL)
		return NULL;
	char *pokemonName = (char*)malloc(strlen(pokemon_name) + 1);
	if (pokemonName == NULL)
		return NULL;
	strcpy(pokemonName, pokemon_name);
	newPokemon->pokemon_name = pokemonName;
	char *pokemonSpecies = (char*)malloc(strlen(species) + 1);
	if (pokemonSpecies == NULL)
		return NULL;
	strcpy(pokemonSpecies, species);
	newPokemon->species = pokemonSpecies;
	newPokemon->type = type;
	type->type_amount++;
	newPokemon->bioInfo = createBio(height, weight, attack);
	if (newPokemon->bioInfo == NULL)
		return NULL;
	return newPokemon;
}

Type* createType(char *type_name){
	if (type_name == NULL)
		return NULL;
	Type* newType = (Type*)malloc(sizeof(Type));
	if (newType == NULL)
		return NULL;
	char *typeName = (char*)malloc(strlen(type_name) + 1);
	if (typeName == NULL)
		return NULL;
	strcpy(typeName, type_name);
	newType->type_name = typeName;
	newType->type_amount = 0;
	newType->effective_against_me = (Type**)malloc(0);
	if (newType->effective_against_me == NULL)
		return NULL;
	newType->effective_against_others = (Type**)malloc(0);
	if (newType->effective_against_others == NULL)
		return NULL;
	newType->eamSize = 0;
	newType->eaoSize = 0;
	return newType;
}

BioInfo* createBio(float height, float weight, int attack){
	BioInfo *newBio = (BioInfo*)malloc(sizeof(BioInfo));
	if (newBio == NULL)
		return NULL;
	newBio->height = height;
	newBio->weight = weight;
	newBio->attack = attack;
	return newBio;
}

status addEAme(Type* me, Type* other){
	if (me == NULL || other == NULL)
		return failure;
	for (int i = 0; i < me->eamSize; i++){
		if (strcmp(me->effective_against_me[i]->type_name, other->type_name) == 0)
			return failure;
	}
	me->eamSize++;
	me->effective_against_me = (Type**)realloc(me->effective_against_me, (me->eamSize)*sizeof(Type*));
	if (me->effective_against_me == NULL)
		return memoryProblem;
	me->effective_against_me[me->eamSize - 1] = other;
	return success;
}

status addEAothers(Type* me, Type* other){
	if (me == NULL || other == NULL)
		return failure;
	for (int i = 0; i < me->eaoSize; i++){
		if (strcmp(me->effective_against_others[i]->type_name, other->type_name) == 0)
			return failure;
	}
	me->eaoSize++;
	me->effective_against_others = (Type**)realloc(me->effective_against_others, (me->eaoSize)*sizeof(Type*));
	if (me->effective_against_others == NULL)
		return memoryProblem;
	me->effective_against_others[me->eaoSize - 1] = other;
	return success;
}

status deleteEAme(Type* me, Type* other){
	if (me != NULL && other != NULL){
		for (int i = 0; i < me->eamSize; i++){
			if (strcmp(me->effective_against_me[i]->type_name, other->type_name) == 0)
			{
				me->eamSize--;
				if (me->eamSize == 0){
					free(me->effective_against_me);
					me->effective_against_me = (Type**)malloc(0);
				}
				else{
					for (int j = i; j < me->eamSize; j++)
						me->effective_against_me[j] = me->effective_against_me[j + 1];
					me->effective_against_me = (Type**)realloc(me->effective_against_me, (me->eamSize)*sizeof(Type*));
				}
				if (me->effective_against_me == NULL)
					return memoryProblem;
				return success;
			}
		}
	}
	return failure;
}

status deleteEAothers(Type* me, Type* other){
	if (me != NULL && other != NULL){
		for (int i = 0; i < me->eaoSize; i++){
			if (strcmp(me->effective_against_others[i]->type_name, other->type_name) == 0)
			{
				me->eaoSize--;
				if (me->eaoSize == 0){
					free(me->effective_against_others);
					me->effective_against_others = (Type**)malloc(0);
				}
				else{
					for (int j = i; j < me->eaoSize; j++)
						me->effective_against_others[j] = me->effective_against_others[j + 1];
					me->effective_against_others = (Type**)realloc(me->effective_against_others, (me->eaoSize)*sizeof(Type*));
				}
				if (me->effective_against_others == NULL)
					return memoryProblem;
				return success;
			}
		}
	}
	return failure;
}

status printPokemon(Pokemon* pokemon){
	if (pokemon != NULL){
		printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n", pokemon->pokemon_name, pokemon->species, pokemon->type->type_name, pokemon->bioInfo->height, pokemon->bioInfo->weight, pokemon->bioInfo->attack);
		return success;
	}
	return failure;
}

status printType(Type* type){
	if (type != NULL){
		printf("Type %s -- %d pokemons\n", type->type_name, type->type_amount);
		if (type->effective_against_me != NULL && type->eamSize != 0){
			int maxEMlen = 0;
			for (int curEM = 0; curEM < type->eamSize; curEM++){
				if (strlen(type->effective_against_me[curEM]->type_name) > maxEMlen)
					maxEMlen = strlen(type->effective_against_me[curEM]->type_name);
			}
			char againstMe[type->eamSize * (maxEMlen + 3)];
			strcpy(againstMe, type->effective_against_me[0]->type_name);
			char addMe[maxEMlen + 1];
			for (int i = 1; i < type->eamSize; i++){
				strcat(againstMe, " ,");
				strcpy(addMe, type->effective_against_me[i]->type_name);
				strcat(againstMe, addMe);
			}
			printf("\tThese types are super-effective against %s:%s\n", type->type_name, againstMe);
		}
		if (type->effective_against_others != NULL && type->eaoSize != 0){
			int maxEOlen = 0;
			for (int curEO = 0; curEO < type->eaoSize; curEO++){
				if (strlen(type->effective_against_others[curEO]->type_name) > maxEOlen)
					maxEOlen = strlen(type->effective_against_others[curEO]->type_name);
			}
			char againstOthers[type->eaoSize * (maxEOlen + 3)];
			strcpy(againstOthers, type->effective_against_others[0]->type_name);
			char addOthers[maxEOlen + 1];
			for (int i = 1; i < type->eaoSize; i++){
				strcat(againstOthers, " ,");
				strcpy(addOthers, type->effective_against_others[i]->type_name);
				strcat(againstOthers, addOthers);
			}
			printf("\t%s moves are super-effective against:%s\n", type->type_name, againstOthers);
		}
		printf("\n");
		return success;
	}
	return failure;
}

status freeType(Type *type){
	for (int eam = 0; eam < type->eamSize; eam++){
		if (type->effective_against_me[eam] != NULL)
			type->effective_against_me[eam] = NULL;
	}
	free(type->effective_against_me);
	type->effective_against_me = NULL;

	for (int eao = 0; eao < type->eaoSize; eao++){
		if (type->effective_against_others[eao] != NULL)
			type->effective_against_others[eao] = NULL;
	}
	free(type->effective_against_others);
	type->effective_against_others = NULL;

	free(type->type_name);
	type->type_name = NULL;

	free(type);
	type = NULL;

	return success;
}

status freeBio(Pokemon *pokemon){
	if (pokemon == NULL || pokemon->bioInfo == NULL)
		return failure;
	free(pokemon->bioInfo);
	pokemon->bioInfo = NULL;
	return success;
}

status freePokemon(Pokemon *pokemon){
	if (pokemon != NULL){
		pokemon->type->type_amount--;
		free(pokemon->pokemon_name);
		pokemon->pokemon_name = NULL;
		free(pokemon->species);
		pokemon->species = NULL;
		if (freeBio(pokemon) == success){
			free(pokemon);
			pokemon = NULL;
			return success;
		}
	}
	return failure;
}
