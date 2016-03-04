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

// Addresses temporaires
#define TEMPADDR_COUNT 2
#define TEMPADDR_BASE 255
typedef struct tempaddr {
	int address;
	int locked;
} tempaddr_t;

tempaddr_t tempaddr[TEMPADDR_COUNT];

int tempaddr_init()
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		tempaddr[i].locked = 0;
		tempaddr[i].address = TEMPADDR_BASE - i;				
	}
}
int tempaddr_lock()
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		if(!tempaddr[i].locked) {
			tempaddr[i].locked = 1;
			return tempaddr[i].address;
		}
	}
	printf("lock_tempaddr: not enough addresses\n");
	return -1;
}
void tempaddr_unlock(int addr)
{
	for(int i = 0; i < TEMPADDR_COUNT; i++)
	{
		if(tempaddr[i].address == addr) {
			tempaddr[i].locked = 0;
			return;
		}
	}
	printf("unlock_tempaddr: no var to unlock");

}



