#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Type {
    ENROLLED,
    EXPELLED,
    RESTORED,
} Type;

typedef struct {
    int mark;
    char *subject;
} Mark;

typedef struct {
    Type type;
    char date[11];
} Event;

typedef struct Student {
    int id;
    char *full_name;
    Mark *marks;
    int marks_count;
    Event *events;
    int events_count;
    struct Student *left;
    struct Student *right;
} Student;

char *duplicate_string(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char *dup = malloc(len);
    if (dup != NULL) {
        memcpy(dup, str, len);
    }
    return dup;
}

Student *create_student(int id, const char *full_name) {
    Student *new_student = malloc(sizeof(Student));
    if (new_student == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_student->id = id;
    new_student->full_name = duplicate_string(full_name);
    new_student->marks = NULL;
    new_student->marks_count = 0;
    new_student->events = NULL;
    new_student->events_count = 0;
    new_student->left = NULL;
    new_student->right = NULL;
    return new_student;
}

Student *insert_student(Student *root, Student *new_student) {
    if (root == NULL) return new_student;
    if (new_student->id < root->id) {
        root->left = insert_student(root->left, new_student);
    } else if (new_student->id > root->id) {
        root->right = insert_student(root->right, new_student);
    }
    return root;
}

Student *find_student_by_name(Student *root, const char *full_name) {
    if (root == NULL) return NULL;
    int cmp = strcmp(full_name, root->full_name);
    if (cmp == 0) return root;
    if (cmp < 0) return find_student_by_name(root->left, full_name);
    return find_student_by_name(root->right, full_name);
}

Student *find_min(Student *root) {
    while (root->left != NULL) root = root->left;
    return root;
}

Student *delete_student(Student *root, int id) {
    if (root == NULL) return NULL;
    if (id < root->id) {
        root->left = delete_student(root->left, id);
    } else if (id > root->id) {
        root->right = delete_student(root->right, id);
    } else {
        if (root->left == NULL) {
            Student *temp = root->right;
            free(root->full_name);
            for (int i = 0; i < root->marks_count; i++) {
                free(root->marks[i].subject);
            }
            free(root->marks);
            free(root->events);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Student *temp = root->left;
            free(root->full_name);
            for (int i = 0; i < root->marks_count; i++) {
                free(root->marks[i].subject);
            }
            free(root->marks);
            free(root->events);
            free(root);
            return temp;
        }
        Student *temp = find_min(root->right);
        root->id = temp->id;
        free(root->full_name);
        root->full_name = duplicate_string(temp->full_name);
        root->right = delete_student(root->right, temp->id);
    }
    return root;
}

void restore_students(Student *root) {
    if (root == NULL) return;
    restore_students(root->left);
    restore_students(root->right);

    int sum_marks = 0;
    for (int i = 0; i < root->marks_count; i++) {
        sum_marks += root->marks[i].mark;
    }
    double average = (double)sum_marks / root->marks_count;

    if (root->events[root->events_count - 1].type == EXPELLED &&
        average > 7.0) {
        root->events[root->events_count - 1].type = RESTORED;
        printf("Restoring student %s with average mark %.2f\n", root->full_name,
               average);
    }
}

Student *delete_students_below_average(Student *root, double threshold) {
    if (root == NULL) return NULL;

    root->left = delete_students_below_average(root->left, threshold);
    root->right = delete_students_below_average(root->right, threshold);

    int sum_marks = 0;
    for (int i = 0; i < root->marks_count; i++) {
        sum_marks += root->marks[i].mark;
    }
    double average = (double)sum_marks / root->marks_count;

    if (average < threshold &&
        root->events[root->events_count - 1].type == ENROLLED) {
        root = delete_student(root, root->id);
    }
    return root;
}

void print_students(Student *root) {
    if (root == NULL) return;
    print_students(root->left);
    printf("ID: %d, Name: %s\n", root->id, root->full_name);
    printf("%d marks: ", root->marks_count);
    for (int i = 0; i < root->marks_count; i++) {
        printf("%s: %d ", root->marks[i].subject, root->marks[i].mark);
    }
    printf("\n");
    print_students(root->right);
}

void free_tree(Student *root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root->full_name);
    for (int i = 0; i < root->marks_count; i++) {
        free(root->marks[i].subject);
    }
    free(root->marks);
    free(root->events);
    free(root);
}

Student *add_student_from_input(Student *root) {
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

    Mark *marks = malloc(marks_count * sizeof(Mark));
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

    Event *events = malloc(events_count * sizeof(Event));
    for (int i = 0; i < events_count; i++) {
        char event_type[20];
        printf(
            "Enter event type (ENROLLED/EXPELLED/RESTORED) and date "
            "(yyyy-mm-dd): ");
        scanf("%s %s", event_type, events[i].date);

        if (strcmp(event_type, "ENROLLED") == 0) {
            events[i].type = ENROLLED;
        } else if (strcmp(event_type, "EXPELLED") == 0) {
            events[i].type = EXPELLED;
        } else if (strcmp(event_type, "RESTORED") == 0) {
            events[i].type = RESTORED;
        } else {
            printf("Invalid event type. Please try again.\n");
            i--;
        }
    }

    Student *new_student = create_student(id, full_name);
    new_student->marks = marks;
    new_student->marks_count = marks_count;
    new_student->events = events;
    new_student->events_count = events_count;

    return insert_student(root, new_student);
}

