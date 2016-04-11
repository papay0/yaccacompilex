#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static int current_tab;
static char arrayTab[256] = {[0 ... 254] = ' ', '\0'};

void istream_open(char* outfile);
void istream_printf(char* format, ... );
void istream_close();
int get_pc();
char* get_tab();
void add_tab();
void remove_tab();
