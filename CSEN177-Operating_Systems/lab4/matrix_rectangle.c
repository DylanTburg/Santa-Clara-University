/*# Name: Dylan Thornburg
 *# Date: 10-15-2024 | Tuesday 2:15-5:00
 *# Title: Lab4 - Matrix Multiplication For Rectangular Matrices
 *# Description: Use threads to create a program that does matrix multiplication
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NTHREADS 3
#define L 2
#define M NTHREADS
#define N 4

void *mult(void *arg);
pthread_t threads[NTHREADS];
double matrixA[L][M], matrixB[M][N], matrixC[L][N];

void *mult(void *arg)
{
	int i = (int)(size_t)arg;
	for(int j=0; j < N; j++)
	{
		double temp = 0;
		for(int k = 0; k < M; k++)
		{
			temp += matrixA[i][k] * matrixB[k][j];
		}
		matrixC[i][j] = temp; 
	}

}


int main() 
{
	srand(time(NULL)); //only need one. Having two caused the matrices A and B to be the same
	for(int i = 0; i < L; i++)
	{
		for(int j = 0; j < M; j++)
		{
			matrixA[i][j] = rand() % 10; //keeps numbers between 0-9
			printf(" %.1f ", matrixA[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	
     	for(int i = 0; i < M; i++)
	{
		for(int j = 0; j < N; j++)
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

	for(int i = 0; i < L; i++)
	{
		for(int j = 0; j < N; j++)
		{
			printf(" %.1f ", matrixC[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	
}

