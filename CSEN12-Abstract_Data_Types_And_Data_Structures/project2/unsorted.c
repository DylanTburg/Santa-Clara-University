/* 
 * Dylan Thornburg
 * 1-20-2023
 * Unsorted array/set 
 * unsorted.c
 * Coen 12 Lab 2
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "set.h"
typedef struct set
{
    	char **elts;
    	int length;
    	int count;
}SET;
//Function creates the set. O(1) (maybe O(n) if you consider making each element within the array as 1)
SET* createSet(int maxElts)
{
    	SET * sp;
    	sp = malloc(sizeof(SET));
    	assert(sp!=NULL);
    	sp->elts = malloc(sizeof(char*) * maxElts);
    	assert(sp->elts);
    	sp->length = maxElts;
    	sp->count = 0;
    	return sp;
}
//search function to find an element.O(n)
static int search(SET *set, char *elt)
{
	int i = 0;
	while (set->elts[i])
	{
		if (strcmp(set->elts[i], elt) == 0)
		{
			return i;
		}
		i++;
	}
	return -1;		
}
//function to add an element.O(1) to add but to search it's O(n). So it's O(n)
void addElement(SET *sp, char *elt)
{	
	assert(sp->count < sp->length);
	if (search(sp, elt) >= 0)
	{
		return;
	}
 	sp->elts[sp->count] = strdup(elt);
        sp->count++;
}
//destroys set. O(n)
void destroySet(SET *sp)
{
	int i = 0;
	while(sp->elts[i] && i < sp->count)
	{
		free(sp->elts[i]);
		i++;
	}
	free(sp->elts); 
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
	int x = search(sp, elt); 
	assert (sp != NULL && elt != NULL);
	if (x != -1)
	{
		free(sp->elts[x]);
		sp->elts[x]=sp->elts[--sp->count];
	}	
}
//finds desired element. O(n)
char *findElement(SET *sp, char *elt)
{
	if (search(sp, elt) >= 0)
	{
		return sp->elts[search(sp, elt)];
	}
	return NULL;
}
//copy's all elts to different char **. O(n)
char **getElements(SET *sp)
{
	char **p;
	p = malloc(sizeof(char*) * sp->count);
	memcpy(p, sp->elts,sizeof(char*) * sp->count);
	return p;	
}







