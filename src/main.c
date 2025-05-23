#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/extra.h"
#include "../include/student.h"


void load_file(int fd, student *students) {

    FILE *fp = fdopen(fd, O_RDONLY);

    int ch = fgetc(fp);
    if (ch == EOF) exit(1);
    ungetc(ch, fp);

    char buffer[100];
    fgets(buffer, sizeof(buffer), fp);
    calculate_widths(buffer);

    fgets(buffer, sizeof(buffer), fp);

    count = count_lines(fp);

    students = realloc(students, count * sizeof(student));

    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d | %[^|] | %f\n", &students[i].id, students[i].name, &students[i].marks);
    }

    fclose(fp);
}


void save_file(int fd, student* students) {

    qsort(students, count, sizeof(student), compare_by_id);

    FILE* fp = fdopen(fd, O_WRONLY);

    if (ftell(fp) == 0) fprint_head(fd);

    for (int i = 0; i < count; i++) {
        student cur_student = students[i];

        char *entry = format_entry(cur_student);

        fprintf(fp, "%s", entry);
    }

    fclose(fp);
}


int main(int argc, char *argv[]) {

    student *students = NULL;

    int fd = 0;
    char* filename;

    if (argc > 1) {
        int fd = open(argv[1], O_RDWR);
        if (fd == -1) {
            perror_open(argv[1]);
            exit(EXIT_FAILURE);
        }

        load_file(fd, students);
    }

    char menu[] = "\n--- Student Management System ---\n1. Add Student\n2. Display All Students\n3. Search for a Student\n4. Update Student Details\n5. Delete a Student\n6. Exit\nEnter your choice: ";

    while (1) {

        puts(menu);

        switch (read_char()) {
            case '1':
                add_std(&students);
                break;
            case '2':
                
                disp_std(students, count);
                break;
            case '3':
                search_std(students);
                break;
            case '4':
                update_std(students);
                break;
            case '5':
                del_std(&students);
                break;
            case '6':
                save_file(fd, students);
                puts("Thanks for using the Student Management System\n");
                goto end;
                break;
            case EOF:
                goto end;
                break;
            default:
                fputs("Invalid Choice.\n", stderr);
                break;
        }
    }

    end: { 
        close(fd);
        free(students);
    } 

    return 0;
}

