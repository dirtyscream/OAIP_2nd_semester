#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 1000
#define MAX_SUBJECT_LENGTH 50
#define MAX_EVENTS 10

typedef enum { ENROLLED, EXPELLED, RESTORED } EventType;

typedef struct {
    int mark;
    char *subject;
} Marks;

typedef struct {
    EventType type;
    char date[11];
} Events;

typedef struct {
    int id;
    char *full_name;
    Marks *marks;
    int num_marks;
    Events events[MAX_EVENTS];
    int num_events;
} Student;

typedef struct {
    int key;
    Student *data;
} Hash_table;

typedef struct Subject_node {
    char *subject;
    Student *student;
    struct Subject_node *next;
} Subject_node;

typedef struct {
    Subject_node *table[MAX_STUDENTS];
} SubjectHashTable;

int hash_function(int key) {
    int hash = key % MAX_STUDENTS;
    return hash;
}

int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % 101;
}

void add_student_to_subject_table(Student *student,
                                  SubjectHashTable *subjectHashTable) {
    for (int i = 0; i < student->num_marks; ++i) {
        int index = hash(student->marks[i].subject);
        Subject_node *new_node = (Subject_node *)malloc(sizeof(Subject_node));
        new_node->subject = student->marks[i].subject;
        new_node->student = student;
        new_node->next = subjectHashTable->table[index];
        subjectHashTable->table[index] = new_node;
    }
}

void add_student(int key, Student *student, Hash_table *HashTable) {
    int hash_index = hash_function(key);
    while (HashTable[hash_index].data != NULL) {
        hash_index = (hash_index + 1) % MAX_STUDENTS;
    }
    HashTable[hash_index].key = key;
    HashTable[hash_index].data = student;
}

Student *create_student(int id, const char *full_name) {
    Student *student = (Student *)malloc(sizeof(Student));
    if (student == NULL) {
        fprintf(stderr, "Memory allocation failed for student.\n");
        return NULL;
    }
    student->id = id;
    student->full_name = strdup(full_name);
    if (student->full_name == NULL) {
        free(student);
        fprintf(stderr, "Memory allocation failed for full_name.\n");
        return NULL;
    }
    student->marks = NULL;
    student->num_marks = 0;
    student->num_events = 0;
    return student;
}

void add_student_from_input(Hash_table *HashTable) {
    int id;
    char full_name[100];
    int marks_count;
    int events_count;

    printf("Enter student ID: ");
    scanf("%d", &id);
    getchar();

    printf("Enter student full name: ");
    fgets(full_name, sizeof(full_name), stdin);
    full_name[strcspn(full_name, "\n")] = '\0';

    printf("Enter number of marks: ");
    scanf("%d", &marks_count);

    Marks *marks = malloc(marks_count * sizeof(Marks));
    for (int i = 0; i < marks_count; i++) {
        marks[i].subject = malloc(100 * sizeof(char));
        printf("Enter mark %d and subject: ", i + 1);
        scanf("%d", &marks[i].mark);
        getchar();
        fgets(marks[i].subject, 100, stdin);
        marks[i].subject[strcspn(marks[i].subject, "\n")] = '\0';
    }
    printf("Enter number of events: ");
    scanf("%d", &events_count);
    Student *new_student = create_student(id, full_name);
    new_student->marks = marks;
    new_student->num_marks = marks_count;
    new_student->num_events = events_count;
    for (int i = 0; i < events_count; i++) {
        char event_type[20];
        printf(
            "Enter event type (ENROLLED/EXPELLED/RESTORED) and date "
            "(yyyy-mm-dd): ");
        scanf("%s %s", event_type, new_student->events[i].date);

        if (strcmp(event_type, "ENROLLED") == 0) {
            new_student->events[i].type = ENROLLED;
        } else if (strcmp(event_type, "EXPELLED") == 0) {
            new_student->events[i].type = EXPELLED;
        } else if (strcmp(event_type, "RESTORED") == 0) {
            new_student->events[i].type = RESTORED;
        } else {
            printf("Invalid event type. Please try again.\n");
            i--;
        }
    }

    add_student(new_student->id, new_student, HashTable);
}

