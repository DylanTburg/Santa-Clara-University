/*
 * Name: Dylan Thornburg
 * Date: 10-22-2024
 * Title: lab5 - step3
 * Description: Classic Consumer & Producer Problem Using Semaphores
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
sem_t *mutex, *full, *empty; 
int in, out;
int queue[n];

void* producer(void* arg)
{
	int num;
	do 
	{                             
   		num = rand()%100;      
    		printf("Thread %d produced %d\n", (intptr_t)arg, num);
		sleep(1);
    		sem_wait(empty);                // empties buffer if full
    		sem_wait(mutex);                // critical section
    		queue[in] = num;               
    		in = (in+1)%n;                  
    		sem_post(mutex);                // critical section end
    		sem_post(full);
		sleep(1);
	} while(1); 
}

// Runs consumer infinite while loop
void* consumer(void* arg) 
{
    	int num;
	do 
	{                             
    		sem_wait(full);                 // wait until buffer is full
    		sem_wait(mutex);                // critical section
    		num = queue[out];            
    		out = (out+1)%n;               
    		sem_post(mutex);                // critical section end
    		sem_post(empty);                // wait till buffer is empty
    		printf("Thread %d is consuming %d\n", (intptr_t)arg, num); 
   		sleep(1);	
	} while(1); 
}

int main() 
{ 
	in = 0;
	out = 0;
	sem_unlink("mutex");
	sem_unlink("empty");
	sem_unlink("full");
	mutex = sem_open("mutex", O_CREAT, 0644, 1);
	empty = sem_open("empty", O_CREAT, 0644, n);
	full = sem_open("full", O_CREAT, 0644, 0);
	static int i;
	for (i = 0; i < NTHREADS/2; i++) //0-4  produce
		pthread_create(&threads[i], NULL, producer, (void *)(intptr_t)i);
	for(i=NTHREADS/2; i < NTHREADS; i++)//5-9 consume
		pthread_create(&threads[i], NULL, consumer, (void *)(intptr_t)i);
	for (i = 0; i < NTHREADS; i++) 
	{
		pthread_join(threads[i],NULL);
	}
	return 0; 
} 
