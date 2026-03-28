//Name: Dylan Thornburg
//Date: 11/12/2024
//Title: Lab8 - step 4 lru
//Description: Code for my lru algorithm


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


typedef struct {
    int pageno;
    int age; //keep track of cache spots ages
    //Any other useful values associated with the page, e.g. referenced
} ref_page;


int main(int argc, char *argv[]){

    int CACHE_SIZE = atoi(argv[1]); // Size of Cache passed by user
    ref_page cache[CACHE_SIZE]; // Virtual cache for simulator 
    char inputBuffer[100]; // Buffer to hold input from test file

    int i, j;
    int totalFaults = 0; // keeps track of the total page faults
    int requestCount = 0;
    
    for (i = 0; i < CACHE_SIZE; i++){//initialise cache array  
         cache[i].pageno = -1;
	 cache[i].age=0;
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
		for(j=0; j < CACHE_SIZE; j++)
		{	
		    cache[j].age++;	    
		}
		cache[i].age=0;

                break; //break out loop because found page_num in cache  
            }
        }   
        if (foundInCache == false){
            //You may print the page that caused the page fault
	    printf("Page %d faulted \n", page_num);
	    totalFaults++;
	
	    for(j=0; j < CACHE_SIZE; j++)
	    {
  		cache[j].age++;
	    }
	    int oldest = -1; //keep track of oldest age in cache for replacement in fault event
	    int placeInArray = -1;
	    for(j=0; j < CACHE_SIZE; j++)
	    {
		if(oldest < cache[j].age)
		{
		    oldest=cache[j].age;
		    placeInArray=j;
		}
	    }
	    cache[placeInArray].pageno = page_num;
 	    cache[placeInArray].age=0;
	    
        }
    }
    double hitrate = (requestCount - totalFaults)/ (double) requestCount;
    fprintf(stderr, "Page Requests: %d\n", requestCount);
    fprintf(stderr, "Page Faults: %d\n", totalFaults);
    fprintf(stderr, "Hitrate: %f\n", hitrate);
    return 0;
}
