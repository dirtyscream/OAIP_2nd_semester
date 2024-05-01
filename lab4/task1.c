#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
    void (*push)(struct Stack*, int);
    int (*pop)(struct Stack*);
    int (*is_empty)(struct Stack*);
} Stack;

void push(Stack* stack, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

int pop(Stack* stack) {
    if (stack->is_empty(stack)) {
        printf("Error: stack is empty\n");
        return -1;
    }
    int data = stack->top->data;
    Node* temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

int is_empty(Stack* stack) { return stack->top == NULL; }

Stack* stack_init() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
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

int is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int priority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

int perform_operation(char operator, int op1, int op2) {
    switch (operator) {
        case '+':
            return op1 + op2;
        case '-':
            return op1 - op2;
        case '*':
            return op1 * op2;
        case '/':
            if (op2 == 0) {
                printf("Error: Division by zero\n");
                return -1;
            }
            return op1 / op2;
        default:
            return 0;
    }
}

int evaluate_operation(char* expression) {
    Stack* stack = stack_init();
    Stack* operators = stack_init();

    while (*expression != '\0') {
        if (*expression == ' ') {
            printf("Invalid expression...\n");
            break;
        }
        if (isdigit(*expression)) {
            int operand1 = 0;
            while (*expression != '\0' && isdigit(*expression)) {
                operand1 = (operand1 * 10) + (*expression - '0');
                expression++;
            }
            expression--;
            stack->push(stack, operand1);
        } else if (*expression == '(') {
            operators->push(operators, '(');
        } else if (*expression == ')') {
            while (!operators->is_empty(operators) &&
                   operators->top->data != '(') {
                char op = operators->pop(operators);
                int operand2 = stack->pop(stack);
                int operand1 = stack->pop(stack);
                int result = perform_operation(op, operand1, operand2);
                stack->push(stack, result);
            }
            if (!operators->is_empty(operators) &&
                operators->top->data == '(') {
                operators->pop(operators);
            } else {
                printf("Error while operating...\n");
                break;
            }
        } else if (is_operator(*expression)) {
            while (!operators->is_empty(operators) &&
                   priority(operators->top->data) >= priority(*expression)) {
                char op = operators->pop(operators);
                int operand2 = stack->pop(stack);
                int operand1 = stack->pop(stack);
                int result = perform_operation(op, operand1, operand2);
                stack->push(stack, result);
            }
            operators->push(operators, *expression);
        }
        expression++;
    }

    while (!operators->is_empty(operators)) {
        char op = operators->pop(operators);
        int operand2 = stack->pop(stack);
        int operand1 = stack->pop(stack);
        int result = perform_operation(op, operand1, operand2);
        stack->push(stack, result);
    }

    int result = stack->top->data;
    return result;
}

void task1() {
    char expression[100];
    printf("Enter arithmetical expression: ");
    fgets(expression, 100, stdin);
    printf("%s", expression);
    int result = evaluate_operation(expression);
    printf("Result: %d\n", result);
    return;
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

void execute_task(int choice, char* file_name) {
    switch (choice) {
        case 1:
            task1();
            break;
        default:
            printf("Invalid value\n");
            break;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("File error\n");
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
