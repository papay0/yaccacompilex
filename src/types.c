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

int type_equals(type_t* t1, type_t* t2)
{
	return type_compatible(t1, t2, 1);
}

// Retourne vrai si 2 types sont compatibles.
// 2 types sont compatibles s'ils sont égaux, à l'exception près
// du type primitif de départ.
// Si equals vaut vrai, la fonction ne retournera vrai que si les 
// 2 types sont strictements égaux.
// Exemples :
// t1     t2    compatible? equals?
// int    int   Yes         Yes
// int    char  Yes         No
// int*   char* Yes         No
// int*   int** No          No
int type_compatible(type_t* t1, type_t* t2, int equals)
{
	if(t1->kind == t2->kind)
	{
		if(t1->kind == TYPE_KIND_PRIMITIVE)
		{
			primtype_t* prim1 = (primtype_t*)t1;
			primtype_t* prim2 = (primtype_t*)t2;
			if(equals)
				return prim1->primitive == prim2->primitive;
			return 1;	
		}
		else if(t1->kind == TYPE_KIND_POINTER)
		{
			ptrtype_t* ptr1 = (ptrtype_t*)t1;
			ptrtype_t* ptr2 = (ptrtype_t*)t2;
			return type_compatible(ptr1->type, ptr2->type, equals);
		}
		else if(t1->kind == TYPE_KIND_FUNCTION)
		{
			functype_t* f1 = (functype_t*)t1;
			functype_t* f2 = (functype_t*)t2;
			if(!type_compatible(f1->return_type, f2->return_type, equals))
				return 0;
			if(f1->argc != f2->argc)
				return 0;
			for(int i = 0; i < f1->argc; i++)
			{
				if(!type_compatible(f1->arg_types[i], f2->arg_types[i], equals))
					return 0;
			}
			return 1;
		}
		else
		{
			print_debug("type_compatible: unknown type kind %d", t1->kind);
		}
	}
	return 0;
	
}
