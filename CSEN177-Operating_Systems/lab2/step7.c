// Name: Dylan Thornburg
// Date: 10-1-2024
// Title: Lab2 - step7
// Description: This code starts with a parent spawning in a child and then using the child to run the ls command.
/*Sample C program for Lab assignment 1*/
#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */
#include <sys/wait.h> /* wait */
/* main function with command-line arguments to pass */
int main(int argc, char *argv[]) 
{
    	pid_t  pid; 
       	pid = fork();
	if(pid==0)
	{
		execlp("/bin/ls", "ls", NULL);
	}
	else
	{
		wait(0);
		printf("Child Complete\n");
		exit(0);
	}
    	return 0;
}