int create_tables(sqlite3 *db) {
    char *err_msg = NULL;
    const char *sql =
        "CREATE TABLE IF NOT EXISTS students ("
        "id INTEGER PRIMARY KEY,"
        "full_name TEXT NOT NULL);"
        "CREATE TABLE IF NOT EXISTS marks ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER,"
        "mark INTEGER NOT NULL,"
        "subject TEXT NOT NULL,"
        "FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE);"
        "CREATE TABLE IF NOT EXISTS events ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER,"
        "type TEXT CHECK(type IN ('ENROLLED', 'EXPELLED', 'RESTORED')) NOT "
        "NULL,"
        "date TEXT NOT NULL,"
        "FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE);";

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create tables: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    return SQLITE_OK;
}

int save_student(sqlite3 *db, const Student *student) {
    char *err_msg = NULL;
    char sql[512];
    int rc;

    snprintf(sql, sizeof(sql), "DELETE FROM events WHERE student_id = %d;",
             student->id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete events for student %d: %s\n",
                student->id, err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    snprintf(sql, sizeof(sql), "DELETE FROM marks WHERE student_id = %d;",
             student->id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete marks for student %d: %s\n",
                student->id, err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    snprintf(sql, sizeof(sql), "DELETE FROM students WHERE id = %d;",
             student->id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete student %d: %s\n", student->id,
                err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    snprintf(sql, sizeof(sql),
             "INSERT INTO students (id, full_name) VALUES (%d, '%s');",
             student->id, student->full_name);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to insert student: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    for (int i = 0; i < student->marks_count; ++i) {
        snprintf(sql, sizeof(sql),
                 "INSERT INTO marks (student_id, mark, subject) VALUES (%d, "
                 "%d, '%s');",
                 student->id, student->marks[i].mark,
                 student->marks[i].subject);

        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to insert mark for student %d: %s\n",
                    student->id, err_msg);
            sqlite3_free(err_msg);
            return rc;
        }
    }

    for (int i = 0; i < student->events_count; ++i) {
        const char *event_type_str;
        switch (student->events[i].type) {
            case ENROLLED:
                event_type_str = "ENROLLED";
                break;
            case EXPELLED:
                event_type_str = "EXPELLED";
                break;
            case RESTORED:
                event_type_str = "RESTORED";
                break;
            default:
                fprintf(stderr, "Invalid event type.\n");
                return SQLITE_ERROR;
        }

        snprintf(sql, sizeof(sql),
                 "INSERT INTO events (student_id, type, date) VALUES (%d, "
                 "'%s', '%s');",
                 student->id, event_type_str, student->events[i].date);

        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Failed to insert event for student %d: %s\n",
                    student->id, err_msg);
            sqlite3_free(err_msg);
            return rc;
        }
    }

    return SQLITE_OK;
}

int save_to_db(sqlite3 *db, const Student *root) {
    if (root == NULL) return SQLITE_OK;
    int rc = save_student(db, root);
    if (rc != SQLITE_OK) return rc;
    rc = save_to_db(db, root->left);
    if (rc != SQLITE_OK) return rc;
    rc = save_to_db(db, root->right);
    if (rc != SQLITE_OK) return rc;

    return SQLITE_OK;
}

char *strdup_sqlite(const unsigned char *text) {
    if (!text) return NULL;
    return strdup((const char *)text);
}

