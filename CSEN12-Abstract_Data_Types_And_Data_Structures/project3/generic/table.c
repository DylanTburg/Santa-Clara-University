/* 
 * Dylan Thornburg
 * 2-8-2023
 * Generic Data Type Hash 
 * Table.c
 * Coen 12 Lab 3
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
#include <stdbool.h>
typedef struct set
{
    	void **elts;
    	int length;
    	int count;
	char *flags;
	int (*compare)();
	unsigned (*hash)();
}SET;
//Function creates the set. O(n) 
SET *createSet(int maxElts, int (*compare)(), unsigned(*hash)())
{
    	int i;
	SET * sp;
    	sp = malloc(sizeof(SET));
    	assert(sp!=NULL);
    	sp->elts =malloc(sizeof(void*) * maxElts);
    	assert(sp->elts!=NULL);
    	sp->length = maxElts;
    	sp->count = 0;
	sp->flags = malloc(sizeof(char)*maxElts);
	assert(sp->flags!=NULL);
	for(i = 0; i < sp->length; i++)
	{
		sp->flags[i]= 'e';	
	}
	sp->compare = compare;
	sp->hash = hash;
    	return sp;
}
//search function to find an element.O(n)
static int search(SET *sp, void *elt, bool *found)
{
	assert(sp!=NULL);
	assert(elt!=NULL);
	int i;
	int x = 0;
	int start = -1;
	unsigned key = (*sp->hash)(elt);
	for(i = 0; i < sp->length; i++)
	{
		x = (key + i) % (sp->length);
		if(sp->flags[x] == 'f')
		{
			if((*sp->compare) (sp->elts[x], elt) == 0)
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
	return start;//ask ta about this (old was -1)		
}
//function to add an element.O(1) to add but to search it's O(n). So it's O(n)
void addElement(SET *sp, void *elt)
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
 	sp->elts[x] = elt;	
	sp->flags[x] = 'f';	
        sp->count++;
	return;
}
//destroys set. O(1)
void destroySet(SET *sp)
{
	assert(sp!=NULL);
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
void removeElement(SET *sp, void *elt)
{
	bool found = false;
	int x = search(sp, elt, &found); 
	assert (sp != NULL && elt != NULL);
	if (found == true)
	{	
		sp->flags[x] = 'd';
		sp->count--;
	}	
}
//finds desired element. O(n)
void *findElement(SET *sp, void *elt)
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
//copy's all elts to different void **. O(n)
void *getElements(SET *sp)
{
	assert(sp!=NULL);
	void **p = malloc(sizeof(void *)* sp->count);
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
