#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

int pk = 0;

typedef union Bits {
    struct {
        unsigned int last_bit : 1;
        unsigned int next_bit : 1;
    };
    unsigned int value;
    void (*find_bits)(union Bits* object, int number);
    void (*print_bits)(union Bits* object, int number);
} Bits;

void find_bits(Bits* object, int number) {
    uint8_t last_bit = 0;
    uint8_t next_bit = 0;
    int size = (int)(log2(number) + 1);

    for (int i = 0; i < size; i++) {
        next_bit = last_bit;
        last_bit = number & 1;
        number >>= 1;
    }
    object->last_bit = last_bit;
    object->next_bit = next_bit;
    return;
}

void print_bits(Bits* object, int num) {
    printf("Number in binary system is: ");
    printf("Last: %u, next: %u\n", object->last_bit, object->next_bit);
}

void task1() {
    int number;
    printf("Enter number: ");
    scanf("%d", &number);
    Bits object;
    object.find_bits = find_bits;
    object.print_bits = print_bits;
    object.find_bits(&object, number);
    object.print_bits(&object, number);
}

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef union Bits {
    struct {
        unsigned int last_bit : 1;
        unsigned int next_bit : 1;
    };
    unsigned int value;
    void (*find_bits)(union Bits* object, int number);
    void (*print_bits)(union Bits* object, int number);
} Bits;

void find_bits(Bits* object, int number) {
    uint8_t last_bit = 0;
    uint8_t next_bit = 0;
    int size = (int)(log2(number) + 1);

    for (int i = 0; i < size; i++) {
        next_bit = last_bit;
        last_bit = number & 1;
        number >>= 1;
    }
    object->last_bit = last_bit;
    object->next_bit = next_bit;
    return;
}

void print_bits(Bits* object, int num) {
    printf("Number in binary system is: ");
    printf("Last: %u, next: %u\n", object->last_bit, object->next_bit);
}

void task1() {
    int number;
    printf("Enter number: ");
    scanf("%d", &number);
    Bits object;
    object.find_bits = find_bits;
    object.print_bits = print_bits;
    object.find_bits(&object, number);
    object.print_bits(&object, number);
}

typedef struct Rating {
    int mark;
    char* subject;
} Rating;

typedef struct Event {
    char* event;
    char* date;
} Event;

typedef struct StudentInfo {
    int identifier;
    char* full_name;
    Rating* marks;
    Event* events;
    char* status;
    void (*input_student)(struct StudentInfo* student, int size);
    void (*delete_student)(struct StudentInfo* student, int index);
} StudentInfo;

void delete_student(struct StudentInfo* array, int* size, int index) {
    if (index < 0 || index >= *size) {
        printf("Invalid index\n");
        return;
    }
    for (int i = index; i < *size - 1; i++) {
        array[i] = array[i + 1];
    }
    (*size)--;
    array = (StudentInfo*)realloc(array, *size * sizeof(StudentInfo));
}

Rating* input_rating(int size) {
    Rating* marks = (Rating*)malloc(size * sizeof(Rating));
    for (int i = 0; i < size; i++) {
        printf("Enter grade for mark %d: ", i + 1);
        scanf("%d", &(marks[i].mark));
        marks[i].subject = (char*)malloc(100 * sizeof(char));
        printf("Enter discipline for mark %d: ", i + 1);
        scanf("%s", marks[i].subject);
    }
    return marks;
}

int validate_date(const char* date) {
    if (strlen(date) != 10) return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (date[i] != '.') return 0;
        } else {
            if (!isdigit(date[i])) return 0;
        }
    }
    return 1;
}

int compare_date(const char* date1, const char* date2) {
    int day1, month1, year1;
    int day2, month2, year2;
    sscanf(date1, "%d.%d.%d", &day1, &month1, &year1);
    sscanf(date2, "%d.%d.%d", &day2, &month2, &year2);
    if (year1 != year2) return year2 - year1;
    if (month1 != month2) return month2 - month1;
    if (day1 != day2) return day2 - day1;

    return 0;
}

