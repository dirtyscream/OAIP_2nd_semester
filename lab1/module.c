#include <stdio.h>
#include <stdlib.h>

int get_length(const char* file_path) {
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    int length = size / sizeof(int);
    fclose(file);
    return length;
}

void print_array(int* array, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void write_array(const char* file_path) {
    int length;
    printf("Enter the size of array: ");
    scanf("%d", &length);
    int* array = (int*)malloc(sizeof(int) * length);
    for (int i = 0; i < length; i++) {
        printf("Enter element %d: ", i);
        scanf("%d", &array[i]);
    }
    FILE* file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }
    fwrite(array, sizeof(int), length, file);
    fclose(file);
    free(array);
}

int* read_array(const char* file_path) {
    int length = get_length(file_path);
    int* array;
    FILE* file = fopen(file_path, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(-1);
    }
    array = (int*)malloc(sizeof(int) * length);
    fread(array, sizeof(int), length, file);
    fclose(file);
    return array;
}
