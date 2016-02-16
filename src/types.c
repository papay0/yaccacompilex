#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "types.h"


const int PTR_SIZE = 64;
// Differents types de types
const type_kind_t TYPE_KIND_PRIMITIVE = 0x01;
const type_kind_t TYPE_KIND_POINTER 	= 0x02;
const type_kind_t TYPE_KIND_FUNCTION 	= 0x03;

// Taille des types primitifs en octets.
const int PRIM_SIZES[2] = { 1, 4 };
const primitive_t PRIM_CHAR = 0x00;
const primitive_t PRIM_INT = 0x01;

type_t type_create_ptr(type_t* type)
{
	ptrtype_t ptr;
	ptr.type = type;
	ptr.kind = TYPE_KIND_POINTER;
	return (type_t)*((type_t*)&ptr);	 
}

type_t type_create_primitive(char* name)
{
	primitive_t prim;
	primtype_t type;
	type.kind = TYPE_KIND_PRIMITIVE;
	if(strcmp(name, "char") == 0)
		type.primitive = PRIM_CHAR;
	else if(strcmp(name, "int") == 0)
		type.primitive = PRIM_INT;
	else
		assert(0);
	return (type_t)*((type_t*)&type);
}

type_t type_create_func(type_t* return_type, type_t** arg_types, int argc)
{
	functype_t func;
	func.kind = TYPE_KIND_FUNCTION;
	func.return_type = return_type;
	for(int i = 0; i < argc; i++)
	{
		func.arg_types[i] = &arg_types[i];
	}
	return (type_t)*((type_t*)&func);
}

// TODO
int type_equals(type_t* t1, type_t* t2)
{
	
	
}
