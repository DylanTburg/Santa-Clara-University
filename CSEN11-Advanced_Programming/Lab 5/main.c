#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NODE struct person
typedef struct person
{
    char firstName[100];
    char lastName[100];
    int age;
    struct person *next;
} Person;
Person *head = NULL;
void insert()
{
    Person *p = head;
    int count = 0;
    Person *temp = (Person*)malloc(sizeof(Person));
    char str[100];
    char str2[100];
    printf("Enter your first name: \n");
    scanf("%s", &str);
    printf("Enter your last name: \n");
    scanf("%s", &str2);
    if (check_duplicate(str, str2) == 1)
    {
        return;
    }
    strcpy(temp->firstName, str);
    strcpy(temp->lastName, str2);
    printf("Enter your age: \n");
    scanf("%d", &temp->age);
    if(head == NULL)
    {
        head = temp;
        head->next = NULL;
    }
    else
    {
        temp->next = head;
        head = temp;
    }
}

int check_duplicate(char *string, char *string2)
{
    Person *d = head;
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
    return 0;
}
void list()
{
    Person *p = head;
    while(p)
    {
        printf("%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
}
 void list_age()
{
    Person *p = head;
    int test1 = 0;
    int age1 = 0;
    printf("What age are you looking for?\n");
    scanf("%d", &age1);
    while(p)
    {
        if (age1 == p->age)
        {
            printf("%s %s is %d years old. \n",p->firstName, p->lastName, p->age);
            test1 = 1;
        }
        p = p->next;
    }
    if (test1 == 0)
    {
        printf("No one is %d years old. \n", age1);
    }
}
void remove1()
{
    NODE *p = head;
    NODE *q = head;
    char remove2[100];
    int remove3[100];
    printf("Enter a first name to remove (type first name): \n");
    scanf("%s", &remove2);
    printf("Enter a last name to remove (type last name): \n");
    scanf("%s", &remove3);
    while(p)
    {
        int z;
        int y;
        z = strcmp(remove2, p->firstName);
        y = strcmp(remove3, p->lastName);
        if (z == 0 && y ==0)
        {
            if (p == head) {
                head = head->next;
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
        p = p->next;
    }
    return;

}
int main()
{
    char number = '5';
    while(number != 0)
    {
        printf("\n");
        printf("Type \"1\" to add first name, last name, and age. \n");
        printf("Type \"2\" to show the list in alphabetical order. \n");
        printf("Type \"3\" to see what people have a specific age. \n");
        printf("Type \"4\" to remove a name. \n");
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

        case '4':
            remove1();
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
