#include <ctype.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student {
    char *name;
    int number;
    Stack *stack;
} Student;

typedef struct Node {
    int data;
    Student *student;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
    void (*push)(struct Stack *, int);
    int (*pop)(struct Stack *);
    int (*is_empty)(struct Stack *);
} Stack;

void push(Stack *stack, int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

int pop(Stack *stack) {
    if (stack->is_empty(stack)) {
        printf("Error: stack is empty\n");
        return -1;
    }
    int data = stack->top->data;
    Node *temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

int is_empty(Stack *stack) { return stack->top == NULL; }

Stack *stack_init() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->top = NULL;
    stack->push = push;
    stack->pop = pop;
    stack->is_empty = is_empty;
    return stack;
}

typedef struct List {
    Node *head;
    void (*append)(struct List *self, int data);
    void (*print_list)(struct List *self);
} List;

Node *add_node(int data) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

void append(struct List *self, int data) {
    Node *new_node = add_node(data);
    Node *last = self->head;
    if (self->head == NULL) {
        self->head = new_node;
        new_node->next = self->head;
        return;
    }
    while (last->next != self->head) last = last->next;
    last->next = new_node;
    new_node->next = self->head;
}

void print_list(struct List *self) {
    Node *temp = self->head;
    if (self->head != NULL) {
        do {
            Student *student = temp->student;
            printf("Name: %s, Number: %d\n", student->name, student->number);
            if (student->stack != NULL) {
                printf("Marks in stack for %s: ", student->name);
                Node *stack_node = student->stack->top;
                while (stack_node != NULL) {
                    printf("%d ", stack_node->data);
                    stack_node = stack_node->next;
                }
                printf("\n");
            }
            temp = temp->next;
        } while (temp != self->head);
    }
    printf("\n");
}

List *list_init() {
    List *clist = (List *)malloc(sizeof(List));
    clist->head = NULL;
    clist->append = &append;
    clist->print_list = &print_list;
    return clist;
}

void task2(char *db_name) {
    List *list = list_init();
    int choice;
    while (1) {
        printf(
            "Select the task: \n1) Show info\n2) Add info\n3)Sort by Name\n4) "
            "Sort by group number\n5) Delete students(lower than 3)\n6) Search "
            "by name\n7) Search by group number\n8) Modify info.\n9) Save to "
            "database\n 10) Load data from database\n11) Exit program\n");
        scanf("%d", &choice);

        if (choice < 0 || choice > 11) {
            printf("Input not correct, try again\n");
        } else {
            switch (choice) {
                case 1: {
                    list->print_list(list);
                    break;
                }
                case 2:

                case 3:

                    break;

                case 4:

                    break;

                case 5:

                    break;
                case 6:

                    break;
                case 7:

                    break;
                case 8:

                    break;
                case 9:

                    break;
                case 10:

                    break;
                case 11:
                    exit(0);
                default:
                    exit(1);
            }
        }
    }
    return 0;
}

int get_choice() {
    int choice = 0;

    while (choice == 0) {
        printf("Enter the number of task or enter '4' to exit: ");
        scanf("%d", &choice);
        scanf("%*c");
        if (choice == 0 || choice > 4 || choice < 0) {
            printf("Invalid value\n");
        }
    }

    return choice;
}

void execute_task(int choice, char *db_name) {
    switch (choice) {
        case 2:
            task2(db_name);
            break;
        default:
            printf("Invalid value\n");
            break;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Database error\n");
        return 1;
    }

    while (1) {
        int choice = get_choice();

        if (choice == 4) {
            printf("Exiting...\n");
            exit(4);
        }

        execute_task(choice, argv[1]);
    }

    return 0;
}