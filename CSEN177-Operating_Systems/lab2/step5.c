// Name: Dylan Thornburg
// Date: 10-1-2024
// Title: Lab2 - step5
// Description: This code creates 7 processes and has each process print their own PID, their parent's PID (if they have one) 
// and their child's PID (if they have one). It builds off of code from steps 1-4

/* Diagram:
 *              parent
 *     two                 three
 *four     five        six       seven
 */
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
    	pid_t  two,three,four,five,six,seven; //all different processes
    	//first child (second process)
    	two = fork();
	if(two==0) //if two is child 
	{		
		four = fork();
    		if (four == 0) 
    		{
			wait(0);
			printf("I am four. My PID is %d\nMy parent's PID is %d and I have no children.\n\n", getpid(), getppid());
			exit(0);
        	}
		five = fork();
		if(five==0)
		{
			wait(0);
			printf("I am five. My PID is %d\nMy parent's PID is %d and I have no children.\n\n", getpid(), getppid());
			exit(0);
		}
		wait(0);
		printf("I am two. My PID is %d\nMy parent's PID is %d and my children are %d and %d.\n\n", getpid(), getppid(), four, five);
		exit(0);
	}
	else
	{
		three = fork();
		if(three==0)
		{	
			six=fork();
			if(six==0) 
			{
				wait(0);
				printf("I am six. My PID is %d\nMy parent's PID is %d and I have no children.\n\n", getpid(), getppid());
				exit(0);
			}
			seven = fork();
			if(seven==0)
			{
				wait(0);
				printf("I am seven. My PID is %d\nMy parent's PID is %d and I have no children.\n\n", getpid(), getppid());
				exit(0);
			}
			wait(0);
			printf("I am three. My PID is %d\nMy parent's PID is %d and my children are %d and %d.\n\n", getpid(), getppid(), six, seven);
			exit(0);
		}
		else
		{
			wait(0);
			printf("I am top parent so my parent is the terminal which is %d\nMy PID is %d and my children are %d and %d\n\n",getppid(), getpid(), two, three);	
		}
	}
    	return 0;
}
