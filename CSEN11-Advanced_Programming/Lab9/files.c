#include "library.h"
void write_file()
{
    FILE* in;
    in = fopen("file_name.txt", "w");
    Person *p = head[0];
    while(p)
    {
        fprintf(in, "%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
    free(p);
    delete_recurse(p);
    p = head[1];
    while(p)
    {
        fprintf(in, "%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
    free(p);
    delete_recurse(p);
    p = head[2];
    while(p)
    {
        fprintf(in, "%s %s %d \n", p->firstName, p->lastName, p->age);
        p = p->next;
    }
    delete_recurse(p);
    free(p);
    fclose(in);
}
void read_file()
{
    char str[100];
    char str2[100];
    int age;
    FILE* out;
    out = fopen("file_name.txt", "r");
    if (NULL == out)
    {
        printf("File cannot be opened. \n");
        return;
    }
    while (fscanf(out, "%s %s %d", str, str2, &age) != EOF){
        insert(str, str2, age);
    }
    fclose(out);
}
void read_file_b()
{
    char str[100];
    char str2[100];
    int age;
    FILE* out;
    Person p;
    out = fopen("file_name_b.txt", "r");
    if (NULL == out)
    {
        printf("File cannot be opened. \n");
        return;
    }
    printf("Currently saved: \n");
    while (fread(&p, sizeof(Person), 1, out) == 1)
    {
        printf("%s %s %d", p.firstName, p.lastName, p.age);
        printf("\n");
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
void *timeThread(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        FILE* in;
        in = fopen("file_name_b.txt", "w");
        Person *p = head[0];
        while(p)
        {
            fwrite(p, sizeof(Person), 1, in);
            p = p->next;
        }
        p = head[1];
        while(p)
        {
            fwrite(p, sizeof(Person), 1, in);
            p = p->next;
        }
        p = head[2];
        while(p)
        {
            fwrite(p, sizeof(Person), 1, in);
            p = p->next;
        }
        fclose(in);
        pthread_mutex_unlock(&lock);
        sleep(15);
    }
    return NULL;
}
