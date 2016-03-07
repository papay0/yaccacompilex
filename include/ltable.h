#ifndef LTABLE_H
#define LTABLE_H

typedef struct ltable {
    int labels[256];
    int index;
} ltable_t;

ltable_t* ltable_new();
void ltable_add(ltable_t* this, int address);
void ltable_print(ltable_t* this);

#endif
