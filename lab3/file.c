#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function.h"

FILE* open_file(char* file_name, char* mode) {
    FILE* file_ptr = fopen(file_name, mode);
    if (!file_ptr) {
        printf("\nfile opening error!\n");
        exit(EXIT_FAILURE);
    }
    return file_ptr;
}

void save_data(student* personal_data, int student_count, char* file_name) {
    FILE* pt = fopen(file_name, "w");

    fprintf(pt, "%d\n", student_count);

    for (int i = 0; i < student_count; i++) {
        fprintf(pt, "%s %s %s ", personal_data[i].name,
                personal_data[i].surname, personal_data[i].patronymic);
        fprintf(pt, "%d ", personal_data[i].number);
        fprintf(pt, "%d ", personal_data[i].number_of_discipline);
        for (int j = 0; j < personal_data[i].number_of_discipline; j++) {
            fprintf(pt, "%s - %d ", personal_data[i].marks[j].discipline,
                    personal_data[i].marks[j].mark);
        }
        fprintf(pt, "%d ", personal_data[i].number_of_events);
        for (int j = 0; j < personal_data[i].number_of_events; j++) {
            fprintf(pt, "%s %u ", personal_data[i].state[j].date,
                    personal_data[i].state[j].current_state);
        }
        fprintf(pt, "%u\n", personal_data[i].state[0].listed_or_not);
    }

    fclose(pt);
}

void read_student_info(FILE* pt, student* personal_data, int student_count) {
    personal_data[student_count].name = malloc(100 * sizeof(char));
    personal_data[student_count].surname = malloc(100 * sizeof(char));
    personal_data[student_count].patronymic = malloc(100 * sizeof(char));
    if (personal_data[student_count].name == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    if (fscanf(pt, "%s %s %s ", personal_data[student_count].name,
               personal_data[student_count].surname,
               personal_data[student_count].patronymic) != 3) {
        printf("Error reading name.\n");
        exit(EXIT_FAILURE);
    }
    printf("[%s]\n", personal_data[student_count].name);
}

void read_student_number(FILE* pt, student* personal_data, int student_count) {
    if (fscanf(pt, "%d ", &personal_data[student_count].number) != 1) {
        printf("Error reading number.\n");
        exit(EXIT_FAILURE);
    }
}

void read_student_marks(FILE* pt, student* personal_data, int student_count) {
    if (fscanf(pt, "%d ", &personal_data[student_count].number_of_discipline) !=
        1) {
        printf("Error reading number of disciplines.\n");
        exit(EXIT_FAILURE);
    }

    personal_data[student_count].marks =
        malloc(personal_data[student_count].number_of_discipline *
               sizeof(marks_and_discipline));
    if (personal_data[student_count].marks == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < personal_data[student_count].number_of_discipline;
         i++) {
        personal_data[student_count].marks[i].discipline =
            malloc(25 * sizeof(char));
        if (personal_data[student_count].marks[i].discipline == NULL) {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
        if (fscanf(pt, "%s - %d ",
                   personal_data[student_count].marks[i].discipline,
                   &personal_data[student_count].marks[i].mark) != 2) {
            printf("Error reading discipline and mark.\n");
            exit(EXIT_FAILURE);
        }
    }
}

void read_student_events(FILE* pt, student* personal_data, int student_count) {
    if (fscanf(pt, "%d ", &personal_data[student_count].number_of_events) !=
        1) {
        printf("Error reading number of events.\n");
        exit(EXIT_FAILURE);
    }
    personal_data[student_count].state =
        malloc(personal_data[student_count].number_of_events * sizeof(event));
    if (personal_data[student_count].state == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < personal_data[student_count].number_of_events; i++) {
        if (fscanf(pt, "%s %u ", personal_data[student_count].state[i].date,
                   &personal_data[student_count].state[i].current_state) != 2) {
            printf("Error reading date and current_state.\n");
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < 1; i++) {
        if (fscanf(pt, "%u ",
                   &personal_data[student_count].state[i].listed_or_not) != 1) {
            printf("Error reading date and listed_or_not.\n");
            exit(EXIT_FAILURE);
        }
    }
}

student* read_data(char* file_name, int* student_count) {
    FILE* pt = open_file(file_name, "r");

    fscanf(pt, "%d", student_count);
    student* personal_data = malloc((*student_count) * sizeof(student));
    if (personal_data == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (*student_count); i++) {
        read_student_info(pt, personal_data, i);

        read_student_number(pt, personal_data, i);

        read_student_marks(pt, personal_data, i);
        read_student_events(pt, personal_data, i);
    }

    fclose(pt);
    return personal_data;
}

void printf_file(student* personal_data, int n) {
    char check_state[2][10] = {"LISTED", "NO_LISTED"};
    char eve[4][9] = {"ENROLLED", "EXPELLED", "RESTORED"};
    for (int i = 0; i < n; i++) {
        printf("Profile: %s %s %s\n", personal_data[i].name,
               personal_data[i].surname, personal_data[i].patronymic);
        printf("Number: %d\n", personal_data[i].number);
        for (int j = 0; j < personal_data[i].number_of_discipline; j++) {
            printf("Discipline: %s - %d\n",
                   personal_data[i].marks[j].discipline,
                   personal_data[i].marks[j].mark);
        }
        for (int j = 0; j < personal_data[i].number_of_events; j++) {
            printf("Event %d date %s state: %s\n", j + 1,
                   personal_data[i].state[j].date,
                   eve[personal_data[i].state[j].current_state]);
        }

        printf("Current state: %s\n",
               check_state[personal_data[i].state[0].listed_or_not]);
        printf("****************************************\n");
    }
}

void free_student_data(student* personal_data, int student_count) {
    if (personal_data == NULL) return;

    for (int i = 0; i < student_count; i++) {
        free(personal_data[i].name);
        free(personal_data[i].surname);
        free(personal_data[i].patronymic);

        for (int j = 0; j < personal_data[i].number_of_discipline; j++) {
            free(personal_data[i].marks[j].discipline);
        }

        free(personal_data[i].marks);

        free(personal_data[i].state);
    }

    free(personal_data);
}

int delete_data(student* personal_data, int student_count, int index) {
    student* temp;

    free(personal_data[index].name);
    free(personal_data[index].surname);
    free(personal_data[index].patronymic);

    for (int i = 0; i < personal_data[index].number_of_discipline; i++) {
        free(personal_data[index].marks[i].discipline);
    }

    free(personal_data[index].marks);

    free(personal_data[index].state);

    for (int i = index; i < student_count - 1; i++) {
        personal_data[i] = personal_data[i + 1];
    }

    student_count--;

    temp = realloc(personal_data, student_count * sizeof(student));
    if (temp) {
        personal_data = temp;
    } else {
        printf("\nmemory allocation error\n");
    }

    return student_count;
}
