#ifndef STUDENT_H
#define STUDENT_H

extern int count;

typedef struct {
    int id;
    char* name;
    float marks;
} student;

char *format_entry(student);

void add_std(student**);

void disp_stds(student*);

void search_std(student*);

void update_std(student*);

void del_std(student**);

void free_students(student*);

#endif
