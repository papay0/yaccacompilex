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
void ctx_close();
// Génère le code l'operation donnée par opname ("ADD", "MULT", etc...) sur les expressions
// e1 et e2, et met le résultat dans l'expression r.
int do_operation(expression_t e1, expression_t e2, expression_t* r, const char* opname);
// Génère le code d'une affectation du symbole donné à l'expression donnée.
// Si unlock = 1 : la variable temporaire est releasée (cas d'une instruction d'affectation)
// Si unlock = 0 : la variable temporaire n'est pas releasée (cas d'une expression d'affectation)
void do_affect(const char* symbol, expression_t expr, int unlock);
void do_loadliteral(int literalValue, expression_t* r);
void do_loadsymbol(const char* name, expression_t* r);


// Variables globales
context_t ctx;
stable_t* symbols;

#endif
