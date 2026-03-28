/*
 * Dylan Thornburg
 * Coen 12 
 * List.c
 * Chaining in a hash table 
 * 02-17-2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"
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

//creates list. O(1)
LIST *createList(int (*compare)())
{
	LIST *lp = malloc(sizeof(LIST));
	assert (lp != NULL);
	lp->count = 0;
	lp->compare = compare;
	lp->head = malloc(sizeof(NODE));
	assert(lp->head != NULL);
	lp->head->next = lp->head;
	lp->head->prev = lp->head;
	return lp;	
}

//destroys list. O(n)	
void destroyList(LIST *lp)
{
	assert(lp != NULL);
	NODE *del;
	NODE *next;
	del = lp->head->next;
	while (del != lp->head)
	{
		free(del);
		next = del->next;
		del = next;
	}
	free(lp);
}

//lists number of items. O(1)
int numItems(LIST *lp)
{
	assert(lp != NULL);
	return(lp->count);
}

//Adds node to front of list. O(1)
void addFirst(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	NODE *temp = malloc(sizeof(NODE));
	temp->data = item;
	temp->prev = lp->head;
	temp->next = lp->head->next;
	lp->head->next = temp;
	temp->next->prev = temp;
	lp->count++;
}

//Adds node to end of list. O(1)
void addLast(LIST *lp, void* item)
{
	assert(lp != NULL && item != NULL);
	NODE *temp = malloc(sizeof(NODE));
	temp->data = item;
	temp->next = lp->head;
	temp->prev = lp->head->prev;
	lp->head->prev = temp;
	temp->prev->next = temp;
	lp->count++;
}

//Removes first item. O(1)
void *removeFirst(LIST *lp)
{
	assert(lp != NULL);
	NODE *del = lp->head->next;
	void *first = del->data;
	lp->head->next = del->next;
	del->next->prev = lp->head;
	free(del);
	lp->count--;
	return first;
}

//removes last item. O(1)
void *removeLast(LIST *lp)
{
	assert(lp != NULL);
	NODE *del = lp->head->prev;
	void *last = del->data;
	del->prev->next = lp->head;
	lp->head->prev = del->prev;
	free(del);
	lp->count--;
	return last;
}

//returns first item. O(1)
void *getFirst(LIST *lp)
{
	assert(lp != NULL);
	return (lp->head->next->data);
}

//returns last item. O(1)
void *getLast(LIST *lp)
{
	assert(lp != NULL);
	return(lp->head->prev->data);
}

//removes item from list if present. O(n)
void removeItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
        NODE *p = lp->head->next;
        int i;
        for(i = 0; i <lp->count;i++)
        {
                if(lp->compare(item, p->data) == 0)
                {
                        p->next->prev = p->prev;
			p->prev->next = p->next;
			free(p);
			lp->count--;
                }
                p = p->next;
        }

}

//finds item if in a list. O(n)
void *findItem(LIST *lp, void *item)
{
	assert(lp != NULL && item != NULL);
	NODE *p = lp->head->next;
	int i;
	for(i = 0; i <lp->count;i++)
	{
		if(lp->compare(item, p->data) == 0)
		{
			return (p->data);
		}
		p = p->next;
	}
	return NULL;
}

//copy and return the list. O(n)
void *getItems(LIST *lp)
{
	void **q = malloc(sizeof(void *)*lp->count);
	NODE *p = lp->head->next;
	int i;
	for(i=0; i < lp->count; i++)
	{
		q[i] = p->data;
		p = p->next;
	}
	return(q);
}
