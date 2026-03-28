//Name: Dylan Thornburg
//Date: 11/19/2024
//Title: Lab9 - Step5
//Description: Code to read the created bin files from beginning to end with different sized buffers
//and have a write operation occur for every read (copy the whole file as we read)
//Now we also have multiple threads

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
int size;
char input[10];
void * threadFunc(void*arg)
{
	char buffer[size];
	FILE *fp;
	fp=fopen(input, "rb");
	FILE *copy;
	char copies[10];
	sprintf(copies, "copy%d.bin", (int)(size_t)arg); 
	copy = fopen(copies, "wb");
	//threads will all try to write to the same file. this is ok
	//because we are just examining time to read and write and not 
	//the data itself. 
	while(fread(buffer, sizeof(buffer), 1, fp))
	{
		fwrite(buffer, sizeof(buffer), 1, copy);
	}
	fclose(fp);
	fclose(copy);
	return NULL;

}
int main(int argc, char *argv[]) 
{
	size = atoi(argv[2]);
	strcpy(input, argv[1]);
	int i;
	int nThreads = atoi(argv[3]);
	pthread_t threads[nThreads];
	for(i=0; i < nThreads; i++)
	{
		pthread_create(&threads[i], NULL, threadFunc, (void*)(size_t)(i));
	}
	for(i=0; i < nThreads; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return 0;
}
