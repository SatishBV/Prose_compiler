#ifndef STACK
#define STACK

#include "headers.h"

typedef struct stack {
    char* value;
    struct stack* next;
    struct stack* prev;
} stack;

stack* s = NULL;

void push(char* str)
{
    stack* s1 = (stack*)malloc(sizeof(stack));
    s1->value = (char*)malloc(sizeof(str));
    strcpy(s1->value, str);
    //s1->value = str;
    s1->next = NULL;
    s1->prev = s;
    s = s1;
    return;
}

char* pop()
{
    if (s == NULL)
        return NULL;
    else {
        char* a = s->value;
        if (s->prev == NULL) {
            free(s);
            s = NULL;
            return a;
        }
        s = s->prev;
        free(s->next);
        s->next = NULL;
        return a;
    }
}

char* readTop()
{
    return s->value;
}

void printStack()
{
    stack* s2 = s;

    while (s2 != NULL) {
        if (s2->prev != NULL) {
            printf("%s <- ", s2->value);
        } else {
            printf("%s \n", s2->value);
        }

        s2 = s2->prev;
    }
    printf("\n");
}

#endif