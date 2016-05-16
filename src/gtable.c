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

void gtable_printtostream(gtable_t* this, ltable_t* labels)
{
    // Commence la zone de bootstrap
    istream_printf(".area bootstrap\n");
    ltable_set_bootstrap(labels, get_pc());

    for(int i = 0; i < this->index; i++) 
    {
        gtable_entry_t* e= &this->entries[i];
        istream_printf("AFC @%d %d\n", e->addr, e->value);
    }

    // JUMP au bootstrap
    istream_printf("CALL @%d\n", labels->labels[ltable_get_main(labels)]);
    istream_printf("EXIT\n");

    
}
