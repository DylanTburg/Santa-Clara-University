/*
 * Dylan Thornburg
 * Term Project
 * March 18 2023
 * Linked List of nodes with arrays inside. Dynamically allocates arrays for each node.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

typedef struct node{
    	void **data;
    	int count;
    	int length;
    	int first;
    	struct node *next;
    	struct node *prev;
}NODE;

typedef struct list{
	int nodeCount;
    	int itemCount;
    	NODE *head;
    	NODE *tail;
}LIST;

// Creates list. O(1)
LIST *createList(void)
{
    	LIST *lp = malloc(sizeof(LIST));
    	assert(lp != NULL);
    	lp->nodeCount = 0;
    	lp->itemCount = 0;
    	lp->head = NULL;
    	lp->tail = NULL;
    	return lp;
}       

//Destroys list. O(n)
void destroyList(LIST *lp)
{
    	assert(lp != NULL);
    	NODE *currentNode = lp->head;
    	while(currentNode != NULL)
	{
        	NODE *nextNode = currentNode->next;
        	free(currentNode->data);
        	free(currentNode);
        	currentNode = nextNode;
    	}
    	free(lp);
}

//Returns number of items. O(1)
int numItems(LIST *lp)
{
   	assert(lp != NULL);
    	return lp->itemCount;
}                              

//Adds item to the first position. O(1)
void addFirst(LIST *lp, void *item)
{
    	assert(lp != NULL && item != NULL);
    	NODE *newNode = NULL;
    	if(lp->head == NULL || lp->head->count == lp->head->length)
	{
    	    	newNode = malloc(sizeof(NODE));
    	    	assert(newNode != NULL);
    	    	newNode->length = 8;
    	    	newNode->first = 0;
    	    	newNode->count = 0;
    	    	newNode->data = malloc(sizeof(void*) * newNode->length);
    	    	assert(newNode->data != NULL);
    	    	newNode->prev = NULL;
    	    	newNode->next = lp->head;
    	    	if(lp->head != NULL)
		{
    	        	lp->head->prev = newNode;
    	    	}
    	    	lp->head = newNode;
    	    	if(lp->tail == NULL)
		{
    	        	lp->tail = newNode;
    	    	}
    	    	lp->nodeCount++;
    	}
    	int idx = (lp->head->first + lp->head->count) % lp->head->length;
    	lp->head->data[idx] = item;
    	lp->head->count++;
    	lp->itemCount++;
}               

//Adds to the end of list. O(1)
void addLast(LIST *lp, void *item)
{
    	assert(lp != NULL && item != NULL);
    	if(lp->nodeCount == 0 || lp->tail->count == lp->tail->length)
	{
    	    	NODE *temp;
        	temp = malloc(sizeof(NODE));
        	assert(temp != NULL);
        	temp->length = 8;
        	temp->first = 0;
        	temp->data = malloc(sizeof(void*)*temp->length);
        	assert(temp->data != NULL);
        	temp->count = 0;
        	temp->prev = NULL;
        	temp->next = NULL;
        	if(lp->nodeCount == 0)
		{
            		lp->head = temp;
            		lp->tail = temp;
        	}
        	else if(lp->tail->count == lp->tail->length)
		{
            		temp->prev = lp->tail;
            		lp->tail->next = temp;
            		lp->tail = temp;
        	}
        	lp->nodeCount = lp->nodeCount + 1;
    	}
    	int idx = (lp->tail->first + lp->tail->count) % lp->tail->length;
    	lp->tail->data[idx] = item;
    	lp->tail->count = lp->tail->count + 1;
    	lp->itemCount = lp->itemCount + 1;
}

//Removes first item. O(1)
void *removeFirst(LIST *lp)
{
	assert(lp != NULL && lp->itemCount > 0);
	void *dataDel;
	if(lp->head->count == 0)
	{
		NODE *deleted = lp->head;
		lp->head = lp->head->next;
		lp->head->prev = NULL;
		free(deleted->data);
		free(deleted);
		lp->nodeCount = lp->nodeCount - 1;
	}
	dataDel = lp->head->data[lp->head->first];
	lp->head->first = (lp->head->first + 1) % lp->head->length;
	lp->head->count = lp->head->count - 1;
	lp->itemCount = lp->itemCount - 1;
	return dataDel;
}

//Removes last item. O(1)
void *removeLast(LIST *lp)
{
	assert(lp != NULL && lp->itemCount > 0);
	void *dataDel;
	if(lp->tail->count == 0)
	{
		NODE *deleted = lp->tail;
		lp->tail = deleted->prev;
		lp->tail->next = NULL;
		lp->nodeCount = lp->nodeCount - 1;
		free(deleted->data);
		free(deleted);
	}
	else
	{
		dataDel = lp->tail->data[(lp->tail->first + lp->tail->count - 1) % lp->tail->length];
		lp->tail->count = lp->tail->count - 1;
		lp->itemCount = lp->itemCount - 1;
		if(lp->tail->count == 0)
		{
			NODE *deleted = lp->tail;
			lp->tail = deleted->prev;
			lp->tail->next = NULL;
			lp->nodeCount = lp->nodeCount - 1;
			free(deleted->data);
			free(deleted);
		}
	}
	return dataDel;
}

//returns an item from an index. O(n) where n is number of nodes
void *getItem(LIST *lp, int index)
{
	assert(lp != NULL && index >= 0 && index < lp->itemCount);
	NODE *temp = lp->head;
	int i;
	int j = 0;
	for(i=0;i < lp->itemCount; i+=temp->count)
	{
		if(index < temp->count)
		{
			j = index;
			break;
		}
		else
		{
			index -= temp->count;
		}
		temp = temp->next;
	}
	int num = (temp->first + j)% temp->length;
	return temp->data[num];	
}

//Sets a value to an index. O(n) where n is number of nodes
void setItem(LIST *lp, int index, void *item)
{
	assert(lp != NULL && item != NULL);
	int count = index;
	NODE *temp = lp->head;
	while(temp != NULL)
	{
		if((count - temp->count) < 0)
		{
			temp->data[(temp->first + count) % temp->length] = item;
			return;
		}
		count = count - temp->count;
		temp = temp->next;
	}
}






