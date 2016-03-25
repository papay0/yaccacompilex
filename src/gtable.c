#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gtable.h"
#include "warning.h"
#include "instruction_stream.h"


gtable_t* gtable_new()
{
    return (gtable_t*)malloc(sizeof(gtable_t));
}

void gtable_add(gtable_t* this, int addr, int value)
{
    this->entries[this->index].addr = addr;
    this->entries[this->index].value = value;
    this->index++;
}

void gtable_printtostream(gtable_t* this)
{
    for(int i = 0; i < this->index; i++) 
    {
        gtable_entry_t* e= &this->entries[i];
        istream_printf("AFC @%d %d\n", e->addr, e->value);
    }
}
