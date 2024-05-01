#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "function.h"

void fill_student_name(student* personal_data, int student_count) {
    printf("Profile info data: ");

    personal_data[student_count].name = malloc(50 * sizeof(char));
    personal_data[student_count].surname = malloc(50 * sizeof(char));
    personal_data[student_count].patronymic = malloc(50 * sizeof(char));

    if (personal_data[student_count].name == NULL ||
        personal_data[student_count].surname == NULL ||
        personal_data[student_count].patronymic == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    scanf("%s %s %s", personal_data[student_count].name,
          personal_data[student_count].surname,
          personal_data[student_count].patronymic);

    int length_name = strlen(personal_data[student_count].name);
    int length_surname = strlen(personal_data[student_count].surname);
    int length_patr = strlen(personal_data[student_count].patronymic);

    personal_data[student_count].name = realloc(
        personal_data[student_count].name, (length_name + 1) * sizeof(char));
    personal_data[student_count].surname =
        realloc(personal_data[student_count].surname,
                (length_surname + 1) * sizeof(char));
    personal_data[student_count].patronymic =
        realloc(personal_data[student_count].patronymic,
                (length_patr + 1) * sizeof(char));
}

void fill_student_number(student* personal_data, int student_count) {
    int unique = 0;

    while (!unique) {
        printf("Number: ");
        scanf("%d", &personal_data[student_count].number);

        unique = 1;

        for (int i = 0; i < student_count; i++) {
            if (personal_data[student_count].number ==
                personal_data[i].number) {
                printf("Number is already used, try again\n");
                unique = 0;
                break;
            }
        }
    }
}

void fill_student_disciplines(student* personal_data, int student_count) {
    printf("Number of disciplines: ");
    scanf("%d", &personal_data[student_count].number_of_discipline);

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
            malloc(50 * sizeof(char));
        if (personal_data[student_count].marks[i].discipline == NULL) {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
        scanf("%*c");
        printf("Discipline %d: ", i + 1);
        fgets(personal_data[student_count].marks[i].discipline, 50, stdin);
        int discipline_length =
            strlen(personal_data[student_count].marks[i].discipline);
        personal_data[student_count].marks[i].discipline =
            realloc(personal_data[student_count].marks[i].discipline,
                    (discipline_length + 1) * sizeof(char));

        printf("Mark for %s: ",
               personal_data[student_count].marks[i].discipline);
        scanf("%d", &personal_data[student_count].marks[i].mark);
        if (personal_data[student_count].marks[i].mark < 0 ||
            personal_data[student_count].marks[i].mark > 10) {
            printf("Input not correct, try again\n");
        }
    }
}

void check_last_state(student* personal_data, int student_count) {
    int last_index = personal_data[student_count].number_of_events - 1;
    if (personal_data[student_count].state[last_index].current_state ==
            ENROLLED &&
        personal_data[student_count].number_of_discipline == 0) {
        personal_data[student_count].state[0].listed_or_not = LISTED;
    } else {
        if (personal_data[student_count].state[last_index].current_state !=
            EXPELLED) {
            personal_data[student_count].state[0].listed_or_not = LISTED;
        } else {
            personal_data[student_count].state[0].listed_or_not = NO_LISTED;
        }
    }
}

int is_date_correct(char* date) {
    if (strlen(date) != 10) return 0;

    if (date[2] != '.' || date[5] != '.') return 0;

    for (int i = 6; i < 10; i++) {
        if (date[i] < '0' || date[i] > '9') return 0;
    }

    int day, month, year;
    if (sscanf(date, "%2d.%2d.%4d", &day, &month, &year) != 3) return 0;

    if (day < 1 || day > 31) return 0;

    if (month < 1 || month > 12) return 0;

    if (year < 1964 || year > 2024) return 0;

    return 1;
}

int is_state_duplicate(student* personal_data, int student_count, int i) {
    if (i != 0 && personal_data[student_count].state[i].current_state ==
                      personal_data[student_count].state[i - 1].current_state) {
        return 1;
    }

    return 0;
}

void fill_student_event(student* personal_data, int student_count, int i) {
    if (i == 0) {
        personal_data[student_count].state[i].current_state = ENROLLED;
    } else {
        if (personal_data[student_count].state[i - 1].current_state ==
            EXPELLED) {
            personal_data[student_count].state[i].current_state = RESTORED;
        } else {
            personal_data[student_count].state[i].current_state = EXPELLED;
        }
    }
}

int compare_dates(char* date1, char* date2) {
    int day1, month1, year1;
    int day2, month2, year2;
    sscanf(date1, "%d.%d.%d", &day1, &month1, &year1);
    sscanf(date2, "%d.%d.%d", &day2, &month2, &year2);

    if (year1 != year2)
        return year1 - year2;
    else if (month1 != month2)
        return month1 - month2;
    else
        return day1 - day2;
}

void bubble_sort_dates(student* personal_data, int num_events,
                       int current_index) {
    for (int i = 0; i < num_events - 1; i++) {
        for (int j = 0; j < num_events - i - 1; j++) {
            char* date1 = personal_data[current_index].state[j].date;
            char* date2 = personal_data[current_index].state[j + 1].date;

            if (compare_dates(date1, date2) > 0) {
                event temp = personal_data[current_index].state[j];
                personal_data[current_index].state[j] =
                    personal_data[current_index].state[j + 1];
                personal_data[current_index].state[j + 1] = temp;
            }
        }
    }
}

int is_date_duplicate(event* events, int current_index) {
    for (int i = 0; i < current_index; i++) {
        int day1, month1, year1;
        int day2, month2, year2;
        sscanf(events[current_index].date, "%d.%d.%d", &day1, &month1, &year1);
        sscanf(events[i].date, "%d.%d.%d", &day2, &month2, &year2);

        if (day1 == day2 && month1 == month2 && year1 == year2) {
            return 1;
        }
    }
    return 0;
}

void fill_student_date(student* personal_data, int student_count) {
    printf("Number of events: ");
    scanf("%d", &personal_data[student_count].number_of_events);

    if (personal_data[student_count].number_of_events != 0) {
        personal_data[student_count].state = malloc(
            personal_data[student_count].number_of_events * sizeof(event));
        if (personal_data[student_count].state == NULL) {
            printf("Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < personal_data[student_count].number_of_events;
             i++) {
            printf("Event %d date (dd.mm.yyyy): ", i + 1);
            scanf("%s", personal_data[student_count].state[i].date);

            while (
                !is_date_correct(personal_data[student_count].state[i].date) ||
                is_date_duplicate(personal_data[student_count].state, i)) {
                printf(
                    "Invalid date format or duplicate date. Please enter again "
                    "(dd.mm.yyyy): ");
                scanf("%s", personal_data[student_count].state[i].date);
            }
        }

        bubble_sort_dates(personal_data,
                          personal_data[student_count].number_of_events,
                          student_count);
        for (int i = 0; i < personal_data[student_count].number_of_events;
             i++) {
            fill_student_event(personal_data, student_count, i);
        }
        check_last_state(personal_data, student_count);
    } else {
        printf("no student listed\n");
        exit(1);
    }
}

int fill_data(student** personal_data, int student_count) {
    printf("Enter student details:\n");

    *personal_data =
        realloc(*personal_data, (student_count + 1) * sizeof(student));
    if (*personal_data == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    fill_student_name(*personal_data, student_count);
    fill_student_number(*personal_data, student_count);
    fill_student_disciplines(*personal_data, student_count);
    fill_student_date(*personal_data, student_count);

    student_count++;
    return student_count;
}
