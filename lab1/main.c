#include <stdio.h>
#include <stdlib.h>

#include "task1.h"
#include "task2.h"
#include "task3.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Invalid arguments\n");
        return 1;
    }
    const char* file_path = argv[1];
    int mode = 1;
    while (mode) {
        printf("Wanna continue: (1 to continue; 0 to exit)\n");
        scanf("%d", &mode);
        if (mode == 0 || mode != 1) {
            break;
        }
        printf("Task 1 (enter 1)\n");
        printf("Task 2 (enter 2)\n");
        printf("Task 3 (enter 3)\n");
        int task;
        scanf("%d", &task);
        switch (task) {
            case 1:
                task1(file_path);
                continue;
            case 2:
                task2(file_path);
                continue;
            case 3:
                task3(file_path);
                continue;
            default:
                printf("Invalid task\n");
                return 0;
        }
    }
    return 0;
}