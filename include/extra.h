#ifndef EXTRA_H
#define EXTRA_H

#include <stdio.h>

#include "../include/student.h"

#define NO_OF_COLUMNS 3

extern int widths[NO_OF_COLUMNS];
extern const char* headers[NO_OF_COLUMNS];

int sum(int arr[]);

void flush_stdin(void);

char read_char(void);

int isalpha_str(char *input);

void trim(char *input);

char *get_pattern(void);

void calculate_widths(char *header);

int count_lines(FILE *fp);

char *format_header(void);

char *format_seperator(void);

void fprint_head(int fd);

void perror_open(char*);

#endif
