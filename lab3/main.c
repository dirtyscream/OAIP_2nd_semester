#include <stdio.h>
#include <stdlib.h>

#include "function.h"

int get_choice() {
    int choice = 0;

    while (choice == 0) {
        printf("Enter the number of task or enter '4' to exit: ");
        scanf("%d", &choice);

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
        case 2:
            task2(file_name);
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
