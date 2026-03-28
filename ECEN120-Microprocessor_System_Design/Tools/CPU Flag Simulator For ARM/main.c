#include <stdio.h>
#include <stdlib.h>

int main()
{
    char cond = 'y';
    while (cond == 'y')
    {
        signed long int x;
        signed long int y;
        signed long int result;
        char k;
        int n=0;
        int z=0;
        int c=0;
        int v=0;
        printf("Enter your two numbers (in hex, no 0x prefix): \n");
        scanf("%X %X", &x, &y);
        printf("\nWould you like to do subtraction or addition? Type '-' for subtraction or '+' for addition: ");
        scanf(" %c", &k);
        if (k == '-')
        {
            result = (x-y);
            int mask = 0x80000000;
            _Bool maskx = x & mask;
            _Bool masky = y & mask;
            _Bool maskr = result & mask;
            c=!((masky & maskr) | (!maskx & maskr) | (!maskx & masky));
            v=((!maskr & maskx & !masky) | (!maskx & maskr & masky));
        }

        if (k == '+')
        {
            result = x+y;
            int mask = 0x80000000;
            _Bool maskx = x & mask;
            _Bool masky = y & mask;
            _Bool maskr = result & mask;
            c=((masky & !maskr) | (maskx & !maskr) | (maskx & masky));
            v=((!maskr & maskx & masky) | (!maskx & maskr & !masky));
        }
        if (result == 0)
        {
            z = 1;
        }
        if (result < 0)
        {
            n = 1;
        }
        printf("The result is %X\n", result);
        printf("Flags:\nN=%d \nZ=%d \nC=%d \nV=%d",n,z,c,v);
        printf("\nWould you like to test again? Type y if yes. ");
        scanf(" %c", &cond);
    }
    return 0;
}
