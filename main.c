#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "employee.h"
#include "validation.h"
#include "helpers.h"
#include "sort.h"

char *del = ",";
char *format = "%d,%s,%s,%d,%s,%s,%s,%d,%d,%d\n";
int unsavedData = 0;

Date *initDate(unsigned int day, unsigned int month, unsigned int year) {
    Date *date = malloc(sizeof(Date));

    date->day = day;
    date->month = month;
    date->year = year;

    return date;
}

Employee *initEmployee(int id, char *first_name, char *last_name, int salary, char *email, char *phone,
                       char *address, unsigned int day, unsigned int month, unsigned int year) {
    Employee *employee = malloc(sizeof(Employee));

    employee->id = id;
    strcpy(employee->first_name, first_name);
    strcpy(employee->last_name, last_name);
    employee->salary = salary;
    strcpy(employee->email, email);
    strcpy(employee->phone, phone);
    strcpy(employee->address, address);
    employee->birthday = initDate(day, month, year);
    return employee;
}

Employee *readEmployee() {
    char id[10];
    char first_name[20];
    char last_name[20];
    char email[30];
    char salary[10];
    char phone[20];
    char address[60];
    unsigned int year;
    unsigned int month;
    unsigned int day;

    int valid;
    do {
        printf("ID: \n");
        getString(id, 9);
        valid = isNumeric(id);
        if(!valid) {
            softError("ID must be numeric\n");
        }
    } while (!valid);

    printf("Last Name: \n");
    getString(last_name, 19);

    printf("First Name: \n");
    getString(first_name, 19);

    do {
        printf("Email: \n");
        getString(email, 29);
        valid = isEmailValid(email);
        if(!valid) {
            softError("Email is invalid\n");
        }
    } while (! valid);

    printf("Address: \n");
    getString(address, 59);

    do {
        printf("Salary: \n");
        getString(salary, 9);
        valid = isNumeric(salary);
        if(!valid) {
            softError("Salary must be numeric\n");
        }
    } while (!valid);

    do {
        printf("Phone: \n");
        getString(phone, 19);
        valid = isPhoneValid(phone);
        if(! valid) {
            softError("Phone is invalid\n");
        }
    } while (! valid);

    do {
        printf("Date (d/m/y) \n");
        scanf("%d/%d/%d", &day, &month, &year);
        valid = isDateValid(day, month, year);
        if(! valid) {
            softError("Birthday is invalid\n");
        }
    } while (! valid);

    return initEmployee(atoi(id), first_name, last_name, atoi(salary), email, phone, address, day, month, year);
}

char *serializeEmployee(Employee *employee) {
    char employeeStr[200];
    sprintf(employeeStr, format,
            employee->id, employee->last_name, employee->first_name, employee->salary,
            employee->email,
            employee->phone, employee->address,
            employee->birthday->day, employee->birthday->month, employee->birthday->year);

    char *str = malloc(strlen(employeeStr) + 1);

    strcpy(str, employeeStr);

    return str;
}

Employee *deserializeEmployee(char employee[200]) {
    char *token = strtok(employee, del);

    int id;
    char first_name[20];
    char last_name[20];
    char email[30];
    int salary;
    char phone[20];
    char address[60];
    unsigned int year;
    unsigned int month;
    unsigned int day;

    int i = 0;

    while (token != NULL) {

        switch (i) {
            case 0:
                id = atoi(token);
                break;
            case 1:
                strcpy(last_name, token);
                break;
            case 2:
                strcpy(first_name, token);
                break;
            case 3:
                salary = atoi(token);
                break;
            case 4:
                strcpy(email, token);
                break;
            case 5:
                strcpy(phone, token);
                break;
            case 6:
                strcpy(address, token);
                break;
            case 7:
                day = atoi(token);
                break;
            case 8:
                month = atoi(token);
                break;
            case 9:
                year = atoi(token);
                break;
        }

        i++;
        token = strtok(NULL, del);
    }

    return initEmployee(id, first_name, last_name, salary, email, phone, address, day, month, year);
}

void printEmployee(Employee *employee) {
    printf("ID: %d, Name: %s %s, Salary: %d, Phone: %s, Address: %s, Birthday: %d/%d/%d\n", employee->id,
           employee->first_name, employee->last_name,
           employee->salary, employee->phone, employee->address,
           employee->birthday->day, employee->birthday->month, employee->birthday->year);
}

