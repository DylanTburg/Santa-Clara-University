//Name: Dylan Thornburg
//Date: 11/12/2024
//Title: Lab8 - Step1
//Description: Code that generates a testinput.txt file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int numRequests = 100;
int possiblePages = 50;
int main(int argc, char *argv[]) {
    FILE *fp;
    char buffer [sizeof(int)];
    int i;
    fp = fopen("testInput.txt", "w");
    for (i=0; i<numRequests; i++){	//Use a number you can test by hand, >possiblePages to force cache hits
        sprintf(buffer, "%d\n", rand()%possiblePages);	//Small for hand testing, >cacheSize to force cache miss
        fputs(buffer, fp);
    }
    fclose(fp);
    return 0;
}
