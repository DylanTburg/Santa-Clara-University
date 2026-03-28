#include "library.h"
int main()
{
    int choice;
    printf("Would you like to import previous list names? (1 for yes, else for no) \n");
    scanf("%d", &choice);
    if (choice == 1)
    {
        read_file();
    }
    pthread_t thr;
    pthread_create(&thr, NULL, timeThread, NULL);
    pthread_mutex_init(&lock, NULL);
    char number = '9';
    while(number != 0)
    {
        Person *temp = (Person*)malloc(sizeof(Person));
        char str[100];
        char str2[100];
        printf("\n");
        printf("Type \"1\" to add first name, last name, and age. \n");
        printf("Type \"2\" to show the list oldest to newest. \n");
        printf("Type \"3\" to show the list newest to oldest. \n");
        printf("Type \"4\" to see what people have a specific age. \n");
        printf("Type \"5\" to remove a name. \n");
        printf("Type \"6\" to change a name's age. \n");
        printf("Type \"7\" to see what names are currently in the auto-save (binary) file. \n");
        printf("Type \"0\" to quit and save. \n");
        scanf("%s", &number);
        switch(number)
        {
        case '1':
            printf("Enter your first name: \n");
            scanf("%s", str);
            printf("Enter your last name: \n");
            scanf("%s", str2);
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
            list_2();
            break;

        case '4':
            list_age();
            break;

        case '5':
            remove1();
            break;

        case '6':
            change_age();
            break;

        case '7':
            read_file_b();
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

