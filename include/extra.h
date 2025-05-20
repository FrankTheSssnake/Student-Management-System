#ifndef EXTRA_H
#define EXTRA_H

int sum(int*);

void flush_stdin(void);

char *join_array(char**, int);

char *format_header(int *);

char *format_seperator(int);

void fprint_head(int, int*);

void trim(char*);

int isalpha_str(char*);

int isnum_str(char*);

void perror_open(char *filename);

#endif
