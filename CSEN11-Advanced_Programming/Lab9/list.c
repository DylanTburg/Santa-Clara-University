#include "library.h"
void insert(char *first, char *last, int* age)
{
    Person *temp = (Person*)malloc(sizeof(Person));
    strcpy(temp->firstName, first);
    strcpy(temp->lastName, last);
    temp->age = age;
    if (temp->age <= 18)
    {
        Person *p = head[0];
        if(head[0] == NULL)
        {
            head[0] = temp;
            head[0]->next = NULL;
            head[0]->prev = NULL;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
            p->next->prev = p;
            temp->next = NULL;
        }
    }
    if (temp->age > 18 && temp->age < 65)
    {
        Person *p = head[1];
        if(head[1] == NULL)
        {
            head[1] = temp;
            head[1]->next = NULL;
            head[1]->prev = NULL;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
            p->next->prev = p;
            temp->next = NULL;
        }
    }
    if (temp->age >= 65)
    {
        Person *p = head[2];
        if(head[2] == NULL)
        {
            head[2] = temp;
            head[2]->next = NULL;
            head[2]->prev = NULL;
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
            p->next->prev = p;
            temp->next = NULL;
        }
    }
}
int check_duplicate(char *string, char *string2)
{
    for (int i = 0; i < 3; i++)
    {
    Person *d = head[i];
        while(d != NULL)
        {
            int val = strcmp(string, d->firstName);
            int val2 = strcmp(string2, d->lastName);
            if (val == 0 && val2 == 0)
            {
                printf("No duplicates!");
                return 1;
            }
            d = d->next;
        }
    }
    return 0;
}
void list()
{
    Person *p = head[0];
    printf("Ages less than or equal to 18: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
    p = head[1];
    printf("\n");
    printf("Ages greater than 18 but less than 65: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
    p = head[2];
    printf("\n");
    printf("Ages greater than or equal to 65: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
}
void list_2()
{
    Person *p = head[0];
    while (p && p->next)
    {
        p = p->next;
    }
    printf("Ages less than or equal to 18: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->prev;
    }
    p = head[1];
    while (p && p->next)
    {
        p = p->next;
    }
    printf("\n");
    printf("Ages greater than 18 but less than 65: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->prev;
    }
    p = head[2];
    while (p && p->next)
    {
        p = p->next;
    }
    printf("\n");
    printf("Ages greater than or equal to 65: \n");
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->prev;
    }
}
void list_age()
{
    int i;
    int test1 = 0;
    int age1 = 0;
    printf("What age are you looking for?\n");
    scanf("%d", &age1);
    for (i = 0; i <3; i++)
    {
        Person *p = head[i];
        while(p)
        {
            if (age1 == p->age)
            {
                printf("%s %s is %d years old. \n",p->firstName, p->lastName, p->age);
                test1 = 1;
            }
            p = p->next;
        }
    }
    if (test1 == 0)
    {
        printf("No one is %d years old. \n", age1);
    }
}
void remove1()
{
    char remove2[100];
    int remove3[100];
    printf("Enter a first name to remove (type first name): \n");
    scanf("%s", remove2);
    printf("Enter a last name to remove (type last name): \n");
    scanf("%s", remove3);
    for (int i = 0; i < 3; i++)
    {
        Person *p = head[i];
        Person *q = head[i];
        while(p)
        {
            int z;
            int y;
            z = strcmp(remove2, p->firstName);
            y = strcmp(remove3, p->lastName);
            if (z == 0 && y ==0)
            {
                if (p == head[i])
                {
                    head[i] = head[i]->next;
                    printf("Removed. \n");
                    return;
                }
                if (p->next == NULL)
                {
                    q->next = NULL;
                    p = NULL;
                    free(p);
                    printf("Removed. \n");
                    return;
                }
                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    free(p);
                    printf("Removed. \n");
                    break;
                }
            }
            q=p;
            p=p->next;
        }
    }
    return;
}
void change_age()
{
    int i;
    int test1 = 0;
    char first[100];
    char last[100];
    printf("Whose age would you like to change? \n");
    printf("Enter the first name:\n");
    scanf("%s", first);
    printf("Enter the last name:\n");
    scanf("%s", last);
    for (i = 0; i <3; i++)
    {
        Person *p = head[i];
        Person *q = head[i];
        char str [100];
        char str2[100];
        int age;
        while(p)
        {
            if ((strcmp(first, p->firstName) == 0) && (strcmp(last, p->lastName) == 0))
            {
                printf("What is %s %s's new age? (currently %d) \n",p->firstName, p->lastName, p->age);
                strcpy(str, p->firstName);
                strcpy(str2, p->lastName);
                age = p->age;
                if (p == head[i])
                {
                    head[i] = head[i]->next;
                }
                if (p->next == NULL)
                {
                    q->next = NULL;
                    p = NULL;
                    free(p);
                }
                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    free(p);
                }
                scanf("%d", &age);
                printf("%s %s's new age is %d. \n",str, str2, age);
                insert(str, str2, age);
                test1 = 1;
                return;
            }
            p = p->next;
        }
    }
    if (test1 == 0)
    {
        printf("No one is named \"%s %s\" in this list. \n", first, last);
    }
}
