/*
 * Dylan Thornburg
 * Coen 12 
 * set.c
 * Chaining in a hash table  
 * 02-24-23
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"
#include "set.h"
typedef struct node
{
	void *data;
	struct node *next;
	struct node *prev;
}NODE;

typedef struct list
{
	int count;
	struct node *head;
	int (*compare)();
}LIST;

typedef struct set
{
	int count;
	int length;
	int (*compare)();
	unsigned (*hash)();
	LIST **lists;
}SET;

//creates set of lists. O(n)
SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())
{
	int a = 20;
	int i;
	SET *sp = malloc(sizeof(SET));
	assert(compare != NULL && hash != NULL);
	assert(sp != NULL);
	sp->count = 0;
	sp->compare = compare;
	sp->hash = hash;
	sp->length = maxElts/a;
	sp->lists = malloc(sizeof(LIST*)*sp->length);
	for(i=0; i <sp->length; i++)
	{
		sp->lists[i] = createList(compare);
	}
	return sp;	
}

//destroys list. O(1)	
void destroySet(SET *sp)
{
	assert(sp != NULL);
	free(sp->lists);
	free(sp);
}

//lists number of elements. O(1)
int numElements(SET *sp)
{
	assert(sp != NULL);
	return(sp->count);
}

//Adds elt to list. O(n1)
void addElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	int i = (*sp->hash)(elt) % sp->length;
	if(findItem(sp->lists[i], elt) == NULL)
	{
		addFirst(sp->lists[i], elt);
		sp->count++;
	}
}


//Removes elt from list. O(n)
void removeElement(SET *sp, void * elt)
{
	assert(sp != NULL && elt != NULL);
	int i = (*sp->hash)(elt) % sp->length;
	if(findItem(sp->lists[i], elt))
	{
		removeItem(sp->lists[i], elt);
		sp->count--;
	} 
}

//finds elt and returns it. Wish I made this before the remove and add element functions. O(n)
void *findElement(SET *sp, void *elt)
{
	assert(sp != NULL && elt != NULL);
	int i = (*sp->hash)(elt) % sp->length;
	return findItem(sp->lists[i], elt);
}

//makes a copy of list. O(n)
void *getElements(SET *sp)
{
	assert (sp != NULL);
	int i;
	int j=0;
	void **copy = malloc(sizeof(void*) * sp->count);
	assert(copy != NULL);
	for(i = 0; i < sp->length; i++)
	{		
		assert(copy != NULL);
		int x = numItems(sp->lists[i]);	
		if (x > 0)
		{
			memcpy(copy + j, getItems(sp->lists[i]), sizeof(void *) * numItems(sp->lists[i]));
			j += numItems(sp->lists[i]);
		}
	}
	return copy;
}






