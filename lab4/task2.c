#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "models.h"
#include "task1.h"

int pop(Stack *stack) {
    if (stack->is_empty(stack)) {
        printf("Error: stack is empty\n");
        return -1;
    }
    int data = stack->top->data;
    Node *temp = stack->top;
    stack->top = stack->top->next;
    free(temp);
    return data;
}

int is_empty(Stack *stack) { return stack->top == NULL; }

void push(Stack *stack, int data) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation error\n");
        return;
    }
    new_node->data = data;
    new_node->next = stack->top;
    stack->top = new_node;
}

Stack *stack_init() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    stack->top = NULL;
    stack->push = push;
    stack->pop = pop;
    stack->is_empty = is_empty;
    return stack;
}

Node *add_node(Student *student) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->student = student;
    temp->next = NULL;
    return temp;
}

void append(struct List *self, Student *student) {
    Node *new_node = add_node(student);
    Node *last = self->head;
    if (self->head == NULL) {
        self->head = new_node;
        new_node->next = self->head;
        return;
    }
    while (last->next != self->head) {
        last = last->next;
    }
    last->next = new_node;
    new_node->next = self->head;
}

void print_list(struct List *self) {
    Node *temp = self->head;
    if (self->head != NULL) {
        do {
            Student *student = temp->student;
            printf("Name: %s, Number: %d\n", student->name, student->number);
            if (student->marks != NULL) {
                printf("Marks in stack for %s: ", student->name);
                Node *stack_node = student->marks->top;
                while (stack_node != NULL) {
                    printf("%d ", stack_node->data);
                    stack_node = stack_node->next;
                }
                printf("\n");
            }
            temp = temp->next;
        } while (temp != self->head);
    }
    printf("\n");
}

List *list_init() {
    List *clist = (List *)malloc(sizeof(List));
    clist->head = NULL;
    clist->append = &append;
    clist->print_list = &print_list;
    return clist;
}

void add_info(List *list) {
    char name[50];
    printf("Enter the name of the student: ");
    fgets(name, 50, stdin);
    size_t length = strlen(name);
    if (name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }
    int number;
    printf("Enter the group of the student: ");
    scanf("%d", &number);
    Stack *marks = stack_init();
    int mark = 1;
    while (1) {
        printf("Enter mark from (1 to 10) or 0 to exit: ");
        scanf("%d", &mark);
        if (mark == 0) {
            break;
        }
        marks->push(marks, mark);
    }
    Student *student = (Student *)malloc(sizeof(Student));
    student->name = (char *)malloc(strlen(name) + 1);
    strcpy(student->name, name);
    student->number = number;
    student->marks = marks;
    list->append(list, student);
}

void sort_by_number(List *list) {
    if (list->head == NULL || list->head->next == NULL) return;
    int swapped;
    Node *current;
    Node *last = NULL;
    do {
        swapped = 0;
        current = list->head;
        do {
            if (current->next == last || current->next == list->head) {
                last = current;
                break;
            }
            if (current->student->number > current->next->student->number) {
                Student *tempData = current->student;
                current->student = current->next->student;
                current->next->student = tempData;
                swapped = 1;
            }
            current = current->next;
        } while (current != last);
    } while (swapped);
}

void sort_name(List *list) {
    Node *node = list->head;
    sort_by_number(list);
    if (list->head != NULL) {
        do {
            Node *next_node = node->next;
            if (next_node == list->head) {
                break;
            }
            if (node->student->number != next_node->student->number) {
                node = next_node;
                continue;
            }
            while (next_node != list->head) {
                if (strcmp(next_node->student->name, node->student->name) < 0) {
                    Student *temp_student = node->student;
                    node->student = next_node->student;
                    next_node->student = temp_student;
                }
                next_node = next_node->next;
            }
            node = node->next;
        } while (node != list->head);
    }
}

sqlite3 *open_db(const char *dbname) {
    sqlite3 *db;
    int rc = sqlite3_open(dbname, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error while opening database: %s\n",
                sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }
    printf("Connected to database succesfully\n");
    return db;
}

