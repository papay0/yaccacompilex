#ifndef IDBUFFER_H
#define IDBUFFER_H
#define STACKBUFF_MAX 128
typedef struct stackbuff_entry
{
	int size;
	void* buff[STACKBUFF_MAX];
} stackbuff_entry_t;

typedef struct stackbuff
{
	int depth;
	stackbuff_entry_t entries[STACKBUFF_MAX];
} stackbuff_t;

stackbuff_t* stackbuff_new();
int stackbuff_size(stackbuff_t* buff);
void stackbuff_push(stackbuff_t* buff);
void stackbuff_add(stackbuff_t* buf, void* ptr);
void* stackbuff_get(stackbuff_t* buff, int i);
void stackbuff_pop(stackbuff_t* buff);
#endif
