#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_BUFF 128
// Ce buffer contient la liste des identifiers concernés par une déclaration.
// Ex : int a, b, c, d, e; => idbuffer = ["a", "b", "c", "d", "e"]

int buffsize;
void* idbuffer[MAX_BUFF];
int bufftype;

void idbuffer_settype(int type)
{
	bufftype = type;	
}


// Initialise le idbuffer
void idbuffer_init()
{
	buffsize = 0;	
	for(int i = 0; i < buffsize; i++)
	{
		free(idbuffer[i]);
	}
}

// Ajoute un identifier au buffer
void idbuffer_addstr(char* identifier)
{
	assert(buffsize < MAX_BUFF);
	char* nbuff = (char*)malloc(strlen(identifier)+1);
	strcpy(nbuff, identifier);
	idbuffer[buffsize++] = nbuff;		
}

void idbuffer_add(void* ptr)
{
	assert(buffsize < MAX_BUFF);
	idbuffer[buffsize++] = ptr;	
}

void* idbuffer_get(int i)
{
	assert(i < buffsize);
	return idbuffer[i];
}

int idbuffer_size()
{
	return buffsize;
}
/* ----------------------------------------------------------------------------
 * PARAMBUFFER 
 * --------------------------------------------------------------------------*/
typedef struct pbuffer_entry
{
	int size;
	void* buff[MAX_BUFF];
} pbuffer_entry_t;

pbuffer_entry_t pbuffer_entries[MAX_BUFF];
int pbuffer_depth = -1;

int parambuffer_size()
{
	return pbuffer_entries[pbuffer_depth].size;
}

void parambuffer_push()
{
	pbuffer_depth++;
	pbuffer_entries[pbuffer_depth].size = 0;
	printf("depth = %d\n", pbuffer_depth);
}

void parambuffer_add(void* ptr)
{
	printf("added param, current size = %d, ptr=%p\n", parambuffer_size(), ptr);
	pbuffer_entries[pbuffer_depth].buff[pbuffer_entries[pbuffer_depth].size++] = ptr;
}

void* parambuffer_get(int i)
{
	assert(i < parambuffer_size());
	printf("retrieve depth= %d\n", pbuffer_depth);
	return pbuffer_entries[pbuffer_depth].buff[i];
}

void parambuffer_pop()
{
	assert(pbuffer_depth >= 0);
	for(int i = 0; i < parambuffer_size(); i++)
		free(parambuffer_get(i));
	pbuffer_depth--;
}
