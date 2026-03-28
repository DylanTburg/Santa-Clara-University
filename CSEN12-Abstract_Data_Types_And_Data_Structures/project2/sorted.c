/*
 * Dylan Thornburg
 * 1-27-2023
 * Sorted array/set within a set
 * Sorted.c  
 * Coen 12 Lab 2  
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
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
        assert(sp);
        sp->elts = malloc(sizeof(char*) * maxElts);
        assert(sp->elts);
        sp->length = maxElts;
        sp->count = 0;
        return sp;
}
//search function to find an element.O(log(n))
static int search(SET *set, char *elt, bool *found)
{
	assert (set != NULL);
	int end = set->count-1;
	int beg = 0;
	int i;
       	while(beg <= end)
	{		
		i = ((beg + end)/2);
	
		if (strcmp(set->elts[i], elt) < 0) //left
		{
			end = i-1;	
		}
		if(strcmp(set->elts[i], elt) > 0) //right
		{
			beg = i+1;
		}
		if (strcmp(set->elts[i], elt) == 0)
		{
                	*found = true;
                	return i;
		}	
	}
	*found = false;
        return beg;
}
//function to add an element. O(nlog(n))
void addElement(SET *sp, char *elt)
{	
	assert (sp != NULL);
	bool found = true;
	int j;
	int i = search(sp, elt, &found);	
        if (found)
        {
                return;
        }
	if(sp->count == sp->length)
	{
		printf("Set is full \n");
	}
	for (j = sp->count; j > i; j--)
        {
               sp->elts[j]= sp->elts[j-1];
        }
	sp->elts[i] = strdup(elt);
	sp->count+=1;
	return; 
}
//destroys set. O(n)
void destroySet(SET *sp)
{
        int i = 0;
        for(i; i < sp->count; i++)
        {
                free(sp->elts[i]);
        }
        free(sp->elts);
        free(sp);
}
//lists num of elements. O(1)
int numElements(SET *sp)
{
	assert(sp);       
	return sp->count;
}
//removes an element. O(nlog(n))
void removeElement(SET *sp, char *elt) 
{
	bool found=false;
        int x = search(sp, elt, &found);
        assert (sp != NULL && elt != NULL);
        if (found)
        {
                free(sp->elts[x]);
		int i;
		for (i = x + 1; i < sp->count; i++)
		{
                	sp->elts[i-1]=sp->elts[i];
		}
		sp->count--;
        }
	return;
}
//finds desired element. O(log(n))
char *findElement(SET *sp, char *elt)
{
	assert (sp != NULL);
	bool found=true;
	int t = search(sp, elt, &found);
        if (found)
        {
                return sp->elts[t];
        }
        return NULL;
}
//copy's all elts to different char **. O(n)
char **getElements(SET *sp)
{
	assert(sp != NULL);
	int i;
        char **p = malloc(sizeof(char *) * sp->count);
	assert (p != NULL);
	memcpy(p, sp->elts, sizeof(char*) * sp->count);
        return p;
}


