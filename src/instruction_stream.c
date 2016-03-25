#include "instruction_stream.h"

#define TOFILE  1

static FILE * fp;
static int pc = 0;


void istream_open() {
  fp = fopen ("bin/yaccacompilex", "w+");
}
// void istream_printf(char* format, ... ) {
//     va_list args;
//     fprintf( fp, "/!\\ Warning: " );
//     va_start( args, format );
//     vfprintf( fp, format, args );
//     va_end( args );
//     pc++;
// }

void istream_printf(char* format, ... ) {
  if (TOFILE) {
    va_list args;
    fprintf(fp, "%d: ", pc);
    va_start( args, format );
    vfprintf(fp, format, args );
    va_end( args );
    fprintf(stdout, "");
    pc++;
  } else {
    va_list args;
    fprintf(stdout, "istream_printf : " );
    va_start( args, format );
    vfprintf(stdout, format, args );
    va_end( args );
    fprintf(stdout, "");
    pc++;
  }
}


void istream_close() {
  fclose(fp);
}

int get_pc() {
  return pc;
}

char* get_tab(){
  char* test = arrayTab + 255 - current_tab;
  return test;
}

int get_current_tab(){
  return current_tab;
}

void add_tab(){
  current_tab += 4;
}

void remove_tab(){
  current_tab -= 4;
}
