#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "types.h"


const int PTR_SIZE = 64;
// Differents types de types

const int TYPE_KIND_PRIMITIVE 	= 0x01;
const int TYPE_KIND_POINTER 	= 0x02;
const int TYPE_KIND_FUNCTION 	= 0x03;

// Taille des types primitifs en octets.
const int PRIM_SIZES[2] = { 1, 4 };
const char* PRIM_NAMES[2] = {"char", "int"};
const int PRIM_CHAR = 0x00;
const int PRIM_INT = 0x01;

int type_getsize(type_t* type)
{
	if(type->kind == TYPE_KIND_PRIMITIVE)
		return PRIM_SIZES[((primtype_t*)type)->primitive];
	else if(type->kind == TYPE_KIND_POINTER)
		return PTR_SIZE;
	else if(type->kind == TYPE_KIND_FUNCTION)
		return PTR_SIZE;
	else
		assert(0);
}

type_t* type_create_ptr(type_t* type)
{
	ptrtype_t* ptr = (ptrtype_t*)malloc(sizeof(ptrtype_t));
	ptr->type = type;
	ptr->kind = TYPE_KIND_POINTER;
	return ((type_t*)ptr);	 
}

type_t* type_create_primitive(char* name)
{
	int prim;
	primtype_t* type = (primtype_t*)malloc(sizeof(primtype_t));
	type->kind = TYPE_KIND_PRIMITIVE;
	if(strcmp(name, "char") == 0)
		type->primitive = PRIM_CHAR;
	else if(strcmp(name, "int") == 0)
		type->primitive = PRIM_INT;
	else
		assert(0);
	return ((type_t*)type);
}

type_t* type_create_func(type_t* return_type, type_t** arg_types, int argc)
{
	functype_t* func = (functype_t*)malloc(sizeof(functype_t));
	func->kind = TYPE_KIND_FUNCTION;
	func->return_type = return_type;
	for(int i = 0; i < argc; i++)
	{
		func->arg_types[i] = arg_types[i];
	}
	func->argc = argc;
	return ((type_t*)func);
}

void type_print(type_t* type)
{
	if(type->kind == TYPE_KIND_PRIMITIVE)
	{
		printf("%s", PRIM_NAMES[((primtype_t*)type)->primitive]);
	}
	else if(type->kind == TYPE_KIND_POINTER)
	{	
		type_print(((ptrtype_t*)type)->type);
		printf("*");
	}
	else if(type->kind == TYPE_KIND_FUNCTION)
	{
		functype_t* func = (functype_t*)type;
		type_print(func->return_type);
		printf("(*) (");
		for(int i = 0; i < func->argc; i++)
		{
			type_print(func->arg_types[i]);
			if(i != func->argc -1)
				printf(",");
		}
		printf(")"); 
	}
	else
	{
		print_debug("type_print: unknown type kind %d", type->kind);
	}
}

// TODO
int type_equals(type_t* t1, type_t* t2)
{
	
	
}