Student *find_student(int key, Hash_table *HashTable) {
    int hash_index = hash_function(key);
    while (HashTable[hash_index].data != NULL) {
        if (HashTable[hash_index].key == key) {
            return HashTable[hash_index].data;
        }
        hash_index = (hash_index + 1) % MAX_STUDENTS;
    }
    return NULL;
}

void restore_students(Hash_table *HashTable) {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (HashTable[i].data != NULL) {
            Student *student = HashTable[i].data;

            int sum_marks = 0;
            for (int j = 0; j < student->num_marks; j++) {
                sum_marks += student->marks[j].mark;
            }
            double average = (double)sum_marks / student->num_marks;

            if (student->events[student->num_events - 1].type == EXPELLED &&
                average > 7.0) {
                student->events[student->num_events - 1].type = RESTORED;
                printf("Restoring student %s with average mark %.2f\n",
                       student->full_name, average);
            }
        }
    }
}

void remove_from_subjectHashTable(Student *student,
                                  SubjectHashTable *subjectHashTable) {
    for (int i = 0; i < student->num_marks; ++i) {
        int subject_index = hash(student->marks[i].subject);
        Subject_node *current = subjectHashTable->table[subject_index];
        Subject_node *prev = NULL;
        while (current != NULL) {
            if (current->student == student) {
                if (prev == NULL) {
                    subjectHashTable->table[subject_index] = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current->subject);
                free(current);
                break;
            }
            prev = current;
            current = current->next;
        }
    }
}

void remove_student(int key, Hash_table *HashTable,
                    SubjectHashTable *subjectHashTable) {
    int hash_index = hash_function(key);
    while (HashTable[hash_index].data != NULL) {
        if (HashTable[hash_index].key == key) {
            remove_from_subjectHashTable(HashTable[hash_index].data,
                                         subjectHashTable);
            free(HashTable[hash_index].data->full_name);
            for (int i = 0; i < HashTable[hash_index].data->num_marks; ++i) {
                free(HashTable[hash_index].data->marks[i].subject);
            }
            free(HashTable[hash_index].data->marks);
            free(HashTable[hash_index].data);
            HashTable[hash_index].data = NULL;
            return;
        }
        hash_index = (hash_index + 1) % MAX_STUDENTS;
    }
}

void remove_underperforming_students(Hash_table *HashTable,
                                     SubjectHashTable *subjectHashTable) {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (HashTable[i].data != NULL) {
            int sum = 0;
            for (int j = 0; j < HashTable[i].data->num_marks; ++j) {
                sum += HashTable[i].data->marks[j].mark;
            }
            float average = (float)sum / HashTable[i].data->num_marks;
            if (average < 5.0) {
                remove_student(HashTable[i].key, HashTable, subjectHashTable);
            }
        }
    }
}

void find_students_by_subject(const char *subject,
                              SubjectHashTable *subjectHashTable) {
    int index = hash(subject);
    Subject_node *node = subjectHashTable->table[index];
    while (node) {
        if (strcmp(node->subject, subject) == 0) {
            printf("Student ID: %d, Name: %s\n", node->student->id,
                   node->student->full_name);
        }
        node = node->next;
    }
}

EventType string_to_event_type(const char *str) {
    if (strcmp(str, "ENROLLED") == 0) {
        return ENROLLED;
    } else if (strcmp(str, "EXPELLED") == 0) {
        return EXPELLED;
    } else if (strcmp(str, "RESTORED") == 0) {
        return RESTORED;
    } else {
        return ENROLLED;
    }
}

typedef struct Hash_data {
    Hash_table *HashTable;
    SubjectHashTable *subjectHashTable;
} Hash_data;

