/*
 * File: count.c
 * Dylan Thornburg
 * Description: counts words in a text file
 * Using fgetc, I count how many spaces, newlines, tabs, and EOF to determine how many words
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char* argv[])
{
    char name[100];
    int count = 0;
    char x;
    int y=0;
    FILE* out;
    out = fopen(argv[1], "r");
    if (argc != 2)
    {
	printf("Not enough arguments \n");
    }
    if (!out) //if not a file, it cannot be opened. O(1)
    {
        printf("File cannot be opened. \n");
	return 0;
    }
    do
    {
        if (x == ' ' || x == '\0' || x == '\t' || x == '\n') //if white (space) character, add count. O(1)
        {
            y++;
            count++;
        }
        if ((x != ' ') && (x != '\t') && (x != '\n') && (x != '\0')) //if not space character, reset y O(1)
        {
            y=0;
        } 
        if (y >= 2) //subtract count to avoid it counting multiple white spaces as words. O(1)
        {
            count--;
        }
	if (y == 1 && x == '\0')// use case for a line/space/tab before the end. O(1)
	{
	    count--;
	} 
    }
    while ((x = fgetc(out)) != EOF); //while loop to loop through each char. O(n)
    fclose(out);
    printf("Total words: %d \n", count);
    return 0;
}

