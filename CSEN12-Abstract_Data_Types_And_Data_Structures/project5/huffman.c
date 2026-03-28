/*
 * Dylan Thornburg
 * Coen 12
 * huffman.c
 * Program for huffman encoding and decoding
 * 3-10-2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

//Creates root. O(1)
static NODE *mkNode(int count, NODE *left, NODE *right)
{
	NODE *root = malloc(sizeof(NODE));
	assert(root != NULL);
	root->count = count;
	root->parent = NULL;
	if (left)
	{
		left->parent = root;
	}
	if (right)
	{
		right->parent = root;
	}
	return root;
}

//compares leaves. O(1) 
static int nodeCmp(NODE *n1, NODE *n2)
{
	assert(n1 != NULL && n2 != NULL);
	if (n1->count <n2->count)
		return -1;
	return 0;
}

//finds depth. O(h)
static int depth(NODE *n)
{
	if(n->parent == NULL)
		return 0;
	else 
		return 1+depth(n->parent);
}

int main(int argc, char *argv[])
{
	int a[257];
	NODE *leaves[257];
	int i;
	int j;
	int k;
	int l;
	int m;
	int count;
	for (i = 0; i < 257; i++)
	{
		a[i]=0;
		leaves[i]=NULL;	
	}
	if(argc != 3)
	{
		printf("error \n");
		return 0;
	}
	FILE *fp = fopen(argv[1], "rb");
	assert(fp !=NULL);
	while((j= getc(fp)) != EOF) 
	{
		a[j]++;
	}
	fclose(fp);
	PQ *pq = createQueue(*nodeCmp);
	for (k=0; k < 257; k++)
	{
		if(a[k] > 0)
		{
			leaves[k] = mkNode(a[k], NULL, NULL);
			addEntry(pq, leaves[k]);
		}
	}
	NODE *end = malloc(sizeof(NODE));
	assert (end!=NULL);
	end->count = 0;
	end->parent = NULL;
	leaves[256] = end;
	addEntry(pq, leaves[256]);
	while(numEntries(pq) > 1)
	{
		NODE *l = removeEntry(pq);
		NODE *r = removeEntry(pq);
		NODE *p = mkNode(l->count + r->count, l, r);
		addEntry(pq, p);
	}
	for(l = 0; l < 257; l++)
	{
		if(leaves[l] != NULL)
		{
			m = depth(leaves[l]);
			count = m*leaves[l]->count;
			int p = isprint(1);
			if (p) 
			{
				printf("%c", 1);
			} 
			else 
			{
				printf("%03o", 1);
			}
			printf("%d multiplied by %d bits equals %d bits total. \n", a[l], m, count);
		}
	}
	pack(argv[1], argv[2], leaves);
	return 0;
}