int read_students_callback(void *data, int argc, char **argv,
                           char **col_names) {
    Hash_data *students_data = (Hash_data *)data;
    Hash_table *HashTable = students_data->HashTable;
    SubjectHashTable *subjectHashTable = students_data->subjectHashTable;
    int id = atoi(argv[0]);
    const char *full_name = argv[1];
    Student *student = find_student(id, HashTable);
    if (student == NULL) {
        student = create_student(id, full_name);
        add_student(id, student, HashTable);
    }

    if (argv[2] != NULL && argv[3] != NULL) {
        int mark = atoi(argv[2]);
        const char *subject = argv[3];

        int mark_exists = 0;
        for (int i = 0; i < student->num_marks; i++) {
            if (student->marks[i].mark == mark &&
                strcmp(student->marks[i].subject, subject) == 0) {
                mark_exists = 1;
                break;
            }
        }

        if (!mark_exists) {
            if (student->marks == NULL) {
                student->marks = (Marks *)malloc(MAX_EVENTS * sizeof(Marks));
            } else {
                student->marks = (Marks *)realloc(
                    student->marks, (student->num_marks + 1) * sizeof(Marks));
            }
            student->marks[student->num_marks].mark = mark;
            student->marks[student->num_marks].subject = strdup(subject);
            student->num_marks++;
            int index = hash(subject);
            Subject_node *node = subjectHashTable->table[index];
            while (node) {
                if (strcmp(node->subject, subject) == 0) {
                    Subject_node *current = node;
                    while (current) {
                        if (current->student->id == student->id) {
                            return 0;
                        }
                        current = current->next;
                    }
                    Subject_node *new_node = malloc(sizeof(Subject_node));
                    new_node->subject = strdup(subject);
                    new_node->student = student;
                    new_node->next = node;
                    subjectHashTable->table[index] = new_node;
                    return 0;
                }
                node = node->next;
            }
            Subject_node *new_node = malloc(sizeof(Subject_node));
            new_node->subject = strdup(subject);
            new_node->student = student;
            new_node->next = subjectHashTable->table[index];
            subjectHashTable->table[index] = new_node;
        }
    }

    if (argv[4] != NULL && argv[5] != NULL) {
        EventType type = string_to_event_type(argv[4]);
        const char *date = argv[5];

        int event_exists = 0;
        for (int i = 0; i < student->num_events; i++) {
            if (student->events[i].type == type &&
                strcmp(student->events[i].date, date) == 0) {
                event_exists = 1;
                break;
            }
        }

        if (!event_exists) {
            student->events[student->num_events].type = type;
            strncpy(student->events[student->num_events].date, date,
                    sizeof(student->events[student->num_events].date) - 1);
            student->events[student->num_events]
                .date[sizeof(student->events[student->num_events].date) - 1] =
                '\0';
            student->num_events++;
        }
    }

    return 0;
}

