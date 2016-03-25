#include <stdlib.h>
#include <stdio.h>
#include "ltable.h"

const int LTABLE_MAIN = 0;
const int LTABLE_BOOTSTRAP = 1;


ltable_t* ltable_new() {
    ltable_t* l = (ltable_t*)malloc(sizeof(ltable_t));
    // Début à 2 :
    // 0 = MAIN
    // 1 = BOOTSTRAP
    l->labels[0] = 777;
    l->labels[1] = 777;
    l->index = 2; 
    return l;
}

void ltable_add(ltable_t* this, int address) {
    this->labels[this->index] = address;
    this->index++;
}

int ltable_getindex(ltable_t* this) {
    return this->index;
}

int  ltable_get_main(ltable_t* this)
{
    return LTABLE_MAIN; 
}

void ltable_set_main(ltable_t* this, int address)
{
    printf("setmain %d\n", address);
    this->labels[LTABLE_MAIN] = address;
}

int  ltable_get_bootstrap(ltable_t* this)
{
    return LTABLE_BOOTSTRAP;
}
void ltable_set_bootstrap(ltable_t* this, int address)
{
    printf("setbootstrap %d\n", address);
    this->labels[LTABLE_BOOTSTRAP] = address;
}


void ltable_print(ltable_t* this) {
  printf("--------------  Labels Table  --------------\n");
	printf("| ID                 |  Address            |\n");
  for (int i = 0; i < this->index; i++) {
    printf("|%20d|%21d|\n", i, this->labels[i]);
  }
  printf("--------------------------------------------\n");
  // test : int main(){ int a = 1; if (a==2){ int b = 2; } }
}
