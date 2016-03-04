#include "instruction_stream.h"

FILE * fp;

void istream_open() {
    fp = fopen ("yaccacompilex", "w+");
}

void istream_printf(char* format, ... ) {
    va_list args;
    fprintf( fp, "/!\\ Warning: " );
    va_start( args, format );
    vfprintf( fp, format, args );
    va_end( args );
}

void istream_close() {
    fclose(fp);
}
