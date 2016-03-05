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
void idbuffer_addstr(char* const identifier)
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


