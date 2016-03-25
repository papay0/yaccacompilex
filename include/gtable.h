#ifndef GTABLE_H
#define GTABLE_H

#include "ltable.h"

#define GTABLE_SIZE 256

typedef struct gtable_entry {
    int addr; 
    int value;
} gtable_entry_t;

typedef struct gtable
{
    gtable_entry_t entries[GTABLE_SIZE];
    int index;
} gtable_t;


// Crée une nouvelle table des fonctions
gtable_t* gtable_new();
// Ajoute une fonction à la table des fonctions
// addr : adresse de la globale
// value : valeur de la globale
void gtable_add(gtable_t* this, int addr, int value);
// Ajoute le code de la table des fonctions dans le flux d'instructions.
void gtable_printtostream(gtable_t* this, ltable_t* labels);

#endif
