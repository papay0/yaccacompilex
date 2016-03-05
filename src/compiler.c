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
	return newaddr;
}

void do_affect(const char* symbol, expression_t expr, int unlock)
{
	int addr = expr.address;
	int addr2 = stable_find(symbols, symbol)->address;
	printf("COP %d %d\n", addr2, addr);
	
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
	r->type = symbol->type;
	r->address = symbol->address;

	printf("blbl\n");
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
