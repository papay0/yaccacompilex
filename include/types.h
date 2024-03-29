#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef TYPES_H
#define TYPES_H
#define MAX_FUNC_ARGS 64
const int PTR_SIZE;

// Differents types de types
const int TYPE_KIND_PRIMITIVE;
const int TYPE_KIND_POINTER;
const int TYPE_KIND_FUNCTION;

const int OPTYPE_ARITHMETICS;
const int OPTYPE_AFFECT;	
const int OPTYPE_EQUALS;
const int OPTYPE_LOGIC;
const int OPTYPE_STRICT;


// Taille des types primitifs en octets.
const int PRIM_SIZES[3];
const int PRIM_CHAR;
const int PRIM_INT;
const int PRIM_VOID;

// Type (générique)
typedef struct type {
	int kind; //
	void* data[MAX_FUNC_ARGS];
} type_t;

// Type primitif
typedef struct primtype {
	int kind;
	int primitive;
} primtype_t;

// Type fonction
typedef struct functype {
	int kind;
	type_t* return_type;
	type_t* arg_types[MAX_FUNC_ARGS];
	int argc;
} functype_t;

// Type pointeur
typedef struct ptrtype {
	int kind;
	type_t* type;
} ptrtype_t;

type_t* type_create_ptr(type_t* type);

type_t* type_create_primitive(char* name);

type_t* type_create_func(type_t* return_type, type_t** arg_types, int argc);

int type_compatible(type_t* t1, type_t* t2, int equals);

int type_equals(type_t* t1, type_t* t2);

void type_print(type_t* type);
// Affiche le type avec la fonction d'affichage donnée.
void type_sprint(void (*pf) (const char *, ...), type_t* type);


#endif
