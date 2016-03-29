#include "instruction_stream.h"
#include "ltable.h"

#define TOFILE  1

static FILE * fp;
FILE * fr;
static int pc = 0;


void istream_open() {
  fp = fopen ("bin/tmp_yaccacompilex", "w+");
  printf("J'ai open fp\n");
  //fr = fopen ("bin/yaccacompilex", "w+");
}

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


void update_label(ltable_t* this){
  //printf("I start updating the label\n");
  //FILE *fr;
  fp = fopen ("bin/tmp_yaccacompilex", "r");
  fr = fopen ("bin/yaccacompilex", "w");
  char line[256];
  //fr = fopen ("bin/tmp_yaccacompilex", "r");
  char instruction[256], else_instr[256];
  int line_number = 0;
  int arg1 = 0;
  int arg2 = 0;
  int value_label_tabel = 0;
  while(fgets(line, 256, fp) != NULL)
  {
    sscanf(line, "%d: %s",&line_number, instruction);
    //printf("Line number = %d\n", line_number);
    //printf("Instr = %s\n", instruction);
    //printf("--> %s\n", line);
    if (strcmp(instruction, "JMF") == 0) {
      sscanf(line, "%d: %s %d %d",&line_number, instruction, &arg1, &arg2);
      //printf("Je dois changer mon label à la ligne %d de mon label table\n", arg2);
      value_label_tabel = get_value_at_index(this, arg2);
      //printf("value at index %d is %d\n", arg2, value_label_tabel);
      fprintf(fr, "%d: %s %d %d\n",line_number, instruction, arg1, value_label_tabel);
    } else if (strcmp(instruction, "JMP") == 0) {
      sscanf(line, "%d: %s %d",&line_number, instruction, &arg1);
      value_label_tabel = get_value_at_index(this, arg1);
      fprintf(fr, "%d: %s %d\n",line_number, instruction, value_label_tabel);
    } else {
      fprintf(fr, "%s", line);
    }

  }
  //printf("Je close my file\n");
  fclose(fr);
  fclose(fp);
  //printf("J'ai closé my file\n");
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
