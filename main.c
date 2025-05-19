#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct {
    int id;
    char name[100];
    float marks;
} student;

student[] students;

void flush_stdin() {
    int c;
    while((c = getchar() != '\n') || c != EOF);
}

void read_std() {
    printf("%s", __func__);
}
void add_std() {
    printf("%s", __func__);
}
void disp_std() {
    printf("%s", __func__);
}
void search_std() {
    printf("%s", __func__);
}
void update_std() {
    printf("%s", __func__);
}
void del_std() {
    printf("%s", __func__);
}

int main(int argc, char *argv[]) {

    // default fdi = STDIN, fdo = STDOUT
    int fdi = 0;
    int fdo = 1;

    // if file give, fd = file
    if (argc > 1) {
        fdi = open(argv[1], O_RDWR);
        fdo = fdi;
        if (fdi == -1) {
            perror("Failed to open file. Reading input from terminal.");
            fdi = 0;
            fdo = 1;
        }
    }

    char menu[] = "\n--- Student Management System ---\n1. Add Student\n2. Display All Students\n3. Search for a Student\n4. Update Student Details\n5. Delete a Student\n6. Exit\nEnter your choice: ";
 
    while (1) {

        puts(menu);

        switch (getchar()) {
            case '1':
                add_std();
                break;
            case '2':
                disp_std();
                break;
            case '3':
                search_std();
                break;
            case '4':
                update_std();
                break;
            case '5':
                del_std();
                break;
            case '6':
                puts("Thanks for using the Student Management System\n");
                goto end;
                break;
            case EOF:
                goto end;
                break;
            default:
                fprintf(stderr, "Invalid choice.\n");
                break;
        }

        flush_stdin();
    }

    end:
        close(fdi);

    return 0;
}

