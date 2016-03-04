#ifndef STABLE_H
#define STABLE_H

#include "types.h"

typedef struct symbol symbol_t;

const int SYMBOL_INITIALIZED;
const int SYMBOL_CONST;
const int SYMBOL_LOCAL;
const int SYMBOL_STATIC;

typedef struct context {
	int depth;
} context_t;

// Represente un symbole dans la table de symboles.
typedef struct symbol {
	symbol_t* next;
	char*  name;
	int address;
	int flags;
	int depth;
	type_t* type;
} symbol_t;

// Represente une table de symboles.
typedef struct symbol_table {
	symbol_t* first;
} stable_t;

// Crée un nouveau symbole avec le nom et l'addresse donnee.
symbol_t* symbol_new(char* name, int address, int depth, type_t* type);
// Libère la mémoire allouée par le symbole donné.
void symbol_free(symbol_t* this);
// Crée une nouvelle table de symboles vide.
stable_t* stable_new();
// Libère la mémoire allouée par la table de symbole.
void stable_free(stable_t* this);
// Ajoute un symbole dans la table de symboles donnee.
void stable_add(stable_t* this, char* name, int address, int depth, type_t* type);
// Supprime tous les symboles à une profondeur donnée.
void stable_remove(stable_t* this, int depth);
// Trouve et retourne le symbole.
symbol_t* stable_find(stable_t* this, char* name);
int stable_setflags(stable_t* this, char* name, int flags);
int stable_print(stable_t* this);

#endif
