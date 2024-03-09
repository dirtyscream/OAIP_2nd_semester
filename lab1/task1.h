#include <stdio.h>
#include <stdlib.h>

#include "module.h"

int length;

struct element {
    int index;
    int count;
};

int process_data(int* array, int length);
void task1(const char* file_path);