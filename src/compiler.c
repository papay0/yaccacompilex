#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"
#include "instruction_stream.h"
#include "warning.h"

void ctx_init()
{
	ctx.depth = 0;
	tempaddr_init();
	symbols = stable_new();
	labels = ltable_new();
	istream_open();
}

void ctx_close()
{
	istream_close();
}
int do_operation(expression_t e1, expression_t e2,
	expression_t* r, char* opname)
{
	int addr1 = e1.address;
	int addr2 = e2.address;
  	tempaddr_unlock(symbols, addr1);
  	tempaddr_unlock(symbols, addr2);
  	int newaddr = tempaddr_lock(symbols);
	istream_printf("%s %d %d %d\n", opname, newaddr, addr1, addr2);
	r->address = newaddr;

	// TODO : type check
	if(!type_compatible(e1.type, e2.type, type_getoptype(opname)))
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

void do_if(expression_t cond){
	istream_printf("JMF %d %d\n", cond.address, labels->index);
	ltable_add(labels, -1);
}

void do_body(){
	int last_index = -1;
	for (int i = 0; i < labels->index; i++) {
			if (labels->labels[i] == -1) {
				last_index = i;
			}
	}
	labels->labels[last_index] = get_pc();
	ltable_print(labels);
}



int do_unary_operation(expression_t e1,
	expression_t* r, char* opname)
{
	int addr1 = e1.address;
  	tempaddr_unlock(symbols, addr1);
  	int newaddr = tempaddr_lock(symbols);
  	//printf("%s %d %d\n", opname, newaddr, addr1);
	istream_printf("%s %d %d\n", opname, newaddr, addr1);
	r->address = newaddr;
	// TODO : type check
	if(strcmp(opname, "COPA") == 0)
	{
		if(e1.type->kind != TYPE_KIND_POINTER)
		{
			print_warning("invalid-dereference: trying to dereference non-pointer type '");
			type_print(e1.type);
			print_wnotes("'.\n");
		}
		else
		{
			ptrtype_t* ptr = (ptrtype_t*)e1.type;
			r->type = ptr->type;
		}
	}
	else
	{
		r->type = e1.type; // FIXME
	}
	return newaddr;
}

void check_type_affect(type_t* exprtype, type_t* dest)
{
	if(!type_compatible(dest, exprtype, OPTYPE_AFFECT))
	{
		print_warning("incompatible-pointer-types : \n");
		print_wnotes("note: types '");
		type_print(dest);
		print_wnotes("' and '");
		type_print(exprtype);
		print_wnotes("' are not compatible for affectation.\n");
	}
}

void do_affect(char* name, expression_t expr, int unlock)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) 	{
		print_warning("symbol %s not found.", name);
	}
	int addr = expr.address;
	int addr2 = symbol->address;

	istream_printf("COP %d %d\n", addr2, addr);

	check_type_affect(expr.type, symbol->type);
	if(unlock)
		tempaddr_unlock(symbols, addr);
}

void do_loadliteral(int literalValue, expression_t* r)
{
	int addr = tempaddr_lock(symbols);
	istream_printf("AFC %d %d\n", addr, literalValue);
  	r->address = addr;
  	r->type = type_create_primitive("int");
}

void do_loadsymbol( char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) {
		print_warning("symbol %s not found.\n", name);
	}
	int addr = tempaddr_lock(symbols);
	istream_printf("COP %d %d\n", addr, symbol->address);
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
		char* symbol = (char*)idbuffer_get(i);
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

void do_reference(char* name, expression_t* r)
{
	symbol_t* symbol = stable_find(symbols, name);
	if(symbol == NULL) {
		print_warning("symbol %s not found.", name);
	}
	r->type = type_create_ptr(symbol->type);
	r->address = tempaddr_lock(symbols);
	istream_printf("AFC %d %d\n", r->address, symbol->address);
}

void do_indexing(expression_t array, expression_t index, expression_t* r)
{
	expression_t tmp;
	do_operation(array, index, &tmp, "ADD");
	do_unary_operation(tmp, r, "COPA");
	istream_printf("ptr %p\n", r->type);
}
/* arr[i] *(arr+i) */
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
