#include "instruction_stream.h"

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
  va_list args;
  fprintf(stdout, "istream_printf : " );
  va_start( args, format );
  vfprintf(stdout, format, args );
  va_end( args );
  fprintf(stdout, "");
  pc++;
}

void istream_close() {
  fclose(fp);
}

int get_pc() {
  return pc;
}
