#ifndef STUDENT_H
#define STUDENT_H

extern int count;

typedef struct {
    int id;
    char* name;
    float marks;
} student;

char* read_name();

student read_std();

void free_students(student*);

void add_std(student**);

void disp_stds(student*);

void disp_std(student*, int);

int search_by_id(student*, int);

int search_by_name(student*, char*);

void search_std(student*);

void update_std(student*);

void del_std(student**);

char *format_entry(student);

int compare_by_id(const void*, const void*);

#endif
