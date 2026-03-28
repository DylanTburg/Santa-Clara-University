#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#define NODE struct person
typedef struct person
{
    char firstName[100];
    char lastName[100];
    int age;
    struct person *next;
    struct person *prev;
} Person;
Person *head[3];
pthread_mutex_t lock;
void insert(char *first, char *last, int *age);
int check_duplicate(char *string, char *string2);
void list();
void list_2();
void list_age();
void remove1();
void change_age();
void write_file();
void read_file();
void read_file_b();
void delete_recurse(Person *p);
void *timeThread(void *arg);
#endif // LIBRARY_H_INCLUDED
