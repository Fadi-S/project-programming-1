#ifndef PROJECTPROG_EMPLOYEE_H
#define PROJECTPROG_EMPLOYEE_H
typedef struct {
    unsigned int year: 12;
    unsigned int month: 4;
    unsigned int day: 5;
} Date;

typedef struct {
    int id;
    char first_name[20];
    char last_name[20];
    float salary;
    char email[30];
    char phone[20];
    char address[60];
    Date *birthday;
} Employee;
#endif //PROJECTPROG_EMPLOYEE_H
