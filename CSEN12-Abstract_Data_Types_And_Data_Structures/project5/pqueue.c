/*
 * Dylan Thornburg
 * pqueue.c
 * priority queue of numbers
 * is a heap
 * 3-1-2023
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "pqueue.h"

# define LENGTH 10
# define p(x) (((x)-1) / 2)
# define l(x) (((x)*2) + 1)
# define r(x) (((x)*2) + 2)
typedef struct pqueue {
	int count;
	int length;
	void **data;
	int (*compare)();
}PQ;
//Creates the queue. O(1)
PQ *createQueue(int (*compare)())
{
	PQ *pq;
	pq = malloc(sizeof(PQ));
	assert(pq != NULL);
	pq->count = 0;
	pq->length = LENGTH; //at least ten numbers is what was asked
	pq->compare = compare;
	assert (compare != NULL);
	pq->data = malloc(sizeof(void*) * LENGTH);
	assert (pq->data != NULL);
	return pq;
}
//destroys queue. O(1)
void destroyQueue(PQ *pq)
{
	assert (pq != NULL);
	free(pq->data);
	free(pq);
}
//Outputs number of entries. O(1)
int numEntries(PQ *pq)
{
	assert (pq != NULL);
	return pq->count;
}
//adds entry. O(logn)
void addEntry(PQ *pq, void *entry)
{
	assert(pq != NULL && entry != NULL);
	if (pq->length == pq->count)
	{
		pq->data = realloc(pq->data, sizeof(void *) *(pq->length * 2));
		pq->length *= 2;	
	}
	pq->data[pq->count] = entry;
	int x = pq->count; 
	while(pq->compare(pq->data[x], pq->data[p(x)]) < 0)
	{
		void *temp = pq->data[p(x)];
		pq->data[p(x)] = pq->data[x];
		pq->data[x] = temp;
		x = p(x);		
	}
	pq->count++;
	return;
}
//removes smallest entry. O(logn) 
void *removeEntry(PQ *pq)
{
	assert(pq != NULL);
	void *root = pq->data[0];
	int x=0;
	int y=0;
	pq->data[0]= pq->data[pq->count-1];
	pq->count--;
	while(l(x) < pq->count)
	{
		y = l(x);
		if (r(x) < pq->count)
		{
			if(pq->compare(pq->data[l(x)], pq->data[r(x)]) <  0)
			{
				y = l(x);
			}
			else
				y = r(x);
		}
		if(pq->compare(pq->data[y], pq->data[x]) <  0)
		{
			void * temp = pq->data[y];
			pq->data[y] = pq->data[x];
			pq->data[x]=temp;	
			x = y;
		}
		else
			break;		
	}
	return root;
}




