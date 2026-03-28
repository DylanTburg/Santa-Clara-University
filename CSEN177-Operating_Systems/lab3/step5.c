/*
 * # Name: Dylan Thornburg
 * # Date: 10-8-2024
 * # Title: Lab3 - step5
 * # Description: Implementation of consumer-producer model using pipes
 */


/***************************
* Lab3 - pipe()
**************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
// main
int main(int argc,char *argv[])
{
	int size = 60;
	int fds[2];
	char buff[size];
	int count;
	int i;
	pipe(fds);
	if (fork()==0)
	{
		close(fds[0]);
		count = 0;
		while(1)
		{
			snprintf(buff, size, "Message %d", count++);
			write(fds[1],buff,strlen(buff)+1);
			//printf("\nWriter on the upstream end of the pipe -> %d arguments \nProduced: %s\n",argc, buff);
			printf("Produced: %s\n", buff);
			sleep(5);
		}
		close(fds[1]);
		exit(0);
	}
	else if(fork()==0)
	{
		close(fds[1]);
		while(1)
		{
			ssize_t bytes = read(fds[0], buff, size);
			if(bytes > 0)
			{
				//printf("\nReader on the downstream end of the pipe \nConsumed: %s\n", buff);
				printf("Consumed: %s\n", buff);
			}
			else
			{
				break;
			}
			sleep(5);
		}
		close(fds[0]);
		exit(0);
	}
	else
	{
		close(fds[0]);
		close(fds[1]);
		wait(0);
		wait(0);
	}
	return 0;
}
