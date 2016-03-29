#ifndef LTABLE_H
#define LTABLE_H

const int LTABLE_MAIN;
const int LTABLE_BOOTSTRAP;

typedef struct ltable {
    int labels[256];
    int l_main; // label main
    int l_global;
    int index;
} ltable_t;

ltable_t* ltable_new();
void ltable_add(ltable_t* this, int address);
// Obtient l'index de l'entrée "main" dans la table des symboles
int  ltable_get_main(ltable_t* this);
// Définit l'adresse de l'entrée "main" dans la table des symboles.
void ltable_set_main(ltable_t* this, int address);
int  ltable_get_bootstrap(ltable_t* this);
void ltable_set_bootstrap(ltable_t* this, int address);
void ltable_print(ltable_t* this);
int get_value_at_index(ltable_t* this, int index);

#endif
