#ifndef Pokemons_H
#define Pokemons_H
#include "Defs.h"

//Pokemon's type struct.
typedef struct p_type {
	char *type_name;
	int type_amount;
	struct p_type **effective_against_me;
	int eamSize;
	struct p_type **effective_against_others;
	int eaoSize;
} Type;

//Pokemon's Biological information struct.
typedef struct p_bioInfo {
	float height;
	float weight;
	int attack;
} BioInfo;

//Pokemon struct.
typedef struct p_pokemon {
	char *pokemon_name;
	char *species;
	Type *type;
	BioInfo *bioInfo;
} Pokemon;

/*Initializing a pokemon with all of the pokemon's details.
Parameters - the new pokemon's name, species, height, weight, attack and type.
returns- pointer to the new pokemon.*/
Pokemon* createPokemon(char*, char*, float, float, int, Type*);

/*Initializing a pokemon's type containing only the type's name.
Parameters - the new type's name.
returns- pointer to the new type.*/
Type* createType(char*);

/*Initializing a pokemon's biological information for createPokemon function to use.
Parameters - the new pokemon's height, weight and attack.
returns- pointer to the new type.*/
BioInfo* createBio(float, float, int);

/*Adds a pokemon's type to a type's effective against me list.
Parameters - a pokemon's type to add to and a pokemon's type to add.
returns- a status.*/
status addEAme(Type*, Type*);

/*Adds a pokemon's type to a type's effective against others list.
Parameters - a pointer to pokemon's type to add to and a pointer to pokemon's type to add.
returns- a status.*/
status addEAothers(Type*, Type*);

/*Removes a pokemon's type from a type's effective against me list.
Parameters - a pointer to pokemon's type to remove from and a pointer to pokemon's type to remove.
returns- a status.*/
status deleteEAme(Type*, Type*);

/*Removes a pokemon's type from a type's effective against others list.
Parameters - a pointer to pokemon's type to remove from and a pointer to pokemon's type to remove.
returns- a status.*/
status deleteEAothers(Type*, Type*);

/*Prints a pokemon with all it's details.
Parameters - a pointer to a pokemon.
returns- a status.*/
status printPokemon(Pokemon*);

/*Prints a pokemon's type with all it's details.
Parameters - a pointer to a pokemon's type.
returns- a status.*/
status printType(Type*);

/*Frees a pokemon's type.
Parameters - a pointer to a pokemon's type.
returns- a status.*/
status freeType(Type*);

/*Frees a pokemon (and it's BioInfo).
Parameters - a pointer to a pokemon.
returns- a status.*/
status freePokemon(Pokemon*);

#endif