int read_students(sqlite3 *db, Hash_data *data) {
    char *err_msg = NULL;
    const char *sql =
        "SELECT students.id, students.full_name, "
        "marks.mark, marks.subject, "
        "events.type, events.date "
        "FROM students "
        "LEFT JOIN marks ON students.id = marks.student_id "
        "LEFT JOIN events ON students.id = events.student_id;";

    int rc = sqlite3_exec(db, sql, read_students_callback, data, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select students: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    return SQLITE_OK;
}

int save_to_db(sqlite3 *db, Hash_table *HashTable) {
    char *err_msg = NULL;
    char *sql_delete_students = "DELETE FROM students;";
    int rc = sqlite3_exec(db, sql_delete_students, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    char *sql_delete_marks = "DELETE FROM marks;";
    rc = sqlite3_exec(db, sql_delete_marks, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    char *sql_delete_events = "DELETE FROM events;";
    rc = sqlite3_exec(db, sql_delete_events, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (HashTable[i].data != NULL) {
            char sql_insert_student[200];
            sprintf(sql_insert_student,
                    "INSERT INTO students (id, full_name) "
                    "VALUES (%d, '%s');",
                    HashTable[i].data->id, HashTable[i].data->full_name);
            rc = sqlite3_exec(db, sql_insert_student, NULL, 0, &err_msg);
            if (rc != SQLITE_OK) {
                fprintf(stderr, "SQL error: %s\n", err_msg);
                sqlite3_free(err_msg);
                return rc;
            }

            for (int j = 0; j < HashTable[i].data->num_marks; ++j) {
                char sql_insert_mark[200];
                sprintf(sql_insert_mark,
                        "INSERT INTO marks (student_id, mark, subject) VALUES "
                        "(%d, %d, '%s');",
                        HashTable[i].data->id, HashTable[i].data->marks[j].mark,
                        HashTable[i].data->marks[j].subject);
                rc = sqlite3_exec(db, sql_insert_mark, NULL, 0, &err_msg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", err_msg);
                    sqlite3_free(err_msg);
                    return rc;
                }
            }

            for (int k = 0; k < HashTable[i].data->num_events; ++k) {
                char sql_insert_event[200];
                sprintf(sql_insert_event,
                        "INSERT INTO events (student_id, type, date) VALUES "
                        "(%d, %d, '%s');",
                        HashTable[i].data->id,
                        HashTable[i].data->events[k].type,
                        HashTable[i].data->events[k].date);
                rc = sqlite3_exec(db, sql_insert_event, NULL, 0, &err_msg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", err_msg);
                    sqlite3_free(err_msg);
                    return rc;
                }
            }
        }
    }

    return SQLITE_OK;
}

void print_all_students(Hash_table *HashTable) {
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (HashTable[i].data != NULL) {
            Student *student = HashTable[i].data;

            printf("Student ID: %d, Name: %s\n", student->id,
                   student->full_name);

            printf("%d marks:\n", student->num_marks);
            for (int j = 0; j < student->num_marks; j++) {
                printf("%s: %d\n", student->marks[j].subject,
                       student->marks[j].mark);
            }

            printf("%d events:\n", student->num_events);
            for (int k = 0; k < student->num_events; k++) {
                printf("Type: %d, Date: %s\n", student->events[k].type,
                       student->events[k].date);
            }

            printf("\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Argument error\n");
        return 1;
    }
    char *db_name = argv[1];
    Hash_table HashTable[MAX_STUDENTS];
    SubjectHashTable subjectHashTable;

    Hash_data data = {.HashTable = HashTable,
                      .subjectHashTable = &subjectHashTable};

    for (int i = 0; i < MAX_STUDENTS; ++i) {
        HashTable[i].key = -1;
        HashTable[i].data = NULL;
    }

    int choice;
    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    int id;

    do {
        printf("\nMenu:\n");
        printf("1. Print all students\n");
        printf("2. Find student by subject\n");
        printf("3. Restore expelled students with average mark > 7\n");
        printf("4. Delete students with average mark < 5\n");
        printf("5. Add a new student\n");
        printf("6. Save changes to database\n");
        printf("7. Read data from database\n");
        printf("8. Delete student by index\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                print_all_students(HashTable);
                break;
            case 2: {
                char subject[MAX_SUBJECT_LENGTH];
                printf("Enter subject: ");
                scanf("%s", subject);
                find_students_by_subject(subject, &subjectHashTable);
                break;
            }
            case 3:
                restore_students(HashTable);
                printf(
                    "Expelled students with average mark > 7 have been "
                    "restored.\n");
                break;
            case 4:
                remove_underperforming_students(HashTable, &subjectHashTable);
                printf("Students with average mark < 5 have been deleted.\n");
                break;
            case 5:
                add_student_from_input(HashTable);
                break;
            case 6:
                rc = save_to_db(db, HashTable);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed to save changes to database\n");
                } else {
                    printf("Changes saved to database successfully\n");
                }
                break;
            case 7:
                rc = read_students(db, &data);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed to read data from database\n");
                } else {
                    printf("Data read from database successfully\n");
                }
                break;
            case 8:
                printf("Enter student id to delete: ");
                scanf("%d", &id);
                remove_student(id, HashTable, &subjectHashTable);
                break;
            case 9:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 9);
    for (int i = 0; i < MAX_STUDENTS; ++i) {
        if (HashTable[i].data != NULL) {
            free(HashTable[i].data->full_name);
            for (int j = 0; j < HashTable[i].data->num_marks; j++) {
                free(HashTable[i].data->marks[j].subject);
            }
            free(HashTable[i].data->marks);
            free(HashTable[i].data);
        }
    }

    sqlite3_close(db);

    return 0;
}
