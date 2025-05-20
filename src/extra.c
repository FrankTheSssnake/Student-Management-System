#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "extra.h"


int sum(int arr[]) {
    int output = 0;

    int i = 0;

    while (1) {
        int curr = arr[i];
        if (!curr) break;

        output += arr[i];
        i++;
    }

    return output;
}

void flush_stdin() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}


char* join_array(char *arr[], int length) {
    char* output = "";

    for (int i = 0; i < length; i++) strcat(output, arr[i]);

    return output;
}


char *format_header(int widths[]) {
    char *out_arr[] = {};

    const char *columns[] = { "ID", "Name", "Marks" };

    size_t column_count = sizeof(columns) / sizeof(columns[0]);

    for (int i = 0; i < column_count; i++) {
        const char *column = columns[i];
        int length = strlen(column);

        // calculation to compute final string
    }

    size_t length = sizeof(out_arr) / sizeof(out_arr[0]);

    char *output = join_array(out_arr, length);

    return output;
}


char *format_seperator(int total_width) {
    char* output = "";

    for (int i = 0; i < total_width; i++) strcat(output, "-");

    return output;
}


void fprint_head(int fd, int widths[]) {
    int total_width = sum(widths);

    char output[] = "";

    char *header = format_header(widths);
    char *seperator = format_seperator(total_width);

    strcat(output, header);
    strcat(output, seperator);

    FILE *fp = fdopen(fd, O_WRONLY);
    fputs(output, fp);
    fclose(fp);
}


int isalpha_str(char *input) {
    int length = strlen(input);

    for (int i = 0; i < length; i++) {
        int c = input[i];

        if ( !(isalpha(c)) ) {
            return 0;
        }
    }

    return 1;
}


void perror_open(char *filename) {
    switch (errno) {
        case ENOENT:
            fprintf(stderr, "Error: File '%s' does not exist.\n", filename);
            break;
        case EACCES:
            fprintf(stderr, "Error: Permission denied for file '%s'.\n", filename);
            break;
        case EISDIR:
            fprintf(stderr, "Error: '%s' is a directory, not a file.\n", filename);
            break;
        case ENOTDIR:
            fprintf(stderr, "Error: A component of the path in '%s' is not a directory.\n", filename);
            break;
        case EEXIST:
            fprintf(stderr, "Error: File '%s' already exists (used with O_EXCL).\n", filename);
            break;
        case EROFS:
            fprintf(stderr, "Error: Filesystem is read-only. Cannot open '%s' for writing.\n", filename);
            break;
        case EMFILE:
            fprintf(stderr, "Error: Too many files open in this process. Cannot open '%s'.\n", filename);
            break;
        case ENFILE:
            fprintf(stderr, "Error: Too many files open system-wide. Cannot open '%s'.\n", filename);
            break;
        case EINVAL:
            fprintf(stderr, "Error: Invalid flag combination used to open '%s'.\n", filename);
            break;
        case ELOOP:
            fprintf(stderr, "Error: Too many symbolic links encountered in path to '%s'.\n", filename);
            break;
        case ENAMETOOLONG:
            fprintf(stderr, "Error: Filename '%s' is too long.\n", filename);
            break;
        case ENOSPC:
            fprintf(stderr, "Error: No space left on device to create or extend '%s'.\n", filename);
            break;
        case ENODEV:
            fprintf(stderr, "Error: No such device exists for file '%s'.\n", filename);
            break;
        case EFAULT:
            fprintf(stderr, "Error: Invalid memory address for filename '%s'.\n", filename);
            break;
        default:
            fprintf(stderr, "Error: Failed to open '%s': %s\n", filename, strerror(errno));
            break;
    }
}

