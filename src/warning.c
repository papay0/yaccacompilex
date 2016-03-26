#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "warning.h"

#define DEBUG     1

// Numéro de ligne de lex
extern int yylineno;

void print_debug(const char* format, ...) {
  if (DEBUG)
  {
    va_list args;
    fprintf(stdout, "\033[1;32m[Debug] "); 
	va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
	fprintf(stdout, "\033[0m");
  }
}

void print_warning( const char* format, ... ) {
  if (DEBUG)
  {
    va_list args;
    fprintf(stdout, "\033[1;31mwarning line %d: ", yylineno );
    va_start( args, format );
    vfprintf(stdout, format, args );
    va_end( args );
	fprintf(stdout, "\033[0m");
  }
}

void print_wnotes(const char* format, ... ) {
  if (DEBUG)
  {
    va_list args;
    fprintf(stdout, "\033[1;31m\t" );
    va_start( args, format );
    vfprintf(stdout, format, args );
    va_end( args );
	fprintf(stdout, "\033[0m");
  }
}


