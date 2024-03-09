#include <stdio.h>
#include <stdlib.h>

#include "module.h"

struct element {
    int index;
    int count;
};

int process_data(const char* file_path, int length) {
    FILE* file = fopen(file_path, "rb+");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }
    int max_length = 1;
    struct element max_element = {.index = 0, .count = 1};

    for (int i = 1; i < length; i++) {
        int current;
        fseek(file, i * sizeof(int), SEEK_SET);
        fread(&current, sizeof(int), 1, file);
        int previous;

        fseek(file, (i - 1) * sizeof(int), SEEK_SET);
        fread(&previous, sizeof(int), 1, file);

        if (current == previous) {
            max_length++;
            if (max_element.count < max_length) {
                max_element.count = max_length;
                max_element.index = i;
            }
        } else {
            max_length = 1;
        }
    }
    if (max_element.count > 1) {
        int result;
        fseek(file, (max_element.index) * sizeof(int), SEEK_SET);
        fread(&result, sizeof(int), 1, file);
        fclose(file);
        printf("Number of elements: %d\n", max_element.count);
        return result;
    } else {
        fclose(file);
        return -1;
    }
}

void task1(const char* file_path) {
    write_array(file_path);
    int* array = read_array(file_path);
    int length = get_length(file_path);
    print_array(array, length);
    int element = process_data(file_path, length);
    if (element == -1) {
        printf("No repeating elements found\n");
        return;
    }
    printf("The largest element sequence is: %d\n", element);
    free(array);
}