Student *find_student(Student *root, int id) {
    while (root != NULL) {
        if (id == root->id) {
            return root;
        } else if (id < root->id) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

Student *create_student_node(int id, const char *full_name) {
    Student *node = (Student *)malloc(sizeof(Student));
    if (!node) {
        perror("Memory allocation failed");
        return NULL;
    }
    node->id = id;
    node->full_name = strdup(full_name);
    node->marks = NULL;
    node->marks_count = 0;
    node->events = NULL;
    node->events_count = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_student(Student *student) {
    if (student) {
        free(student->full_name);
        if (student->marks) {
            for (int i = 0; i < student->marks_count; ++i) {
                free(student->marks[i].subject);
            }
            free(student->marks);
        }
        if (student->events) {
            free(student->events);
        }
        free(student);
    }
}

Mark *create_mark(int mark, const char *subject) {
    Mark *new_mark = (Mark *)malloc(sizeof(Mark));
    if (!new_mark) {
        perror("Memory allocation failed");
        return NULL;
    }
    new_mark->mark = mark;
    new_mark->subject = strdup(subject);
    return new_mark;
}

Event *create_event(Type type, const char *date) {
    Event *new_event = (Event *)malloc(sizeof(Event));
    if (!new_event) {
        perror("Memory allocation failed");
        return NULL;
    }
    new_event->type = type;
    strncpy(new_event->date, date, sizeof(new_event->date) - 1);
    new_event->date[sizeof(new_event->date) - 1] = '\0';
    return new_event;
}
int read_students_callback(void *data, int argc, char **argv,
                           char **col_names) {
    Student **root = (Student **)data;

    int id = atoi(argv[0]);
    const char *full_name = argv[1];
    Student *node = find_student(*root, id);
    if (!node) {
        node = create_student_node(id, full_name);
        if (!node) {
            fprintf(stderr, "Failed to create student node\n");
            return SQLITE_ERROR;
        }
        *root = insert_student(*root, node);
    }
    for (int i = 2; i < argc; i += 2) {
        if (strcmp(col_names[i], "mark") == 0 &&
            strcmp(col_names[i + 1], "subject") == 0) {
            int mark = atoi(argv[i]);
            const char *subject = argv[i + 1];
            if (node->marks == NULL) {
                node->marks = (Mark *)malloc(sizeof(Mark));
            } else {
                node->marks = (Mark *)realloc(
                    node->marks, (node->marks_count + 1) * sizeof(Mark));
            }

            if (!node->marks) {
                perror("Memory allocation failed");
                return SQLITE_ERROR;
            }
            node->marks[node->marks_count].mark = mark;
            node->marks[node->marks_count].subject = strdup(subject);
            node->marks_count++;
        } else if (strcmp(col_names[i], "type") == 0 &&
                   strcmp(col_names[i + 1], "date") == 0) {
            const char *event_type_str = argv[i];
            const char *date = argv[i + 1];
            Type event_type;
            if (strcmp(event_type_str, "ENROLLED") == 0)
                event_type = ENROLLED;
            else if (strcmp(event_type_str, "EXPELLED") == 0)
                event_type = EXPELLED;
            else if (strcmp(event_type_str, "RESTORED") == 0)
                event_type = RESTORED;
            else {
                fprintf(stderr, "Unknown event type: %s\n", event_type_str);
                return SQLITE_ERROR;
            }

            if (node->events == NULL) {
                node->events = (Event *)malloc(sizeof(Event));
            } else {
                node->events = (Event *)realloc(
                    node->events, (node->events_count + 1) * sizeof(Event));
            }

            if (!node->events) {
                perror("Memory allocation failed");
                return SQLITE_ERROR;
            }
            node->events[node->events_count].type = event_type;
            strncpy(node->events[node->events_count].date, date,
                    sizeof(node->events[node->events_count].date) - 1);
            node->events[node->events_count]
                .date[sizeof(node->events[node->events_count].date) - 1] = '\0';
            node->events_count++;
        }
    }

    return SQLITE_OK;
}

int read_students(sqlite3 *db, Student **root) {
    char *err_msg = NULL;
    const char *sql =
        "SELECT students.id, students.full_name, "
        "marks.mark, marks.subject, "
        "events.type, events.date "
        "FROM students "
        "LEFT JOIN marks ON students.id = marks.student_id "
        "LEFT JOIN events ON students.id = events.student_id;";

    int rc = sqlite3_exec(db, sql, read_students_callback, root, &err_msg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to select students: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }

    return SQLITE_OK;
}

void free_binary_tree(Student *root) {
    if (root) {
        free_binary_tree(root->left);
        free_binary_tree(root->right);
        free_student(root);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <database_name>\n", argv[0]);
        return 1;
    }
    char *db_name = argv[1];
    Student *root = NULL;
    int choice;

    sqlite3 *db;
    int rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = create_tables(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create tables in database\n");
        sqlite3_close(db);
        return 1;
    }
    do {
        printf("\nMenu:\n");
        printf("1. Print all students\n");
        printf("2. Find student by name\n");
        printf("3. Restore expelled students with average mark > 7\n");
        printf("4. Delete students with average mark < 5\n");
        printf("5. Add a new student\n");
        printf("6. Save changes to database\n");
        printf("7. Read data from database\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                print_students(root);
                break;
            case 2: {
                char name[100];
                printf("Enter student name: ");
                getchar();
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                Student *found = find_student_by_name(root, name);
                if (found != NULL) {
                    printf("Found student\n");
                    printf("ID: %d, Name: %s\n", found->id, found->full_name);
                    printf("%d marks: ", found->marks_count);
                    for (int i = 0; i < found->marks_count; i++) {
                        printf("%s: %d ", found->marks[i].subject,
                               found->marks[i].mark);
                    }
                    printf("\n");
                } else {
                    printf("Student not found.\n");
                }
                break;
            }
            case 3:
                restore_students(root);
                break;
            case 4:
                root = delete_students_below_average(root, 5.0);
                break;
            case 5:
                root = add_student_from_input(root);
                break;
            case 6:
                rc = save_to_db(db, root);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "Failed to save changes to database\n");
                } else {
                    printf("Changes saved to database successfully\n");
                }
                break;
            case 7:
                read_students(db, &root);
                break;
            case 8:
                free_binary_tree(root);
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 8);

    sqlite3_close(db);

    return 0;
}
