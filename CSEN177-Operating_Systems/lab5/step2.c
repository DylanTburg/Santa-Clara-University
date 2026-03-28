/*
 * Name: Dylan Thornburg
 * Date: 10-22-2024
 * Title: lab5 - step2
 * Description: Changing the given threadsync.c code to use mutex locks instead of semaphore
 */

// Thread Sychronization
//To compile this code:
//If using the SCU Linux lab first add #include<fcntl.h>
//Then you're ready to compile.
//To compile just add -lpthread to your gcc command like:
//gcc threadSync.c -lpthread
//gcc threadSync.c -o test -lpthread

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h> 

#define NTHREADS 10
pthread_t threads[NTHREADS];
pthread_mutex_t lock; 

void* go(void* arg) 
{ 
	pthread_mutex_lock(&lock); //entry section
  	printf("Thread %d Entered Critical Section..\n", (intptr_t)arg); //critical section 
   	sleep(1); 
  	pthread_mutex_unlock(&lock); //exit section 
  	return (NULL);
} 

int main() 
{ 
	if(pthread_mutex_init(&lock, NULL) !=0)
	{
		printf("\n failed to create mutex \n");
		return 1;	
	}
	static int i;
	for (i = 0; i < NTHREADS; i++)  
	   	pthread_create(&threads[i], NULL, go, (void *)(intptr_t)i);
	for (i = 0; i < NTHREADS; i++) 
	{
		pthread_join(threads[i],NULL);
		printf("\t\t\tThread %d returned \n", i);
	}
	printf("Main thread done.\n");
	pthread_mutex_destroy(&lock);
	return 0; 
} 
