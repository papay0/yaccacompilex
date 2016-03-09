#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stable.h"
#include "warning.h"
#include "instruction_stream.h"


const int SYMBOL_INITIALIZED 		= 0x01;
const int SYMBOL_CONST			= 0x02;
const int SYMBOL_LOCAL			= 0x04;
const int SYMBOL_STATIC			= 0x08;
const int SYMBOL_FUNC			= 0x10;


symbol_t* symbol_new(char* chr, int address, int depth, type_t* type)
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
	s->type = type;
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
	s->last = NULL;
	s->current_depth = 0;
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

			// Branchement de this->last
			if(next == NULL)
			{
				this->last = prev;
			}

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

void stable_add(stable_t* this, char* name, type_t* type)
{
	symbol_t* sameSymbol = stable_find(this, name);
	int depth = this->current_depth;
	if (sameSymbol != NULL && sameSymbol->depth == depth)
	{
		print_warning("Variable %s existe deja dans la même portée.\n", name);
	}

	symbol_t* symbol = symbol_new(name, -1, depth, type);
	if(this->first == NULL)
	{
		this->first = symbol;
		symbol->address = 0;
	}
	else
	{
		symbol_t* current = this->first;
		while(current->next != NULL)
		{
			current = current->next;
		}
		current->next = symbol;
		symbol->address = current->address+1;
	}

	this->last = symbol;
}

// Retourne le symbole dont le nom est donné par name.
// Cette fonction retourne l'addresse du dernier symbole trouvé
// (profondeur la plus élevée).
// Retourne NULL s'il n'existe pas.
symbol_t* stable_find(stable_t* this, char* name)
{
	symbol_t* current = this->first;
	symbol_t* result = NULL;
	while(current != NULL)
	{
		if (strcmp(name, current->name) == 0)
			result = current;
		current = current->next;
	}
	// printf("[stable_find] warning: symbol %s not found\n", name);
	return result;
}

void stable_setflags(stable_t* this, char* name, int flags)
{
	symbol_t* symbol = stable_find(this, name);
	if(symbol == NULL)
	{
		print_debug("error: symbol %s not found, segfault inc !", name);
	}
	symbol->flags = flags;
}

void stable_print(stable_t* this)
{
	symbol_t* current = this->first;
	printf("--------------  Symbols Table  -------------\n");
	printf("| ID      |Address |FLAGS   |DEPTH   | Type \n");
	while(current != NULL)
	{

		printf("| %8s|%8d|%8d|%8d| ", current->name, current->address,
				current->flags, current->depth);
		type_print(current->type);
		printf("\n");
		current = current->next;
	}
	printf("--------------------------------------------\n");
}

void stable_block_enter(stable_t* this)
{
	char* tab = get_tab();
	print_debug("%sEntering block...\n", tab);
	this->current_depth++;
	add_tab();
}

void stable_block_exit(stable_t* this)
{
	remove_tab();
	char* tab = get_tab();
	print_debug("%sExiting block... \n", tab);
	print_debug("%sSymbol table before exit : \n", tab);
	stable_print(this);
	stable_remove(this, this->current_depth);
	this->current_depth--;
}
/* ----------------------------------------------------------------------------
 * TEMPADDR
 * --------------------------------------------------------------------------*/

void tempaddr_init()
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		tempaddr[i].locked = 0;
		tempaddr[i].id = i;
	}
}

int tempaddr_lock(stable_t* symbols)
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		if(!tempaddr[i].locked) {
			tempaddr[i].locked = 1;
			if(symbols->last == NULL)
				tempaddr[i].address = i;
			else
				tempaddr[i].address = symbols->last->address + i;
			return tempaddr[i].address;
		}
	}
	print_debug("lock_tempaddr: not enough addresses\n");
	return -1;
}

void tempaddr_unlock(stable_t* symbols, int addr)
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		if(tempaddr[i].address == addr) {
			tempaddr[i].locked = 0;
			return;
		}
	}
	print_debug("unlock_tempaddr: no var to unlock");
}

int test_stable()
{
	type_t* type = type_create_primitive("int");
	stable_t* s = stable_new();
	stable_add(s, "a", type);
	stable_add(s, "b", type);
	stable_block_enter(s);
	stable_add(s, "c", type);
	stable_add(s, "d", type);
	stable_block_exit(s);
	stable_block_exit(s);
	stable_add(s, "a", type);
	stable_print(s);
	if(s->last == NULL)
		printf("LAST : null");
	else
		printf("LAST : %s\n", s->last->name);

	return 0;
}
