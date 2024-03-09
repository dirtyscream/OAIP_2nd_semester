#include <stdio.h>
#include <stdlib.h>

#include "module.h"

void change_position(const char* file_path, int length) {
    FILE* file = fopen(file_path, "rb+");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }
    int max = 0;
    int min = 0;
    fseek(file, 0 * sizeof(int), SEEK_SET);
    fread(&max, sizeof(int), 1, file);

    fseek(file, 0 * sizeof(int), SEEK_SET);
    fread(&min, sizeof(int), 1, file);

    for (int i = 0; i < length; i++) {
        int current;
        fseek(file, i * sizeof(int), SEEK_SET);
        fread(&current, sizeof(int), 1, file);
        if (max < current) {
            max = current;
        }
        if (min > current) {
            min = current;
        }
    }

    for (int i = 0; i < length; i++) {
        int current;
        fseek(file, i * sizeof(int), SEEK_SET);
        fread(&current, sizeof(int), 1, file);
        if (current == min) {
            fseek(file, i * sizeof(int), SEEK_SET);
            fwrite(&max, sizeof(int), 1, file);
        } else if (current == max) {
            fseek(file, i * sizeof(int), SEEK_SET);
            fwrite(&min, sizeof(int), 1, file);
        }
    }
    fclose(file);
}

void task2(const char* file_path) {
    write_array(file_path);
    int length = get_length(file_path);
    int* array = read_array(file_path);
    printf("Arr from file:  ");
    print_array(array, length);
    change_position(file_path, length);
    free(array);
    array = read_array(file_path);
    printf("Modified array: ");
    print_array(array, length);
    free(array);
}