#include <stdio.h>
#include <string.h>
char firstName[10][1000];
char number = '5';
char lastName[10][1000];
int age[10];
int i = 0;
int entry = 0;
int g;
int y;
void insert()
{
            if (entry >= 10)
            {
                printf("No more spaces!");
                return;
            }
            printf("Enter your first name: \n");
            scanf("%s", &firstName[entry]);
            for(y = 0;y < 10; y++)
            {
                if (y == entry)
                {
                    continue;
                }
                int val = strcmp(firstName[y], firstName[entry]);
                if (val == 0)
                {
                    printf("Cannot have repeat first or last names!\n");
                    return;
                }
            }
            printf("Enter your last name: \n");
            scanf("%s", &lastName[entry]);
            for(y=0;y < 10; y++)
            {
                if(y == entry)
                {
                    continue;
                }
                int val = strcmp(lastName[y],lastName[entry]);
                if (val == 0)
                {
                    printf("Cannot have repeat first or last names!\n");
                    return;
                }
            }
            printf("Enter your age: \n");
            scanf("%d", &age[entry]);
            number = 5;
            entry++;
}
void list()
{
    i = 0;
    for (i; i < entry; i++)
    {
        printf("%s %s %d \n", firstName[i], lastName[i], age[i]);
    }
}
void list_age()
{
    int test1 = 0;
    int age1 = 0;
    printf("What age are you looking for?\n");
    scanf("%d", &age1);
    for (g = 0; g < 10; g++)
    {
        if (age1 == age[g])
        {
            printf("%s %s is %d years old. \n",firstName[g], lastName[g], age[g]);
            test1 = 1;
        }

    }
    if (test1 == 0)
    {
            printf("No one is %d years old. \n", age1);
    }
}
int main()
{
    while(number != 0)
    {
        printf("\n");
        printf("Type \"1\" to add first name, last name, and age. \n");
        printf("Type \"2\" to show the list from oldest entries to newest. \n");
        printf("Type \"3\" to see what people have a specific age. \n");
        printf("Type \"0\" to quit \n");
        scanf("%s", &number);
        switch(number)
        {
        case '1':
            insert();
            break;
        case '2':
            list();
            break;

        case '3':
            list_age();
            break;

        case '0':
            printf("Goodbye then! \n");
            return;
            break;

        default:
            printf("Not a valid option \n");
        }

    }
    return 0;
}

