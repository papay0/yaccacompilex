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
	symbol_t* last;
	int current_depth;
} stable_t;

typedef struct tempaddr {
	int id;
	int locked;
	int address;
} tempaddr_t;


/* -------------------------------------------------------
 * GLOBALES
 * -----------------------------------------------------*/

#define TEMPADDR_COUNT 4
tempaddr_t tempaddr[TEMPADDR_COUNT];

/* -------------------------------------------------------
 * FONCTIONS
 * -----------------------------------------------------*/
// Crée un nouveau symbole avec le nom et l'addresse donnee.
symbol_t* symbol_new(char* name, int address, int depth, type_t* type);
// Libère la mémoire allouée par le symbole donné.
void symbol_free(symbol_t* this);
// Crée une nouvelle table de symboles vide.
stable_t* stable_new();
// Libère la mémoire allouée par la table de symbole.
void stable_free(stable_t* this);
// Ajoute un symbole dans la table de symboles donnee.
void stable_add(stable_t* this, char* name, type_t* type);
// Supprime tous les symboles à une profondeur donnée.
void stable_remove(stable_t* this, int depth);

// Retourne le symbole dont le nom est donné par name.
// Cette fonction retourne l'addresse du dernier symbole trouvé
// (profondeur la plus élevée).
// Retourne NULL s'il n'existe pas.
symbol_t* stable_find(stable_t* this, char* name);
int stable_setflags(stable_t* this, char* name, int flags);
int stable_print(stable_t* this);
// Signale à la table des symboles l'entrée dans un block.
// Cela a pour effet d'augmenter la profondeur actuelle.
int stable_block_enter();
// Signale à la table des symboles la sortie d'un block.
// Cela a pour effet de décrémenter la profondeur actuelle et 
// supprimer les variables de la profondeur précédente.
int stable_block_exit();
/* ----------------------------------------------------------------------------
 * TEMPADDR
 * --------------------------------------------------------------------------*/
int tempaddr_init();
int tempaddr_lock(stable_t* symbols);
void tempaddr_unlock(stable_t* symbols, int addr);
#endif
