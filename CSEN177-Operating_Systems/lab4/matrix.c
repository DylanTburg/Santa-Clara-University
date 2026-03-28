/*# Name: Dylan Thornburg
 *# Date: 10-15-2024 | Tuesday 2:15-5:00
 *# Title: Lab4 - Matrix Multiplication
 *# Description: Use threads to create a program that does matrix multiplication*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NTHREADS 3

void *mult(void *arg);
pthread_t threads[NTHREADS];
double matrixA[NTHREADS][NTHREADS], matrixB[NTHREADS][NTHREADS], matrixC[NTHREADS][NTHREADS];

void *mult(void *arg)
{
	int i = (int)(size_t)arg;
	for(int j=0; j < NTHREADS; j++)
	{
		double temp = 0; //this temp variable isn't necessary for square matrices, but it is for rectangular ones
		for(int k = 0; k < NTHREADS; k++)
		{
			temp += matrixA[i][k] * matrixB[k][j];
		}
		matrixC[i][j] = temp;
	}

}


int main() 
{
	srand(time(NULL)); //only need one. Having two caused the matrices A and B to be the same
	for(int i = 0; i < NTHREADS; i++)
	{
		for(int j = 0; j < NTHREADS; j++)
		{
			matrixA[i][j] = rand() % 10; //keeps numbers between 0-9
			printf(" %.1f ", matrixA[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	
     	for(int i = 0; i < NTHREADS; i++)
	{
		for(int j = 0; j < NTHREADS; j++)
		{
			matrixB[i][j] = rand() % 10; 
			printf(" %.1f ", matrixB[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	
	int x;
	for(x=0; x < NTHREADS; x++)
	{
		pthread_create(&threads[x], NULL, mult, (void *)(size_t)x);
		//Using the alternate casting method in lab 3 step 7 caused a seg fault here
	}

	for(x=0; x < NTHREADS; x++)
		pthread_join(threads[x], NULL);

	for(int i = 0; i < NTHREADS; i++)
	{
		for(int j = 0; j < NTHREADS; j++)
		{
			printf(" %.1f ", matrixC[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	
}

