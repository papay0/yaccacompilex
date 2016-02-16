#ifndef COMPILER_H
#define COMPILER_H

#include "stable.h"
#include "idbuffer.h"

void ctx_init();

// Variables globales
context_t ctx;
stable_t* symbols;

#endif
