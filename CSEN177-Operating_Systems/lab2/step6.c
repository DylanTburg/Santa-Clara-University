// Name: Dylan Thornburg
// Date: 10-1-2024
// Title: Lab2 - step6
// Description: This is code takes the code we were given but uses two threads instead of two processes
/*Sample C program for Lab assignment 1*/
#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */
#include <pthread.h>	/* threads */
/* main function with command-line arguments to pass */
int n;
void* threadFunc(void * arg)
{
    int x = (int)(long)arg; //0 or 1 depending on which thread
    int i=0;
    for(;i<100;i++)
    {
    	if(x==0)
        {
            printf("\t \t \t Parent Thread %d \n",i);
	}
	else
	{
	    printf("Child Thread %d\n",i);
	}
	usleep(n);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) //check to make sure two parameters entered
    {
        fprintf(stderr, "Usage: ./program [microseconds]\n");
        return 1;
    }
    pthread_t parent, child;
    n = atoi(argv[1]); // n microseconds to input from keyboard for delay
    printf("\nBefore Threading.\n");
    if(pthread_create(&parent, NULL, threadFunc, (void*)(long)0)!=0)
    {
        fprintf(stderr, "Error creating parent thread\n");
	return 1;
    }
    if(pthread_create(&child, NULL, threadFunc, (void*)(long)1)!=0)
    {
        fprintf(stderr, "Error creating parent thread\n");
	return 1;
    }
    pthread_join(parent, NULL);
    pthread_join(child, NULL);

    return 0;
}