void create_tables(sqlite3 *db) {
    char *sql_students =
        "CREATE TABLE IF NOT EXISTS students (\
                            id INTEGER PRIMARY KEY AUTOINCREMENT,\
                            name TEXT,\
                            number INTEGER\
                          )";

    char *sql_marks =
        "CREATE TABLE IF NOT EXISTS marks (\
                         id INTEGER PRIMARY KEY AUTOINCREMENT,\
                         student_id INTEGER,\
                         mark INTEGER,\
                         FOREIGN KEY (student_id) REFERENCES students(id)\
                       )";
    if (sqlite3_exec(db, sql_students, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error creating students table: %s\n",
                sqlite3_errmsg(db));
        return;
    }

    if (sqlite3_exec(db, sql_marks, NULL, NULL, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error creating marks table: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("Tables created successfully.\n");
}

int callback(void *data, int argc, char **argv, char **col_name) {
    List *list = (List *)data;
    static Student *previous_student = NULL;
    if (previous_student != NULL &&
        strcmp(previous_student->name, argv[1]) == 0) {
        for (int i = 0; i < argc; i++) {
            if (strcmp(col_name[i], "mark") == 0) {
                previous_student->marks->push(previous_student->marks,
                                              atoi(argv[i]));
                return 0;
            }
        }
    } else {
        Student *student = (Student *)malloc(sizeof(Student));
        student->marks = stack_init();

        for (int i = 0; i < argc; i++) {
            if (strcmp(col_name[i], "name") == 0) {
                student->name = strdup(argv[i]);
            } else if (strcmp(col_name[i], "number") == 0) {
                student->number = atoi(argv[i]);
            } else if (strcmp(col_name[i], "mark") == 0) {
                student->marks->push(student->marks, atoi(argv[i]));
            }
        }

        list->append(list, student);
        previous_student = student;
    }

    return 0;
}

void delete_student(List *list) {
    if (list->head != NULL) {
        Node *node = list->head;
        Node *temp = node;
        do {
            if (temp->next == list->head) {
                break;
            }
            temp = temp->next;
        } while (temp != list->head);

        do {
            Stack *marks = node->student->marks;
            float average = 0;
            int count = 0;
            Node *stack_node = marks->top;
            while (stack_node != NULL) {
                average += stack_node->data;
                stack_node = stack_node->next;
                count++;
            }
            average = average / count;
            if (average <= 3) {
                free(node->student->marks);
                free(node->student);
                if (temp == list->head) {
                    list->head = node->next;
                }
                temp->next = node->next;
                free(node);
                node = temp->next;
                continue;
            }
            temp = temp->next;
            node = node->next;
        } while (node != list->head);
    }
}

void search_by_name(List *list) {
    char name[50];
    printf("Enter the name of the student: ");
    fgets(name, 50, stdin);
    size_t length = strlen(name);
    if (name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }
    if (list->head != NULL) {
        Node *node = list->head;
        do {
            if (strcmp(node->student->name, name) == 0) {
                printf("Name: %s, Number: %d\n", node->student->name,
                       node->student->number);
                if (node->student->marks != NULL) {
                    printf("Marks in stack for %s: ", node->student->name);
                    Node *stack_node = node->student->marks->top;
                    while (stack_node != NULL) {
                        printf("%d ", stack_node->data);
                        stack_node = stack_node->next;
                    }
                    printf("\n");
                }
            }
            node = node->next;
        } while (node != list->head);
    }
}

void search_by_group(List *list) {
    int group;
    printf("Enter the group number: ");
    scanf("%d", &group);
    if (list->head != NULL) {
        Node *node = list->head;
        do {
            if (node->student->number == group) {
                printf("Name: %s, Number: %d\n", node->student->name,
                       node->student->number);
                if (node->student->marks != NULL) {
                    printf("Marks in stack for %s: ", node->student->name);
                    Node *stack_node = node->student->marks->top;
                    while (stack_node != NULL) {
                        printf("%d ", stack_node->data);
                        stack_node = stack_node->next;
                    }
                    printf("\n");
                }
            }
            node = node->next;
        } while (node != list->head);
    }
}

void update(List *list) {
    char name[50];
    printf("Enter the name of the student you want to update: ");
    fgets(name, 50, stdin);
    size_t length = strlen(name);
    if (name[length - 1] == '\n') {
        name[length - 1] = '\0';
    }
    if (list->head != NULL) {
        Node *node = list->head;
        do {
            if (strcmp(node->student->name, name) == 0) {
                printf("Enter new name to the student: ");
                char newname[50];
                fgets(newname, 50, stdin);
                size_t length = strlen(newname);
                if (newname[length - 1] == '\n') {
                    newname[length - 1] = '\0';
                }
                free(node->student->name);
                node->student->name = (char *)malloc(strlen(name) + 1);
                strcpy(node->student->name, newname);
                printf("Enter new group number: ");
                scanf("%d", &(node->student->number));
                Stack *marks = stack_init();
                int mark = 1;
                while (1) {
                    printf("Enter mark from (1 to 10) or 0 to stop: ");
                    scanf("%d", &mark);
                    if (mark == 0) {
                        break;
                    }
                    marks->push(marks, mark);
                }
                free(node->student->marks);
                node->student->marks = marks;
                break;
            }
            node = node->next;
        } while (node != list->head);
    }
}

void get_data(sqlite3 *db, List *list) {
    char *err_msg = 0;
    char *sql = sqlite3_mprintf(
        "SELECT students.id, students.name, students.number, marks.mark "
        "FROM students LEFT JOIN marks ON students.id = marks.student_id");
    int rc = sqlite3_exec(db, sql, callback, list, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error while inserting into database: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(-1);
    }
    sqlite3_free(sql);
}

void insert_data(List *list, sqlite3 *db) {
    char *sql_clear_students = "DELETE FROM students";
    char *sql_clear_marks = "DELETE FROM marks";

    int rc;
    rc = sqlite3_exec(db, sql_clear_students, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error clearing students table: %s\n",
                sqlite3_errmsg(db));
        return;
    }

    rc = sqlite3_exec(db, sql_clear_marks, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error clearing marks table: %s\n", sqlite3_errmsg(db));
        return;
    }

    Node *node = list->head;
    do {
        char name[50];
        Student *student = node->student;
        strcpy(name, student->name);

        char *sql_student = sqlite3_mprintf(
            "INSERT INTO students (name, number) VALUES (%Q, %d)", name,
            student->number);

        rc = sqlite3_exec(db, sql_student, NULL, NULL, NULL);
        sqlite3_free(sql_student);

        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error inserting student data: %s\n",
                    sqlite3_errmsg(db));
            return;
        }

        int student_id = sqlite3_last_insert_rowid(db);
        Stack *marks = student->marks;
        Node *mark_node = marks->top;
        while (mark_node != NULL) {
            char *sql_mark = sqlite3_mprintf(
                "INSERT INTO marks (student_id, mark) VALUES (%d, %d)",
                student_id, mark_node->data);

            rc = sqlite3_exec(db, sql_mark, NULL, NULL, NULL);
            sqlite3_free(sql_mark);

            if (rc != SQLITE_OK) {
                fprintf(stderr, "Error inserting mark data: %s\n",
                        sqlite3_errmsg(db));
                return;
            }

            mark_node = mark_node->next;
        }

        node = node->next;
    } while (node != list->head);

    printf("Data inserted successfully.\n");
}

void task2(char *db_name) {
    List *list = list_init();
    sqlite3 *db = open_db(db_name);
    create_tables(db);
    int choice;
    while (1) {
        printf(
            "Select the task: \n1) Show info\n2) Add info\n3) Sort\n"
            "4) Delete students(lower than 3)\n5) Search "
            "by name\n6) Search by group number\n7) Modify info.\n8) Save "
            "to "
            "database\n9) Load data from database\n10) Exit program\n");
        scanf("%d", &choice);
        scanf("%*c");

        if (choice < 0 || choice > 11) {
            printf("Input not correct, try again\n");
        } else {
            switch (choice) {
                case 1: {
                    list->print_list(list);
                    break;
                }
                case 2:
                    add_info(list);
                    break;
                case 3:
                    sort_name(list);
                    break;

                case 4:
                    delete_student(list);
                    break;
                case 5:
                    search_by_name(list);
                    break;
                case 6:
                    search_by_group(list);
                    break;
                case 7:
                    update(list);
                    break;
                case 8:
                    insert_data(list, db);
                    break;
                case 9:
                    get_data(db, list);
                    break;
                case 10:
                    exit(0);
                    break;
                default:
                    exit(1);
            }
        }
    }
}