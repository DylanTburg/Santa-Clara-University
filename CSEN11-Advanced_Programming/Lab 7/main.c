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
Person *head[3] = {0};
void insert(char *first, char *last, int *age)
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
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
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
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
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
        }
        else
        {
            while (p->next != NULL)
            {
                p = p->next;
            }
            p->next = temp;
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
    scanf("%s", &remove2);
    printf("Enter a last name to remove (type last name): \n");
    scanf("%s", &remove3);
    for (int i = 0; i < 3; i++)
    {
        NODE *p = head[i];
        NODE *q = head[i];
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
    scanf("%s", &first);
    printf("Enter the last name:\n");
    scanf("%s", &last);
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
void write_file()
{
    FILE* in;
    fopen_s(&in, "file_name.txt", "w");
    Person *p = head[0];
    while(p)
    {
        fwrite(p, sizeof(Person), 1, in);
        p = p->next;
    }
    p = head[0];
    delete_recurse(p);
    p = head[1];
    while(p)
    {
        fwrite(p, sizeof(Person), 1, in);
        p = p->next;
    }
    p = head[1];
    delete_recurse(p);
    p = head[2];
    while(p)
    {
        fwrite(p, sizeof(Person), 1, in);
        p = p->next;
    }
    p = head[2];
    delete_recurse(p);
    fclose(in);
}
void read_file()
{
    char str[100];
    char str2[100];
    int age;
    FILE* out;
    Person p;
    out = fopen("file_name.txt", "r");
    if (NULL == out)
    {
        printf("File cannot be opened. \n");
        return;
    }
    while (fread(&p, sizeof(Person), 1, out) == 1){
        insert(p.firstName, p.lastName, p.age);
    }
    fclose(out);
}
void delete_recurse(Person *p)
{
    if (p == NULL) {
        return;
    }
    delete_recurse(p->next);
    free(p);
}
int main()
{
    int choice;
    printf("Would you like to import previous list names? (1 for yes, else for no) \n");
    scanf("%d", &choice);
    if (choice == 1)
    {
        read_file();
    }
    char number = '7';
    while(number != 0)
    {
        Person *temp = (Person*)malloc(sizeof(Person));
        char str[100];
        char str2[100];
        printf("\n");
        printf("Type \"1\" to add first name, last name, and age. \n");
        printf("Type \"2\" to show the list in alphabetical order. \n");
        printf("Type \"3\" to see what people have a specific age. \n");
        printf("Type \"4\" to remove a name. \n");
        printf("Type \"5\" to change a name's age. \n");
        printf("Type \"0\" to quit and save. \n");
        scanf("%s", &number);
        switch(number)
        {
        case '1':
            printf("Enter your first name: \n");
            scanf("%s", &str);
            printf("Enter your last name: \n");
            scanf("%s", &str2);
            if (check_duplicate(str, str2) == 1)
            {
                continue;
            }
            strcpy(temp->firstName, str);
            strcpy(temp->lastName, str2);
            printf("Enter your age: \n");
            scanf("%d", &temp->age);
            insert(temp->firstName,temp->lastName,temp->age);
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

        case '5':
            change_age();
            break;

        case '0':
            printf("Goodbye then! \n");
            write_file();
            return;
            break;

        default:
            printf("Not a valid option \n");
        }

    }
    return 0;
}

