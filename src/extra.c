#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <regex.h>

#include "../include/extra.h"

int widths[NO_OF_COLUMNS] = { 4, 4, 4 };
const char* headers[NO_OF_COLUMNS] = { "ID", "Name", "Marks" };


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


char read_char() {
    char output = getchar();

    flush_stdin();

    return output;
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

    
void trim(char *input) {
    if (input == NULL) return;

    size_t len = strlen(input);

    while (len > 0 && isspace((unsigned char)input[len - 1])) {
        len--;
    }

    input[len] = '\0';
}


char *get_pattern() {
    
    int length = 1 + (8 * NO_OF_COLUMNS) + 1;

    for (int i = 0; i < NO_OF_COLUMNS; i++) {
        length += strlen(headers[i]);
    }
    length -= 3;
    
    char *pattern = malloc(length);

    pattern[0] = '^';

    for (int i = 0; i < NO_OF_COLUMNS; i++) {
        strcat(pattern, headers[i]);
        strcat(pattern, "(\\s*)\\| ");
    }
    
    pattern[length - 1] = '$';
    pattern[length] = '\0';

    return pattern;
}


void calculate_widths(char *header) {

    char *pattern = get_pattern();

    regex_t regex;
    regmatch_t matches[sizeof(headers) / sizeof(headers[0]) + 1];

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fputs("Internal Error: Failed to compiler Regex\n", stderr);
        exit(1);
    }

    int ret = regexec(&regex, header, 4, matches, 0);

    if (ret == 0) {
    
        for (int i = 1; i < 4; i++) {
            widths[i-1] = ( matches[i].rm_eo - matches[i].rm_so ) + strlen(headers[i-1]) - 1;
        }

    } else {
        fputs("Internal Error: Regex Failed\n", stderr);
        exit(1);
    }

    regfree(&regex);
    free(pattern);
}


int count_lines(FILE *fp) {
    int lines = 0;
    char ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    rewind(fp);

    return lines;
}


char *format_header() {

    char *output = malloc(1);

    if (output == NULL) {
        fputs("Internal Error: Failed to allocate memory", stderr);
        exit(1);
    }

    int output_len = 1;

    output[0] = '\0';

    for (int i = 0; i < NO_OF_COLUMNS; i++) {
        int needed = snprintf(NULL, 0, "%-*s | ", widths[i], headers[i]);

        char *temp = malloc(needed + 1);

        if (temp == NULL) {
            fputs("Internal Error: Failed to allocate memory", stderr);
            exit(1);        
        }

        snprintf(temp, needed + 1, "%-*s | ", widths[i], headers[i]);

        output = realloc(output, output_len + needed);

        strcat(output, temp);

        free(temp);

        output_len += needed;
    }

    output[strlen(output) - 2] = '\0';

    return output;
}

char *format_seperator() {
    char* output = "";

    int total_width = sum(widths);

    for (int i = 0; i < total_width; i++) strcat(output, "-");

    return output;
}


void fprint_head(int fd) {

    char *header = format_header();
    char *seperator = format_seperator();

    FILE *fp = fdopen(fd, "w");

    fputs(header, fp);
    fputs("\n", fp);
    fputs(seperator, fp);
    fputs("\n", fp);

    free(header);
    free(seperator);

    fclose(fp);
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

