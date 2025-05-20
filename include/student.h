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

void add_std(student**);

void disp_std(student*, int);

void free_students(student*);

int search_by_id(student*, int);

int search_by_name(student*, char*);

void search_std(student*);

void update_std(student*);

void del_std(student**);

#endif

