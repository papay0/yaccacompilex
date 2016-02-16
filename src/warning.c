#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "warning.h"

void print_warn(char* message, int print)
{
  if (print) {
    printf("/!\\ %s /!\\ \n", message);
    exit(0);
  }
}
