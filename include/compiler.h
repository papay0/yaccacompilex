#ifndef COMPILER_H
#define COMPILER_H

#include "stable.h"
#include "ltable.h"
#include "gtable.h"
#include "idbuffer.h"
#include "types.h"

typedef struct expression
{
	int address;
	type_t* type;
} expression_t;

typedef struct context {
	int verbose;
} context_t;


void ctx_init();
void ctx_close();
// Génère le code l'operation donnée par opname ("ADD", "MULT", etc...) sur les expressions
// e1 et e2, et met le résultat dans l'expression r.
int do_operation(expression_t e1, expression_t e2, expression_t* r, char* opname);
// Génère le code d'une affectation du symbole donné à l'expression donnée.
// Si unlock = 1 : la variable temporaire est releasée (cas d'une instruction d'affectation)
// Si unlock = 0 : la variable temporaire n'est pas releasée (cas d'une expression d'affectation)
#define DOAFFECT_NONE		0x0
#define DOAFFECT_UNLOCK 	0x1
#define DOAFFECT_DEREFERENCE	0x2
void do_affect(char* symbol, expression_t expr, int op);
void do_loadliteral(int literalValue, expression_t* r);
void do_loadsymbol(char* name, expression_t* r);
void do_variable_declarations(type_t* type);
void do_variable_affectations(expression_t* expr);
void check_type_affect(type_t* dest, type_t* exprtype);

// Test de fonction pour le if, table des labels
void do_if(expression_t e);
void do_body();

type_t* do_makefunctype(type_t* return_type);

// Variables globales
context_t ctx;
stable_t* symbols;
ltable_t* labels;
gtable_t* globals;

#endif
