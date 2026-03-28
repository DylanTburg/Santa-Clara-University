/*
 * # Name: Dylan Thornburg
 * # Date: 10-8-2024
 * # Title: Lab3 - step7
 * # Description: Fixing step6's code by mallocing a variable and assigning the variable's pointer to i so each thread has its own i.
 */


/**************************
* COEN177: Threads *
**************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void *go(void *);
#define NTHREADS 10
pthread_t threads[NTHREADS];
int main() 
{
	int i;
	for (i = 0; i < NTHREADS; i++)
	{
		int *mem = malloc(sizeof(int)); //dynamically allocate memory for each i iteration so each thread has a "different" i
		*mem = i;
		pthread_create(&threads[i], NULL, go, mem);
	}
	for (i = 0; i < NTHREADS; i++) 
	{
		printf("Thread %d returned\n", i);
		pthread_join(threads[i],NULL);
	}
	printf("Main thread done.\n");
	return 0;
}
void *go(void *arg) 
{
	printf("Hello from thread %ld with iteration %d\n", (long)pthread_self(), *(int*)arg);
	return 0;	
}
