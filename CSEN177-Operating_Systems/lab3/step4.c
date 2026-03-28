/*
 * # Name: Dylan Thornburg
 * # Date: 10-8-2024
 * # Title: Lab3 - step4
 * # Description: C program that uses the same system calls to implement cat /etc/passwd | grep root 
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
	int fds[2];
	char buff[60]; //increase if you dont want a random word cutoff
	int count;
	int i;
	pipe(fds);
	if (fork()==0)
	{
		dup2(fds[1], 1);
		close(fds[0]);
		execlp("/bin/cat", "cat", "/etc/passwd", 0);
		exit(0);
	}
	else if(fork()==0)
	{
		dup2(fds[0], 0);
		close(fds[1]);
		execlp("/bin/grep", "grep", "root", 0);
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
