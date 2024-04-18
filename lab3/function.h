#include <stdio.h>
#include <stdlib.h>

typedef enum {
    ENROLLED, 
    EXPELLED,   
    RESTORED,    
} current_state;

typedef enum {
    LISTED,
    NO_LISTED
} listed_or_not;

typedef struct {
    int mark;
    char* discipline;
} marks_and_discipline;

typedef struct {
    char date[11];
    current_state current_state;
    listed_or_not listed_or_not;
} event;

typedef struct {
    char* name;
    char* surname;
    char* patronymic;
    int number;
    int number_of_discipline;
    int number_of_events;
    marks_and_discipline* marks;
    event* state;
} student;

student* read_data(char* file_name, int* student_count);
void printf_file(student* personal_data, int n);
void save_data(student* personal_data, int student_count, char* file_name);
void free_student_data(student* personal_data, int student_count);
int delete_data(student* personal_data, int student_count, int index);
void check_last_state(student* personal_data, int student_count);
int fill_data(student** personal_data, int student_count);
int task2(char* file_name);
int task1();
