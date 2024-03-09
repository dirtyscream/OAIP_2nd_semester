#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file(char* file_name, char* mode) {
    FILE* file = fopen(file_name, mode);
    if (!file) {
        printf("File opening error!\n");
        exit(-1);
    }
    return file;
}

char* algorithm_1(FILE* file) {
    char line[100];
    char* min_word = NULL;
    int min_length = INT_MAX;
    while (fgets(line, sizeof(line), file) != NULL) {
        char* word = strtok(line, " \t\n");
        while (word != NULL) {
            int length = strlen(word);
            if (length < min_length) {
                min_length = length;
                if (min_word != NULL) {
                    free(min_word);
                }
                min_word = malloc(length + 1);
                strcpy(min_word, word);
            }
            word = strtok(NULL, " \t\n");
        }
    }
    return min_word;
}

void task1(char* file_path) {
    FILE* file = open_file(file_path, "r");
    char* result = algorithm_1(file);
    if (result == NULL) {
        printf("File is empty\n");
        return;
    }
    fclose(file);
    printf("Word with the minimum length is '%s'\n", result);
}

void delete_words(FILE* main_file, FILE* temp_file, char symbol) {
    char word[100];
    int i = 0;
    int c;
    while ((c = fgetc(main_file)) != EOF) {
        if (c == ' ' || c == '\n') {
            word[i] = '\0';
            if (word[0] != symbol) {
                fprintf(temp_file, "%s ", word);
            }
            i = 0;
            if (c == '\n') {
                fprintf(temp_file, "\n");
            }
        } else {
            word[i] = c;
            i++;
        }
    }
    if (i > 0) {
        word[i] = '\0';
        if (word[0] != symbol) {
            fprintf(temp_file, "%s", word);
        }
    }
}
void task2(char* file_path) {
    FILE* main_file = open_file(file_path, "r+");
    FILE* temp_file = fopen("temp.txt", "r");
    if (temp_file != NULL) {
        printf(
            "File temp.txt already exists. Please delete, rename or remove it "
            "to another diretory\n");
        exit(-1);
    }
    temp_file = open_file("temp.txt", "w");
    char symbol;
    printf("Enter symbol: ");
    scanf(" %c", &symbol);

    delete_words(main_file, temp_file, symbol);

    if (remove(file_path) != 0) {
        printf("Error\n");
        exit(-1);
    }
    if (rename("temp.txt", file_path) != 0) {
        printf("Error\n");
        exit(-1);
    }
    printf("Deleted successfully. Check the file\n");
    fclose(main_file);
}
