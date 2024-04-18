#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "function.h"

double average_mark(student* personal_data) {
    double average = 0;
    for (int j = 0; j < personal_data->number_of_discipline; j++) {
        average += (double)personal_data->marks[j].mark;
    }
    return (average / personal_data->number_of_discipline);
}

void add_event(student* personal_data, int current_index, int last_index) {
    personal_data[current_index].state[0].listed_or_not = LISTED;

    personal_data[current_index].number_of_events++;

    personal_data[current_index].state =
        realloc(personal_data[current_index].state,
                personal_data[current_index].number_of_events * sizeof(event));
    if (personal_data[current_index].state == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    time_t mytime = time(NULL);
    struct tm* now = localtime(&mytime);
    strftime(personal_data[current_index].state[last_index + 1].date, 11,
             "%d.%m.%Y", now);
}

int restore_students(student* personal_data, int student_count) {
    for (int i = 0; i < student_count; i++) {
        int last_index = personal_data[i].number_of_events - 1;
        double avg = average_mark(&personal_data[i]);

        if (personal_data[i].state[last_index].current_state == EXPELLED) {
            if (avg > 7 && personal_data[i].number_of_discipline != 0) {
                add_event(personal_data, i, last_index);

                personal_data[i].state[last_index + 1].current_state = RESTORED;

            } else {
                personal_data[i].state[last_index + 1].listed_or_not =
                    NO_LISTED;
            }
        }
    }
    return student_count;
}

int delete_lowperforming_students(student* personal_data, int student_count) {
    for (int i = 0; i < student_count; i++) {
        int last_index = personal_data[i].number_of_events - 1;
        double avg = average_mark(&personal_data[i]);
        if (personal_data[i].state[last_index].current_state != EXPELLED) {
            if (avg < 5) {
                student_count = delete_data(personal_data, student_count, i);
                i--;
            } else {
                personal_data[i].state[last_index + 1].listed_or_not = LISTED;
            }
        }
    }
    return student_count;
}

int get_valid_index(int size) {
    int index;
    while (1) {
        printf("Enter the index of the student to delete: ");
        scanf("%d", &index);
        if (index < 0 || index >= size) {
            printf("Invalid index. Please try again.\n");
        } else {
            break;
        }
    }
    return index;
}

int delete_student_interface(student* personal_data, int size) {
    int index = get_valid_index(size);
    size = delete_data(personal_data, size, index);
    return size;
}

int task2(char* file_name) {
    int index;
    int flag = 0;
    int vote;

    int size = 0;
    int choice;

    student* personal_data = read_data(file_name, &size);
    while (1) {
        printf(
            "Select the task:\n1)check info (delete or restore)\n2)delete "
            "student\n3)add information\n4)show info\n5)free data\n6)"
            "save changes\n7)exit program\n");
        scanf("%d", &choice);

        if (choice < 0 || choice > 8) {
            printf("Input not correct, try again\n");
        } else {
            switch (choice) {
                case 1: {
                    size = restore_students(personal_data, size);
                    size = delete_lowperforming_students(personal_data, size);
                    flag = 1;
                    break;
                }
                case 2:
                    size = delete_student_interface(personal_data, size);
                    flag = 1;
                    break;

                case 3:
                    size = fill_data(&personal_data, size);
                    flag = 1;
                    break;

                case 4:
                    printf_file(personal_data, size);
                    flag = 1;
                    break;

                case 5:
                    free_student_data(personal_data, size);
                    size = 0;
                    break;
                case 6:
                    save_data(personal_data, size, file_name);
                    flag = 0;
                    break;
                case 7:
                    if (flag == 1) {
                        int vote;
                        printf(
                            "Are you sure that you want to quit the "
                            "program?\nIf you want to save the changes, enter "
                            "0\n");
                        scanf("%d", &vote);
                        if (vote == 0) {
                            save_data(personal_data, size, file_name);
                        }
                    }
                default:
                    exit(1);
            }
        }
    }
    free_student_data(personal_data, size);
    return 0;
}
