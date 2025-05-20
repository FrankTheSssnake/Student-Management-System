#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"
#include "extra.h"

int count = 0;
int widths[4];

char* read_name() {
    char* name;

    char buffer[100];

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    name = malloc(strlen(buffer) + 1);
    if (name == NULL) {
        fputs("Failed to allocate memory.", stderr);
        exit(1);
    }

    strcpy(name, buffer);

    return name;
}


student read_std() {
    student new_student;

    puts("ID: ");
    scanf("%d", &new_student.id);
    flush_stdin();

    puts("Name: ");
    new_student.name = read_name();

    puts("Enter marks: ");
    scanf("%f", &new_student.marks);
    flush_stdin();

    return new_student;
}


void free_students(student *students) {
    for (int i = 0; i < count; i++) {
        free(students[i].name);
    }
    free(students);
}


void add_std(student **students) {
    student new_student = read_std();

    student *tmp = realloc(*students, sizeof(student) * (count + 1));
    if (tmp == NULL) {
        fputs("Failed to allocate memory.\n", stderr);
        exit(1);
    }

    (*students)[count++] = new_student;
}


void disp_std(student *student, int std_count) {
    if (std_count == 0) {
        fputs("No Students Found.\n", stderr);
        exit(0);
    }

    char *header = format_header(widths);
    char *seperator = format_seperator(sum(widths));
}


int search_by_id(student *students, int id) {
    int index = -1;

    for (int i = 0; i < count - 1; i++) {
        if (students[i].id == id) {
            index = i;
            break;
        }
    }

    return index;
}


int search_by_name(student *students, char *name) {
    int index = -1;

    for (int i = 0; i < count - 1; i++) {
        if (strcmp(students[i].name, name)) {
            index = i;
            break;
        }
    }

    return index;
}


void search_std(student *students) {
    char menu[] = "Search By\n1. ID\n2. Name\n>: ";
    puts(menu);

    int index = -1;

    switch (getchar()) {
        case '1': {
            int id;
            scanf("%d", &id);
            index = search_by_id(students, id);
            break;
        }
        case '2': {
            char* name = read_name();
            index = search_by_name(students, name);
            break;
        }
        default:
            fputs("Invalid choice.\n", stderr);
            search_std(students);
            break;
    }

    if (index == -1) {
        fputs("Record not found.\n", stderr);
        exit(1);
    } else {
        fputs("Student Found:\n", stdout);
        disp_std(&students[index], 1);
    }
}


void update_std(student *students) {
    int id = -1;

    puts("Enter ID: ");
    scanf("%d", &id);
    flush_stdin();

    int index = search_by_id(students, id);

    puts("Current Details:\n");
    disp_std(&students[index], 1);

    puts("Enter new details:\n");
    students[index] = read_std();
}


void del_std(student **students) {
    int id = -1;

    puts("Enter ID: ");
    scanf("%d", &id);
    flush_stdin();

    int index = search_by_id(*students, id);

    free((*students)[index].name);

    for (int i = index; i < count - 2; i++) {
        (*students)[i] = (*students)[i+1];
    }

    student *tmp = realloc(*students, sizeof(student) * (count - 1));
    if (tmp == NULL) {
        fputs("Failed to deallocate memory.\n", stderr);
        exit(1);
    }
    
    count--;
    *students = tmp;
}

