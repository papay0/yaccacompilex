#ifndef COMPILER_H
#define COMPILER_H

#include "stable.h"
#include "idbuffer.h"
#include "types.h"

typedef struct expression
{
	int address;
	type_t* type;
} expression_t;

void ctx_init();

// Variables globales
context_t ctx;
stable_t* symbols;

#endif
