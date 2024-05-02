#include <stdio.h>

struct Node;

typedef struct Stack {
    struct Node *top;
    void (*push)(struct Stack *, int);
    int (*pop)(struct Stack *);
    int (*is_empty)(struct Stack *);
} Stack;

typedef struct Student {
    char *name;
    int number;
    Stack *marks;
} Student;

typedef struct Node {
    int data;
    Student *student;
    struct Node *next;
} Node;

typedef struct List {
    Node *head;
    void (*append)(struct List *self, Student *student);
    void (*print_list)(struct List *self);
} List;