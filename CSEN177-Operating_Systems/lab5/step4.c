/*
 * Name: Dylan Thornburg
 * Date: 10-22-2024
 * Title: lab5 - step4
 * Description: Classic Consumer & Producer Problem Using Mutex
 */

// Thread Sychronization
//To compile this code:
//If using the SCU Linux lab first add #include<fcntl.h>
//Then you're ready to compile.
//To compile just add -lpthread to your gcc command like:
//gcc threadSync.c -lpthread
//gcc threadSync.c -o test -lpthread
//Shared data: semaphores called "full", "empty", and "lock"
//Buffer to hold n items
//"lock" provides mutual exclusion to the buffer pool
//"empty" and "full" count the number of empty and full slots in the buffer
//Initially: full = 0, empty = n, mutex = 1

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> 
#include <semaphore.h>
#include <stdlib.h>

#define NTHREADS 10
#define n 10
pthread_t threads[NTHREADS];
pthread_mutex_t lock;
pthread_cond_t full, empty;
int in, out;
int queue[n];
int isFull = 0;

void* producer(void* arg)
{
	int num;
	do 
	{                             
   		num = rand()%100;      
    		printf("Thread %d produced %d\n", (intptr_t)arg, num);
		sleep(1);
    		pthread_mutex_lock(&lock); //crit section to stop producing if full
		while(isFull == n)
		{
			pthread_cond_wait(&empty, &lock);
		}
    		queue[in] = num;               
    		in = (in+1)%n;
		isFull++;
		pthread_cond_signal(&full);		
    		pthread_mutex_unlock(&lock); //end crit section
		sleep(1);
	} while(1); 
}

// Runs consumer infinite while loop
void* consumer(void* arg) 
{
    	int num;
	do 
	{                             
		pthread_mutex_lock(&lock);
		while(isFull == 0)
		{
			pthread_cond_wait(&full, &lock);
		}	
		num = queue[out];            
    		out = (out+1)%n;               
    		isFull--;
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&lock);
    		printf("Thread %d is consuming %d\n", (intptr_t)arg, num); 
   		sleep(1);	
	} while(1); 
}

int main() 
{ 
	in = 0;
	out = 0;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&empty, NULL);
	pthread_cond_init(&full, NULL);
	static int i;
	for (i = 0; i < NTHREADS/2; i++) //0-4  produce
		pthread_create(&threads[i], NULL, producer, (void *)(intptr_t)i);
	for(i=NTHREADS/2; i < NTHREADS; i++)//5-9 consume
		pthread_create(&threads[i], NULL, consumer, (void *)(intptr_t)i);
	for (i = 0; i < NTHREADS; i++) 
	{
		pthread_join(threads[i],NULL);
	}
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&empty);
	pthread_cond_destroy(&full);

	return 0; 
} 
