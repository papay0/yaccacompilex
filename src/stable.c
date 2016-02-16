#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stable.h"
#include "warning.h"

#define DEBUG       1

const int SYMBOL_INITIALIZED 		= 0x01;
const int SYMBOL_CONST			= 0x02;
const int SYMBOL_LOCAL			= 0x04;
const int SYMBOL_STATIC			= 0x08;
const int SYMBOL_FUNC			= 0x10;


symbol_t* symbol_new(char* chr, int address, int depth, int size)
{
	// On crée une copie du string.
	char* cpy = malloc(1+strlen(chr));
	strcpy(cpy, chr);

	symbol_t* s = (symbol_t*)malloc(sizeof(symbol_t));
	s->name = cpy;
	s->address = address;
	s->next = NULL;
	s->flags = 0;
	s->depth = depth;
	s->size = size;
	return s;
}

void symbol_free(symbol_t* this)
{
	free(this->name);
	free(this);
}

// Crée la table de symboles
stable_t* stable_new()
{
	stable_t* s = (stable_t*)malloc(sizeof(stable_t));
	s->first = NULL;
	return s;
}

void stable_remove(stable_t* this, int depth)
{
	symbol_t* current = this->first;
	symbol_t* prev = NULL;
	while(current != NULL)
	{
		if(current->depth == depth)
		{
			symbol_t* next = current->next;
			if(prev != NULL)
				prev->next = next;
			else
				this->first = next;
			symbol_free(current);

			current = next;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
}

void stable_free(stable_t* this)
{
	// todo faire ça bien
	free(this);
}

void stable_add(stable_t* this, char* name, int address, int depth, int size)
{
	if (stable_find(this, name) != NULL)
	{
		print_warn("Variable existe deja", DEBUG);
	}
	symbol_t* symbol = symbol_new(name, address, depth, size);
	if(this->first == NULL)
	{
		this->first = symbol;
	}
	else
	{
		symbol_t* current = this->first;
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = symbol;
	}
}

// Retourne l'address du symbole dont le nom est donné par name.
// Retourne NULL s'il n'existe pas.
symbol_t* stable_find(stable_t* this, char* name)
{
	symbol_t* current = this->first;
	while(current != NULL)
	{
		if(strcmp(name, current->name) == 0)
			return current;
		current = current->next;
	}
	return NULL;
}

int stable_setflags(stable_t* this, char* name, int flags)
{
	symbol_t* symbol = stable_find(this, name);
	if(symbol == NULL) return -1;
	symbol->flags = flags;
}

int stable_print(stable_t* this)
{
	symbol_t* current = this->first;
	printf("ID   |Address |FLAGS |DEPTH\n");
	while(current != NULL)
	{

		printf("%5s|%8d|%6d|%5d\n", current->name, current->address,
				current->flags, current->depth);
		current = current->next;
	}
	return -1;
}
