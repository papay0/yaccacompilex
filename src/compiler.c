#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

void ctx_init()
{
	ctx.depth = 0;
	symbols = stable_new();
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
