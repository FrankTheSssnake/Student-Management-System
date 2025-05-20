#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "extra.h"
#include "student.h"


void load_file(int fd, student *students) {
    // TODO
    // 1. Check if file has data
    // 2. If it does not, return
    // 3. If it does, discard first two lines
    // 4. Count number of lines
    // 5. Allocate appropriate memory for students
    // 6. Read every line and scanf into student
}


void save_file(int fd, student* students) {
    // TODO
    // 1. Check if file has data
    // 2. If it does not, write header into it, goto 3
    // 3. Write every students data by fprintf
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

        switch (getchar()) {
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

        flush_stdin();
    }

    end: { 
        close(fd);
    }
    
    

    return 0;
}

