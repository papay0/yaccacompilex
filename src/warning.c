#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "warning.h"

#define DEBUG     1

void print_debug(const char* format, ...) {
  if (DEBUG)
  {
    va_list args;
    fprintf(stdout, "\033[1;31m[Debug] "); 
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
    fprintf( stderr, "/!\\ Warning: " );
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
  }
  exit(0);
}