void sort_events(Event* events, int size) {
    for (int i = 1; i < size; i++) {
        Event key = events[i];
        int j = i - 1;
        while (j >= 0 && compare_date(events[j].date, key.date) < 0) {
            events[j + 1] = events[j];
            j--;
        }
        events[j + 1] = key;
    }
}

Event* input_events(int size) {
    Event* events = (Event*)malloc(size * sizeof(Event));
    for (int i = 0; i < size; i++) {
        events[i].event = (char*)malloc(100 * sizeof(char));
        events[i].date = (char*)malloc(100 * sizeof(char));
        events[i].event = NULL;
        events[i].date = NULL;
    }
    for (int i = 0; i < size; i++) {
        printf("Enter event %d: ", i + 1);
        events[i].event = (char*)malloc(100 * sizeof(char));
        scanf("%s", events[i].event);
        printf("Enter date (dd.mm.yyyy) for event %d: ", i + 1);
        events[i].date = (char*)malloc(11 * sizeof(char));
        scanf("%s", events[i].date);
        while (!validate_date(events[i].date)) {
            printf("Invalid date. Enter date (dd.mm.yyyy) for event %d: ",
                   i + 1);
            scanf("%s", events[i].date);
        }
    }
    sort_events(events, size);
    return events;
}

void input_student(struct StudentInfo* student) {
    printf("Enter identifier: ");
    scanf("%d", &(student->identifier));
    student->full_name = (char*)malloc(100 * sizeof(char));
    printf("Enter full name: ");
    scanf("%s", student->full_name);

    student->marks = input_rating(size_rate);
    student->events = input_events(size_events);
    student->status = (char*)malloc(100 * sizeof(char));
    printf("Enter status: ");
    scanf("%s", student->status);
}

void restore_student(struct StudentInfo* student) {
    student->events = (Event*)malloc(1 * sizeof(Event));
    student->events[0].event = (char*)malloc(100 * sizeof(char));
    student->events[0].date = (char*)malloc(11 * sizeof(char));
    strcpy(student->events[0].event, "Восстановление");
    time_t raw_time;
    struct tm* local_time;
    time(&raw_time);
    local_time = localtime(&raw_time);
    strftime(student->events[0].date, 11, "%d.%m.%Y", local_time);
}

void task2() {
    struct StudentInfo student;
    int size_rate;
    printf("Enter number of ratings: ");
    scanf("%d", &size_rate);
    int size_events;
    printf("Enter number of events: ");
    scanf("%d", &size_events);
    student.input_student = input_student;
    student.delete_student = delete_student;
    student.input_student(&student);
    printf("Identifier: %d\n", student.identifier);
    printf("Full name: %s\n", student.full_name);
    printf("Ratings:\n");
    for (int i = 0; i < 2; i++) {
        printf("Subject: %s\n", student.marks[i].subject);
        printf("Mark: %d\n", student.marks[i].mark);
    }
    printf("Events:\n");
    for (int i = 0; i < 2; i++) {
        printf("Event: %s\n", student.events[i].event);
        printf("Date: %s\n", student.events[i].date);
    }
    printf("Status: %s\n", student.status);
    student.delete_student(&student, 0);
    restore_student(&student);
    printf("Restored student:\n");
    printf("Event: %s\n", student.events[0].event);
    printf("Date: %s\n", student.events[0].date);

    return 0;
}

int main() {
    int mode = 1;

    while (mode) {
        printf("Wanna continue: (1 to continue; 0 to exit)\n");
        scanf("%d", &mode);
        if (mode == 0 || mode != 1) {
            break;
        }
        printf("Task 1 (enter 1)\n");
        printf("Task 2 (enter 2)\n");
        int task;
        scanf("%d", &task);
        switch (task) {
            case 1:
                task1();
                continue;
            case 2:
                task2();
            default:
                printf("Invalid task\n");
                return 0;
        }
    }
    return 0;
}