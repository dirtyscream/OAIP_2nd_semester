#include <ctype.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"

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

Node *add_node(Student *student) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->student = student;
    temp->next = NULL;
    return temp;
}

void append(struct List *self, Student *student) {
    Node *new_node = add_node(student);
    Node *last = self->head;
    if (self->head == NULL) {
        self->head = new_node;
        new_node->next = self->head;
        return;
    }
    while (last->next != self->head) {
        last = last->next;
    }
    last->next = new_node;
    new_node->next = self->head;
}

void print_list(struct List *self) {
    Node *temp = self->head;
    if (self->head != NULL) {
        do {
            Student *student = temp->student;
            printf("Name: %s, Number: %d\n", student->name, student->number);
            if (student->marks != NULL) {
                printf("Marks in stack for %s: ", student->name);
                Node *stack_node = student->marks->top;
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

void add_info(List *list) {
    char name[50];
    printf("Enter the name of the student: ");
    fgets(name, 50, stdin);
    size_t length = strlen(name);
    if (name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }
    int number;
    printf("Enter the group of the student: ");
    scanf("%d", &number);
    Stack *marks = stack_init();
    int mark = 1;
    while (1) {
        printf("Enter mark from (1 to 10) or 0 to exit: ");
        scanf("%d", &mark);
        if (mark == 0) {
            break;
        }
        marks->push(marks, mark);
    }
    Student *student = (Student *)malloc(sizeof(Student));
    student->name = (char *)malloc(strlen(name) + 1);
    strcpy(student->name, name);
    student->number = number;
    student->marks = marks;
    list->append(list, student);
}

Node *FindNext(Node *stack, Node *next) {
    Node *p = stack;
    if (!next) {
        while (p->next) {
            p = p->next;
        }
        return p;
    }
    for (; p->next != next; p = p->next)
        ;
    return p;
}
void sort_by_number(List *list) {
    Node *root = list->head;
    Node *p, *key;
    Node *result = root;
    root = root->next;
    result->next = NULL;

    while (root->next != NULL) {
        if (root->next == root) {
            break;
        }
        key = root;
        root = root->next;
        if (key->student->number < result->student->number) {
            key->next = result;
            result = key;
        } else {
            p = result;
            while (p->next != NULL) {
                if (p->next->student->number > key->student->number) break;
                p = p->next;
            }
            key->next = p->next;
            p->next = key;
        }
    }
    root = result;
    list->head = root;
}

void sort_name(List *list) {
    Node *node = list->head;
    sort_by_number(list);
    if (list->head != NULL) {
        do {
            Node *next_node = node->next;
            if (next_node == list->head) {
                break;
            }
            if (node->student->number != next_node->student->number) {
                continue;
            }
            while (next_node != list->head) {
                if (strcmp(next_node->student->name, node->student->name) < 0) {
                    Student *temp_student = node->student;
                    node->student = next_node->student;
                    next_node->student = temp_student;
                }
                next_node = next_node->next;
            }
            node = node->next;
        } while (node != list->head);
    }
}

void task2(char *db_name) {
    List *list = list_init();
    int choice;
    while (1) {
        printf(
            "Select the task: \n1) Show info\n2) Add info\n3) Sort\n"
            "4) Delete students(lower than 3)\n5) Search "
            "by name\n6) Search by group number\n7) Modify info.\n8) Save to "
            "database\n9) Load data from database\n10) Exit program\n");
        scanf("%d", &choice);
        scanf("%*c");

        if (choice < 0 || choice > 11) {
            printf("Input not correct, try again\n");
        } else {
            switch (choice) {
                case 1: {
                    list->print_list(list);
                    break;
                }
                case 2:
                    add_info(list);
                    break;
                case 3:
                    sort_name(list);
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