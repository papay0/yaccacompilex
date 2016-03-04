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

int do_operation(expression_t e1, expression_t e2, 
	expression_t* r, const char* opname)
{
	int addr1 = e1.address;
	int addr2 = e2.address;
  	tempaddr_unlock(addr1);
  	tempaddr_unlock(addr2);
  	int newaddr = tempaddr_lock();
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
		tempaddr_unlock(addr);
	
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
