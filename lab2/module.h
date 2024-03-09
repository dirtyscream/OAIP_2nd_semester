#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file(char* file_name, char* mode);

char* algorithm_1(FILE* file);
void task1(char* file_path);

void delete_words(FILE* main_file, FILE* temp_file, char symbol);

void task2(char* file_path);
