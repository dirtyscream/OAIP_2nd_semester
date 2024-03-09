#include <stdio.h>
#include <stdlib.h>

#include "module.h"

void modify_array_center(const char* file_path, int length) {
    FILE* file = fopen(file_path, "rb+");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }

    int center = length / 2;

    for (int i = 0; i < center; i++) {
        int start_offset = i * sizeof(int);
        int end_offset = (length - i - 1) * sizeof(int);

        int start, end;
        fseek(file, start_offset, SEEK_SET);
        fread(&start, sizeof(int), 1, file);
        fseek(file, end_offset, SEEK_SET);
        fread(&end, sizeof(int), 1, file);

        fseek(file, start_offset, SEEK_SET);
        fwrite(&end, sizeof(int), 1, file);
        fseek(file, end_offset, SEEK_SET);
        fwrite(&start, sizeof(int), 1, file);
    }

    fclose(file);
}

void task3(const char* file_path) {
    write_array(file_path);
    int length = get_length(file_path);
    int* array = read_array(file_path);
    printf("Arr from file:  ");
    print_array(array, length);
    modify_array_center(file_path, length);
    free(array);
    array = read_array(file_path);
    printf("Modified array: ");
    print_array(array, length);
    free(array);
}
