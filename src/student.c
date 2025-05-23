#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/student.h"
#include "../include/extra.h"

int count = 0;

student error_student = { .id = -1, .name = NULL, .marks = 0.0 };


void update_widths(student input_std) {
    float len;

    if ((len = log10(input_std.id)) > widths[0]) {
        widths[0] = len + 1;
    }
    if ((len = strlen(input_std.name)) > widths[1]) {
        widths[1] = len + 1;
    }
    if ((len = log10(input_std.marks) + 3) > widths[2]) {
        widths[2] = len + 1;
    }
}


char *format_entry(student input_std) {

    int needed = snprintf(NULL, 0, "%-*d | %-*s | %-*.2f\n",
            widths[0], input_std.id,
            widths[1], input_std.name,
            widths[2], input_std.marks
    );

    char *output = malloc(needed + 1);

    if (output == NULL) {
        fputs("Internal Error: Failed to allocate memory", stderr);
        return NULL;
    }

    snprintf(output, needed + 1, "%-*d | %-*s | %-*.2f\n",
            widths[0], input_std.id,
            widths[1], input_std.name,
            widths[2], input_std.marks
    );

    return output;
}


int compare_by_id(const void *a, const void *b) {
    student *s1 = (student *)a;
    student *s2 = (student *)b;
    return s1->id - s2->id;
}


char* read_name() {
    size_t size = 16;
    size_t len = 0;

    char *buffer = malloc(size);

    if (buffer == NULL) {
        fputs("Internal Error: Failed to allocate memory.\n", stderr);
        return NULL;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    
        if (len + 1 >= size) {
            size *= 2;
            char *temp = realloc(buffer, size);
        
            if (temp == NULL) {
                fputs("Internal Error: Failed to reallocate memory.\n", stderr);

                free(temp);
                return NULL;
            }

            buffer = temp;
        }

        buffer[len++] = (char)ch;
    }

    flush_stdin();

    buffer[len] = '\0';
    trim(buffer);

    if ( !(isalpha_str(buffer)) ) {
        fputs("Invalid Name provided.\n", stderr);
        return NULL;
    }

    return buffer;
}


student read_std() {
    student new_student;

    fputs("ID: ", stdout);
    scanf("%d", &new_student.id);
    flush_stdin();

    if (new_student.id < 0) {
        fputs("Invalid ID provided.\n", stderr);
        goto err;
    }

    fputs("Name: ", stdout);
    new_student.name = read_name();
    if (new_student.name == NULL) goto err;

    fputs("Enter marks: ", stdout);
    scanf("%f", &new_student.marks);
    flush_stdin();

    if (new_student.marks < 0 || new_student.marks > 100) {
        fputs("Invalid Marks provided.\n", stderr);
        goto err;
    }

    err:
        return error_student;

    return new_student;
}


void add_std(student **students) {
    student new_student = read_std();
    
    if (new_student.id == -1) exit(EXIT_FAILURE);

    student *tmp = realloc(*students, sizeof(student) * (count + 1));
    if (tmp == NULL) {
        fputs("Internal Error: Failed to allocate memory.\n", stderr);
        exit(EXIT_FAILURE);
    }

    *students = tmp;

    (*students)[count++] = new_student;

    qsort(*students, count, sizeof(student), compare_by_id);
    
    update_widths(new_student);
}


void disp_stds(student *students) {
    fprint_head(1);

    for (int i = 0; i < count; i++) {
        char *entry = format_entry(students[i]);
        puts(entry);
        free(entry);
    }
}


void disp_std(student* students, int std_count) {
    fprint_head(1);

    for (int i = 0; i < std_count; i++) {
        char *entry = format_entry(students[i]);
        puts(entry);
        free(entry);
    }
}


int search_by_id(student *students, int id) {
    int index = -1;

    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            index = i;
            break;
        }
    }

    return index;
}


int search_by_name(student *students, char *name) {
    int index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].name, name)) {
            index = i;
            break;
        }
    }

    return index;
}


void search_std(student *students) {
    char menu[] = "Search By\n1. ID\n2. Name\n>: ";
    fputs(menu, stdout);

    int index = -1;

    switch (getchar()) {
        case '1': {
            fputs("Enter ID: ", stdout);
            int id;
            scanf("%d", &id);
            index = search_by_id(students, id);
            break;
        }
        case '2': {
            char* name = read_name();
            if (name == NULL) {
                exit(EXIT_FAILURE);
            }

            index = search_by_name(students, name);
            
            free(name);
            break;
        }
        default:
            fputs("Invalid choice.\n", stderr);
            search_std(students);
            break;
    }

    if (index == -1) {
        fputs("Record not found.\n", stderr);
        exit(EXIT_FAILURE);
    } else {
        fputs("Student Found:\n", stdout);
        disp_std(&students[index], 1);
    }
}


void update_std(student *students) {
    int id = -1;

    fputs("Enter ID: ", stdout);
    scanf("%d", &id);
    flush_stdin();

    int index = search_by_id(students, id);

    fputs("Current Details:\n", stdout);
    disp_std(&students[index], 1);

    student tmp;
    fputs("Enter new details:\n", stdout);
    fputs("Name: ", stdout);
    tmp.name = read_name();
    if (tmp.name == NULL) exit(EXIT_FAILURE);;

    fputs("Enter marks: ", stdout);
    scanf("%f", &tmp.marks);
    flush_stdin();

    if (tmp.marks < 0 || tmp.marks > 100) {
        fputs("Invalid Marks provided.\n", stderr);
        exit(EXIT_FAILURE);
    }
}


void del_std(student **students) {
    int id = -1;

    fputs("Enter ID: ", stdout);
    scanf("%d", &id);
    flush_stdin();

    int index = search_by_id(*students, id);

    for (int i = index; i < count - 1; i++) {
        (*students)[i] = (*students)[i+1];
    }
    
    free((*students)[count - 1].name);

    student *tmp = realloc(*students, sizeof(student) * (count - 1));
    if (tmp == NULL) {
        fputs("Internal Error: Failed to deallocate memory.\n", stderr);
        exit(EXIT_FAILURE);
    }
    
    count--;
    *students = tmp;

    qsort(*students, count, sizeof(student), compare_by_id);
}


void free_students(student *students) {
    for (int i = 0; i < count; i++) {
        free(students[i].name);
    }
    free(students);
}
