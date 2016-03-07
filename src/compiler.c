#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "instruction_stream.h"

void ctx_init()
{
	ctx.depth = 0;
	tempaddr_init();	
	symbols = stable_new();
	istream_open();
}

void ctx_close()
{
	istream_close();
	
}
int do_operation(expression_t e1, expression_t e2, 
	expression_t* r, const char* opname)
{
	int addr1 = e1.address;
	int addr2 = e2.address;
  	tempaddr_unlock(symbols, addr1);
  	tempaddr_unlock(symbols, addr2);
  	int newaddr = tempaddr_lock(symbols);
  	printf("%s %d %d %d\n", opname, newaddr, addr1, addr2);
	r->address = newaddr;

	// TODO : type check
	if(!type_compatible(e1.type, e2.type, 0))
	{
		print_warning("incompatible-pointer-types\n");
		print_wnotes("note: cannot perform operation %s on types '", opname);
		type_print(e1.type);
		print_wnotes("' and '");
		type_print(e2.type);
		print_wnotes(".\n"); 
	}
	r->type = e1.type; // FIXME
	return newaddr;
}

int do_unary_operation(expression_t e1, 
	expression_t* r, const char* opname)
{
	int addr1 = e1.address;
  	tempaddr_unlock(symbols, addr1);
  	int newaddr = tempaddr_lock(symbols);
  	printf("%s %d %d\n", opname, newaddr, addr1);
	r->address = newaddr;
	// TODO : type check
	r->type = e1.type; // FIXME
	return newaddr;
}

void check_type_affect(type_t* dest, type_t* exprtype)
{
	if(!type_compatible(dest, exprtype, 0))
	{
		print_warning("incompatible-pointer-types : \n");
		print_wnotes("note: types '");
		type_print(dest);
		print_wnotes("' and '");
		type_print(exprtype);
		print_wnotes("' are not compatible for affectation.\n"); 
	}
}

void do_affect(const char* name, expression_t expr, int unlock)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) 	{
		print_warning("symbol %s not found.", name);
	}
	int addr = expr.address;
	int addr2 = symbol->address;

	printf("COP %d %d\n", addr2, addr);
	
	check_type_affect(expr.type, symbol->type);
	if(unlock)
		tempaddr_unlock(symbols, addr);
}

void do_loadliteral(int literalValue, expression_t* r)
{
	int addr = tempaddr_lock(symbols);
  	printf("AFC %d %d\n", addr, literalValue);
  	r->address = addr;
  	r->type = type_create_primitive("int");
}

void do_loadsymbol(const char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) {
		print_warning("symbol %s not found.", name);
	}
	int addr = tempaddr_lock(symbols);
	printf("COP %d %d\n", addr, symbol->address);
	r->type = symbol->type;
	r->address = addr;
}


void do_variable_declarations(type_t* type)
{
	for(int i = 0; i < idbuffer_size(); i++)
	{
		stable_add(symbols, (char*)idbuffer_get(i), type);
	}
}

void do_variable_affectations(expression_t* expr)
{
	for(int i = 0; i < idbuffer_size(); i++)
	{
		const char* symbol = (const char*)idbuffer_get(i);
		do_affect(symbol, *expr, 1);
	}
}

type_t* do_makefunctype(type_t* return_type) 
{
  	type_t** args = (type_t**)malloc(sizeof(type_t*)*idbuffer_size());
  	for(int i = 0; i < idbuffer_size(); i++)
	{
    	args[idbuffer_size() - i - 1] = idbuffer_get(i);
	}
	type_t* func = type_create_func(return_type, args, idbuffer_size()); 
	return func;
}

void do_dereference(const char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) {
		print_warning("symbol %s not found.", name);
	}
	r->type = type_create_ptr(symbol->type);
	r->address = tempaddr_lock(symbols);	
	printf("AFC %d %d\n", r->address, symbol->address);
}
/*
int main()
{
	stable_t* stable = stable_new();
	stable_add(stable, "a", 5, 0, 4);
	stable_add(stable, "b", 130, 0, 4);
	stable_add(stable, "c", 12, 1, 4);
	stable_setflags(stable, "b", SYMBOL_INITIALIZED | SYMBOL_CONST);
	stable_remove(stable, -1);
	stable_add(stable, "d", 12, 0, 4);
	stable_print(stable);
}
*/
