#include <stdio.h>
#include <stdlib.h>

#include "task1.h"
#include "task2.h"

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