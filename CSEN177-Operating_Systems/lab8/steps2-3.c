//Name: Dylan Thornburg
//Date: 11/12/2024
//Title: Lab8 - steps 2-3
//Description: read in and reprint output from standrad input into the page replacement program.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
    int pageno;
    //Any other useful values associated with the page, e.g. referenced
} ref_page;


int main(int argc, char *argv[]){

    int CACHE_SIZE = atoi(argv[1]); // Size of Cache passed by user
    ref_page cache[CACHE_SIZE]; // Virtual cache for simulator 
    char inputBuffer[100]; // Buffer to hold input from test file

    int i;
    int totalFaults = 0; // keeps track of the total page faults
    
    for (i = 0; i < CACHE_SIZE; i++){//initialise cache array  
         cache[i].pageno = -1;
    }

    while (fgets(inputBuffer, 100, stdin)){
    	int page_num = atoi(inputBuffer); // Stores number read from file as an int
        printf("%d\n", page_num);
    	/*

		
		Page Replacement Implementation Goes Here 


    	*/
    }

    return 0;
}
