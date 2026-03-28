#include <stdio.h>
int main()
{
    srand((int)time(NULL));
    int correct;
    int count = 0;

    for(count; count < 10; count++)
    {
        int divisor = rand() % 12 + 1; /* 12 + 1 Because divisor cannot equal zero. Range goes from 0-11 to 1-12 with the + 1 */
        int quotient = rand() % 13; /*This can be zero */
        int dividend = divisor * quotient;
        int answer;
        printf("What is %d / %d: ", dividend, divisor);
        scanf(" %d", &answer);
        if (answer == quotient)
        {
            printf("Good job! +1 correct.\n");
            correct++;
        }
        else
            printf("OOF! Try again next time bozo.\n");
    }
    printf("Final score: %d/10", correct);
}
