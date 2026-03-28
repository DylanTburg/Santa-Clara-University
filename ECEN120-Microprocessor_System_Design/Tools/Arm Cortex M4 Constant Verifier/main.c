#include <stdio.h>
#include <stdlib.h>

int num()
{
    int i = 0;
    int x;
    int j = 0;
    char a[9];
    printf("Pick an eight digit number, using zeros if necessary, in hex to test. No '0x' prefix and no mixing lowercase with uppercase.: ");
    scanf("%s", a);
    if(a[0]=='0' && a[2] != '0') //Determines if number has 00xy00xy notation which is allowed
        if(a[1]=='0' && a[3] != '0')
            if(a[4]=='0' && a[6] != '0')
                if((a[5]=='0' && a[7] != '0') && (a[2] == a[6] && a[3] == a[7]))
                {
                    printf("%s can be used in ARM", a);
                    return 0;
                }
    if(a[2]=='0' && a[0] != '0') //Determines if number has xy00xy00 notation which is allowed
        if(a[3]=='0' && a[1] != '0')
            if(a[6]=='0' && a[4] != '0')
                if((a[7]=='0' && a[5] != '0') && (a[0] == a[4] && a[1] == a[5]))
                {
                    printf("%s can be used in ARM", a);
                    return 0;
                }
    if((a[0]== a[2] && a[0] == a[4] && a[0] == a[6]) && (a[1] == a[3] && a[1] == a[5] && a[1] == a[7])) //Determines if all four bytes are the same
    {
        printf("%s can be used in ARM", a);
        return 0;
    }
    char binary[33];
    int g;
    int f = 3;
    for(g=0; g < 8; g++) //case loop that converts hex to binary
    {
        switch (a[g])
        {
        case '0':
            binary[f] = '0';
            binary[f-1] = '0';
            binary[f-2] = '0';
            binary[f-3] = '0';
            break;
        case '1':
            binary[f] = '1';
            binary[f-1] = '0';
            binary[f-2] = '0';
            binary[f-3] = '0';
            break;
        case '2':
            binary[f] = '0';
            binary[f-1] = '1';
            binary[f-2] = '0';
            binary[f-3] = '0';
            break;
        case '3':
            binary[f] = '1';
            binary[f-1] = '1';
            binary[f-2] = '0';
            binary[f-3] = '0';
            break;
        case '4':
            binary[f] = '0';
            binary[f-1] = '0';
            binary[f-2] = '1';
            binary[f-3] = '0';
            break;
        case '5':
            binary[f] = '1';
            binary[f-1] = '0';
            binary[f-2] = '1';
            binary[f-3] = '0';
            break;
        case '6':
            binary[f] = '0';
            binary[f-1] = '1';
            binary[f-2] = '1';
            binary[f-3] = '0';
            break;
        case '7':
            binary[f] = '1';
            binary[f-1] = '1';
            binary[f-2] = '1';
            binary[f-3] = '0';
            break;
        case '8':
            binary[f] = '0';
            binary[f-1] = '0';
            binary[f-2] = '0';
            binary[f-3] = '1';
            break;
        case '9':
            binary[f] = '1';
            binary[f-1] = '0';
            binary[f-2] = '0';
            binary[f-3] = '1';
            break;
        case 'A':
        case 'a':
            binary[f] = '0';
            binary[f-1] = '1';
            binary[f-2] = '0';
            binary[f-3] = '1';
            break;
        case 'B':
        case 'b':
            binary[f] = '1';
            binary[f-1] = '1';
            binary[f-2] = '0';
            binary[f-3] = '1';
            break;
        case 'C':
        case 'c':
            binary[f] = '0';
            binary[f-1] = '0';
            binary[f-2] = '1';
            binary[f-3] = '1';
            break;
        case 'D':
        case 'd':
            binary[f] = '1';
            binary[f-1] = '0';
            binary[f-2] = '1';
            binary[f-3] = '1';
            break;
        case 'E':
        case 'e':
            binary[f] = '0';
            binary[f-1] = '1';
            binary[f-2] = '1';
            binary[f-3] = '1';
            break;
        case 'F':
        case 'f':
            binary[f] = '1';
            binary[f-1] = '1';
            binary[f-2] = '1';
            binary[f-3] = '1';
            break;
        default:
            printf("\nInvalid hexadecimal digit %c. Try again and remember, it needs to be 8 digits long. \n", a[g]);
            return 0;
        }
        f+=4;
    }
    for(g=0; g<32;g++) //Loop that uses the binary number converted from hex to determine if it breaks convention of the last acceptable constant format
    {                  //The format simply states that once the first 1 bit is found, there can be no one bits more than 7 bits later (8-bit distance max between start 1 and end 1)
        if(binary[g] == '1') //This looks like this: xy000000,0xy00000,00xy0000,000xy000,0000xy00,00000xy0,000000xy. Something like 00xy000y or 00xyffff would not work
        {
            int start = g;
            int h = g;
            while(h < 32)
            {
                if((h-start) > 7 && binary[h] == '1')
                {
                    printf("%s CANNOT be used in ARM", a);
                    return 0;
                }
                h++;
            }
        }
    }
    printf("%s can be used in ARM", a);
    return 0;
}
int main()
{
    char cond = 'y';
    while (cond == 'y')
    {
        num();
        printf("\nWould you like to test a different constant? Enter y if yes.");
        scanf(" %c", &cond);
    }
}
