//Name: Dylan Thornburg
//Date: 11/19/2024
//Title: Lab9 - Step4
//Description: Code to read the created bin files from beginning to end with different sized buffers
//and have a write operation occur for every read (copy the whole file as we read)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) 
{
	int size = atoi(argv[2]);
	char buffer[size];
	FILE *fp;
	fp=fopen(argv[1], "rb");
	FILE *copy;
	copy = fopen("copy.bin", "wb");
	while(fread(buffer, sizeof(buffer), 1, fp))
	{
		fwrite(buffer, sizeof(buffer), 1, copy);
	}
	fclose(fp);
	fclose(copy);
	return 0;
}
