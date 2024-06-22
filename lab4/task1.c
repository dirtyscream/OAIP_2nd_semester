#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structures and function pointers
typedef struct Node {
    double data;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
} Stack;

typedef void (*PushFunc)(Stack *, double);
typedef double (*PopFunc)(Stack *);
typedef bool (*IsEmptyFunc)(Stack *);

// Function prototypes
Stack *stack_init();
void push(Stack *stack, double data);
double pop(Stack *stack);
bool is_empty(Stack *stack);
int is_operator(char c);
int priority(char op);
double perform_operation(char op, double operand1, double operand2);
double calculate(char *expression);

// Initialize stack
Stack *stack_init() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->top = NULL;
    return stack;
}

// Push a double onto the stack
void push(Stack *stack, double data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    node->data = data;
    node->next = stack->top;
    stack->top = node;
}

// Pop a double from the stack
double pop(Stack *stack) {
    if (stack->top == NULL) {
        printf("Stack underflow\n");
        exit(1);
    }
    Node *temp = stack->top;
    double data = temp->data;
    stack->top = temp->next;
    free(temp);
    return data;
}

// Check if the stack is empty
bool is_empty(Stack *stack) { return stack->top == NULL; }

// Check if a character is an operator
int is_operator(char c) { return c == '+' || c == '-' || c == '*' || c == '/'; }

// Get the priority of an operator
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Perform arithmetic operations
double perform_operation(char op, double operand1, double operand2) {
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            if (operand2 == 0) {
                printf("Division by zero\n");
                exit(1);
            }
            return operand1 / operand2;
        default:
            printf("Invalid operator\n");
            exit(1);
    }
}

// Calculate expression result
double calculate(char *expression) {
    Stack *stack = stack_init();
    Stack *operators = stack_init();

    bool unary = true;

    while (*expression != '\0') {
        if (*expression == ' ') {
            expression++;
            continue;
        }
        if (isdigit(*expression) || (*expression == '-' && unary)) {
            double operand = strtod(expression, &expression);
            push(stack, operand);
            unary = false;
            continue;
        }
        if (*expression == '(') {
            push(operators, '(');
            unary = true;  // Reset unary flag after opening parenthesis
        } else if (*expression == ')') {
            while (!is_empty(operators) && operators->top->data != '(') {
                char op = (char)pop(operators);
                double operand2 = pop(stack);
                double operand1 = pop(stack);
                double result = perform_operation(op, operand1, operand2);
                push(stack, result);
            }
            if (!is_empty(operators) && operators->top->data == '(') {
                pop(operators);
            } else {
                printf("Error while operating...\n");
                exit(1);
            }
            unary = false;  // Reset unary flag after closing parenthesis
        } else if (is_operator(*expression)) {
            // If the previous character was an operator or if it's the
            // beginning of the expression, treat '-' as unary negation
            if (*expression == '-' &&
                (is_operator(*(expression - 1)) || *(expression - 1) == '(')) {
                push(stack, 0);  // Push 0 onto the stack to represent the unary
                                 // negation
                unary = true;
            } else {
                while (!is_empty(operators) && priority(operators->top->data) >=
                                                   priority(*expression)) {
                    char op = (char)pop(operators);
                    double operand2 = pop(stack);
                    double operand1 = pop(stack);
                    double result = perform_operation(op, operand1, operand2);
                    push(stack, result);
                }
                push(operators, *expression);
                unary = true;  // Reset unary flag after pushing an operator
            }
        } else {
            printf("Invalid expression...\n");
            printf("This symbol is invalid '%c'\n", *expression);
            exit(1);
        }
        expression++;
    }

    while (!is_empty(operators)) {
        char op = (char)pop(operators);
        double operand2 = pop(stack);
        double operand1 = pop(stack);
        double result = perform_operation(op, operand1, operand2);
        push(stack, result);
    }

    double result = stack->top->data;
    free(stack);
    free(operators);
    return result;
}

int main() {
    char expression[100];
    printf("Enter arithmetical expression: ");
    fgets(expression, 100, stdin);
    size_t length = strlen(expression);
    if (expression[length - 1] == '\n') {
        expression[length - 1] = '\0';
    }

    printf("%s\n", expression);
    double result = calculate(expression);
    printf("Result: %lf\n", result);
    return 0;
}
