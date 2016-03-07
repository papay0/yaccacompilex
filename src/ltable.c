#include "ltable.h"

ltable_t* ltable_new() {
    ltable_t* l = (ltable_t*)malloc(sizeof(ltable_t));
    l->index = 0;
    return l;
}

void ltable_add(ltable_t* this, int address) {
    this->labels[this->index] = address;
    this->index++;
}

void ltable_print(ltable_t* this) {
    for (int i = 0; i < this->index; i++) {
        printf("i = %d, address = %d\n", i, this->labels[i]);
    }
}