Employee ** loadEmployees(int *numberOfRows)
{
    FILE *file = fopen("employees.txt", "r");
    if(! file) {
        * numberOfRows = 0;
        return malloc(0);
    }

    Employee * employeesTemp[100];
    char employeeStr[201];
    char *check;

    int i = 0;
    while (!feof(file)) {
        check = fgets(employeeStr, 200, file);
        if(! check) {
            break;
        }
        employeesTemp[i] = deserializeEmployee(employeeStr);

        i++;
    }

    Employee ** employees = malloc(sizeof (Employee *) * i);
    * numberOfRows = i;
    for (i = 0; i < * numberOfRows; ++i) {
        employees[i] = employeesTemp[i];
    }

    return employees;
}

void printEmployees(int n, Employee **employees)
{
    for (int i = 0; i < n; ++i) {
        printEmployee(employees[i]);
    }
}

void sortEmployees(int n, Employee **employees)
{
    int item;
    printf("Sort By: \n");
    printf("1. Last Name\n");
    printf("2. Date of Birth\n");
    printf("3. Salary\n");
    scanf("%d", &item);

    switch (item) {
        case 1:
            sortByLname(employees, n);
            break;
        case 2:
            sortByDOB(employees, n);
            break;
        case 3:
            sortBySalary(employees, n);
            break;
        default:
            softError("Command not found!");
    }

    printEmployees(n, employees);
}

Employee ** search(char *term, int *n, Employee **employees) {
    Employee *searchedTemp[200];

    int i, totalNumber = *n;
    *n = 0;
    for (i = 0; i < totalNumber; ++i) {
        if(strcasecmp(employees[i]->last_name, term) == 0) {
            searchedTemp[*n] = employees[i];

            *n += 1;
        }
    }

    int j;
    Employee **searched = malloc(sizeof (Employee *) * *n);
    for (j = 0; j < *n; ++j) {
        searched[j] = searchedTemp[j];
    }

    return searched;
}

void searchEmployees(int n, Employee **employees)
{
    char term[20];
    printf("Search by last name: ");
    getString(term, 19);

    Employee **searched = search(term, &n, employees);

    printEmployees(n, searched);

    free(searched);
}

void saveData(int n, Employee **employees)
{
    FILE *file = fopen("employees.txt", "w");
    validateFile(file);
    for (int i = 0; i < n; i++) {
        char *employeeStr = serializeEmployee(employees[i]);

        fprintf(file, "%s", employeeStr);

        free(employeeStr);
    }

    fclose(file);

    unsavedData = 0;

    printf("Data saved successfully!\n");
}

Employee ** addEmployee(int *n, Employee ** employees) {
    employees = realloc(employees, sizeof(Employee *) * ++(*n));
    employees[(*n) - 1] = readEmployee();

    unsavedData = 1;
    printf("Employee added successfully!\n");

    return employees;
}

Employee ** deleteEmployees(Employee **employees, int *n) {
    char firstname[30];
    char lastname[30];

    Employee * employeesTemp[200];
    int num = 0;

    printf("Please enter Last-Name: \n");
    getString(lastname, 29);
    printf("Please enter First-Name: \n");
    getString(firstname, 29);
    for (int i = 0; i < *n; i++) {

        if (strcasecmp(employees[i]->first_name, firstname) == 0
            && strcasecmp(employees[i]->last_name, lastname) == 0) {
            continue;
        }

        employeesTemp[num++] = employees[i];

    }

    free(employees);

    employees = malloc(sizeof (Employee *) * num);

    int i;
    for (i = 0; i < num; ++i) {
        employees[i] = employeesTemp[i];
    }

    *n = num;

    return employees;
}

void printMenu()
{
    printf("\n");
    printf("1. Search Employees\n");
    printf("2. Add Employee\n");
    printf("3. Delete Employee\n");
    printf("4. Modify Employee\n");
    printf("5. Print (Sort) Employees\n");
    printf("6. Save Data\n");
    printf("7. Quit\n");
}

int main() {
    printf("Welcome, \n");

    int employeesCount;
    Employee ** employees = loadEmployees(&employeesCount);

    int item;

    do {
        printMenu();
        printf("Enter command number: ");
        scanf("%d", &item);
        switch (item) {
            case 1:
                searchEmployees(employeesCount, employees);
                break;
            case 2:
                employees = addEmployee(&employeesCount, employees);

                break;
            case 3:
                employees = deleteEmployees(employees, &employeesCount);
                break;
            case 4:
                // TODO Modify
                break;
            case 5:
                sortEmployees(employeesCount, employees);
                break;
            case 6:
                saveData(employeesCount, employees);
                break;
            case 7:
                printf("\nBye!");
                exit(0);
            default:
                softError("Command not found!");
        }
    } while (item != 7);

    return 0;
}