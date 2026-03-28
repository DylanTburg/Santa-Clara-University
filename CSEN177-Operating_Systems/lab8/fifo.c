//Name: Dylan Thornburg
//Date: 11/12/2024
//Title: Lab8 - step 4 fifo
//Description: Code for my fifo algorithm


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


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

    int placeInArray = 0;
    int requestCount=0;
    
    for (i = 0; i < CACHE_SIZE; i++){//initialise cache array  
         cache[i].pageno = -1;
    }

    while (fgets(inputBuffer, 100, stdin)){
    	int page_num = atoi(inputBuffer); // Stores number read from file as an int
	requestCount++;
        //printf("%d\n", page_num);
    	/*

		
		Page Replacement Implementation Goes Here 


    	*/
	bool foundInCache = false;
        for (i=0; i<CACHE_SIZE; i++){
            if (cache[i].pageno == page_num){
                foundInCache = true;
                break; //break out loop because found page_num in cache  
            }
        }   
        if (foundInCache == false){
            //You may print the page that caused the page fault
	    printf("Page %d faulted \n", page_num);
            cache[placeInArray].pageno = page_num;
	    totalFaults++;
 	    placeInArray = (placeInArray+1) % CACHE_SIZE; //Need to keep the value within the cacheSize
	    
        }
    }
    double hitrate = (requestCount - totalFaults)/ (double) requestCount;
    fprintf(stderr, "Page Requests: %d\n", requestCount);
    fprintf(stderr, "Page Faults: %d\n", totalFaults);
    fprintf(stderr, "Hitrate: %f\n", hitrate);
    return 0;
}
