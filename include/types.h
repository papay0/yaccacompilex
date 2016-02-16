#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_FUNC_ARGS 64
const int PTR_SIZE;

typedef int type_kind_t;
typedef int primitive_t;

// Differents types de types
const type_kind_t TYPE_KIND_PRIMITIVE;
const type_kind_t TYPE_KIND_POINTER;
const type_kind_t TYPE_KIND_FUNCTION;

// Taille des types primitifs en octets.
const int PRIM_SIZES[2];
const primitive_t PRIM_CHAR;
const primitive_t PRIM_INT;

// Type (générique)
typedef struct type {
	type_kind_t kind; // 
	void* padding[MAX_FUNC_ARGS+1];
} type_t;

// Type primitif
typedef struct primtype {
	type_kind_t kind;
	primitive_t primitive;
} primtype_t;

// Type fonction
typedef struct functype {
	type_kind_t kind;
	type_t* return_type;
	type_t* arg_types[MAX_FUNC_ARGS];
} functype_t;

// Type pointeur
typedef struct ptrtype {
	type_kind_t kind;
	type_t* type;
} ptrtype_t;




type_t* type_create_ptr(type_t* type);

type_t* type_create_primitive(char* name);

type_t* type_create_func(type_t* return_type, type_t** arg_types, int argc);

int type_equals(type_t* t1, type_t* t2);
