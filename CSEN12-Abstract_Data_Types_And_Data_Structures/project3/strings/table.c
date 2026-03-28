/* 
 * Dylan Thornburg
 * 1-27-2023
 * Hash Table of Strings 
 * Table.c
 * Coen 12 Lab 3
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include <stdbool.h>
unsigned strhash(char *s);
typedef struct set
{
    	char **elts;
    	int length;
    	int count;
	char *flags;
}SET;
//Function creates the set. O(n) 
SET *createSet(int maxElts)
{
    	int i;
	SET * sp;
    	sp = malloc(sizeof(SET));
    	assert(sp!=NULL);
    	sp->elts =malloc(sizeof(char*) * maxElts);
    	assert(sp->elts!=NULL);
    	sp->length = maxElts;
    	sp->count = 0;
	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags!=NULL);
	for(i = 0; i < sp->length; i++)
	{
		sp->flags[i]= 'e';	
	}
    	return sp;
}
//search function to find an element.O(n)
static int search(SET *sp, char *elt, bool *found)
{
	assert(sp!=NULL);
	assert(elt!=NULL);
	int i;
	int x = 0;
	int start = -1;
	unsigned key = strhash(elt);
	for(i = 0; i < sp->length; i++)
	{
		x = (key + i) % (sp->length);
		if(sp->flags[x] == 'f')
		{
			if(strcmp(sp->elts[x], elt) == 0)
               		{
                        	*found = true;
				return x;
                	}
		}
		if (sp->flags[x] == 'd')
		{
			start = x;
		}
		if(sp->flags[x] == 'e')
		{
			if(start > -1)
			{
				return start;
			}
			return x;
		}
	}
	return start;		
}
//function to add an element.O(1) to add but to search it's O(n). So it's O(n)
void addElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL); 
	if (sp->count == sp->length)
	{
		return;
	}
	bool found = false;
	int x = search(sp, elt, &found);
	if (x == -1 || found == true)
	{
		return;
	}
 	sp->elts[x] = strdup(elt);
	sp->flags[x] = 'f';
        sp->count++;
}
//destroys set. O(n)
void destroySet(SET *sp)
{
	int i = 0;
	while(i < sp->count)
	{
		if(sp->flags[i] == 'f')
		{
			free(sp->elts[i]);	
		}
		i++;
	}
	free(sp->elts);
	free(sp->flags); 
	free(sp);		
}
//lists num of elements. O(1)
int numElements(SET *sp)
{
	return sp->count;
}
//removes an element. O(n)
void removeElement(SET *sp, char *elt)
{
	bool found = false;
	int x = search(sp, elt, &found); 
	assert (sp != NULL && elt != NULL);
	if (found == true)
	{
		free(sp->elts[x]);
		sp->flags[x] = 'd';
		sp->count--;
	}	
}
//finds desired element. O(n)
char *findElement(SET *sp, char *elt)
{
	assert(sp!=NULL && elt!=NULL);
	bool found = false;
	int x = search(sp, elt, &found);
	if (found == true)
	{
		return sp->elts[x];
	}
	return NULL;
}
//copy's all elts to different char **. O(n)
char **getElements(SET *sp)
{
	assert(sp!=NULL);
	char **p = malloc(sizeof(char *)* sp->count);
	assert(p!=NULL);
	int i;
	int x = 0;
	for (i = 0; i < sp->length; i++)
	{
		if(sp->flags[i] == 'f')
		{
			p[x] = sp->elts[i];
			x++;
		}
	}
	return p;	
}
//Hashes char. O(1)
unsigned strhash(char *s)
{
	unsigned hash = 0;
	while(*s != '\0')
	{
		hash = 31 * hash + *s ++;
	}
	return hash;
}







