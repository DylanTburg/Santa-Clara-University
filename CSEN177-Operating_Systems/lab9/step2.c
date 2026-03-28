//Name: Dylan Thornburg
//Date: 11/19/2024
//Title: Lab9 - Step2
//Description: Code to read the created bin files from beginning to end

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	char buffer[10000];
	FILE *fp;
	fp=fopen(argv[1], "rb");
	while(fread(buffer, sizeof(buffer), 1, fp))
	{
		
	}
	fclose(fp);
	return 0;
}
