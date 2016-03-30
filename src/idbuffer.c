#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "idbuffer.h"

/* ----------------------------------------------------------------------------
 * STACKBUFFER
 * --------------------------------------------------------------------------*/
extern int yylineno;
stackbuff_t* stackbuff_new()
{
	stackbuff_t* buff = (stackbuff_t*)malloc(sizeof(stackbuff_t));
	buff->depth = -1;
	return buff;
}

int stackbuff_size(stackbuff_t* buff)
{
	return buff->entries[buff->depth].size;
}

void stackbuff_push(stackbuff_t* buff)
{
	buff->depth++;
	buff->entries[buff->depth].size = 0;
	// print_warning("line %d: [DEBUG] PUSH\n", yylineno);
}

void stackbuff_add(stackbuff_t* buff, void* ptr)
{
	// print_warning("line %d: [DEBUG] buff=%p size=%d, ptr=%d\n", yylineno, buff, buff->depth, ptr);
	buff->entries[buff->depth].buff[buff->entries[buff->depth].size++] = ptr;
}

void stackbuff_addstr(stackbuff_t* buff, char* ptr)
{
    char* nbuff = (char*)malloc(strlen(ptr)+1);
    strcpy(nbuff, ptr);
	// print_warning("[DEBUG] buff=%p size=%d, ptr=%d\n", buff, buff->depth, ptr);
	buff->entries[buff->depth].buff[buff->entries[buff->depth].size++] = nbuff;
}

void* stackbuff_get(stackbuff_t* buff, int i)
{
	assert(i < stackbuff_size(buff));
	return buff->entries[buff->depth].buff[i];
}

void stackbuff_pop(stackbuff_t* buff)
{
	// print_warning("line %d: [DEBUG] POP\n", yylineno);
	assert(buff->depth >= 0);
	buff->depth--;
}
