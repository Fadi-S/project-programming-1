#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "employee.h"
#include "validation.h"
#include "helpers.h"
#include "sort.h"

char *del = ",-";
char *format = "%d,%s,%s,%.2f,%d-%d-%d,%s,%s,%s";
char *fileName = "Company.txt";
int unsavedData = 0;

Date *initDate(unsigned int day, unsigned int month, unsigned int year) {
    Date *date = malloc(sizeof(Date));

    date->day = day;
    date->month = month;
    date->year = year;

    return date;
}

Employee *initEmployee(int id, char *first_name, char *last_name, float salary, char *email, char *phone,
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
    char year[5] = "0";
    char month[3] = "0";
    char day[3] = "0";

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
        printf("Birthday: \n");
        printf("Day: ");
        getString(day, 3);
        printf("Month: ");
        getString(month, 3);
        printf("Year: ");
        getString(year, 5);
        valid = isNumeric(day) && isNumeric(month) && isNumeric(year) && isDateValid(atoi(day), atoi(month), atoi(year));
        if(! valid) {
            softError("Birthday is invalid\n");
        }
    } while (! valid);

    return initEmployee(atoi(id), first_name, last_name, atof(salary), email, phone, address, atoi(day), atoi(month), atoi(year));
}

char *serializeEmployee(Employee *employee) {
    char employeeStr[200];
    sprintf(employeeStr, format,
            employee->id, employee->last_name, employee->first_name, employee->salary,
            employee->birthday->day, employee->birthday->month, employee->birthday->year,
            employee->address,
            employee->phone, employee->email);

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
    float salary;
    char phone[20];
    char address[60];
    unsigned int year = 0;
    unsigned int month = 0;
    unsigned int day = 0;

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
                salary = atof(token);
                break;
            case 4:
                day = atoi(token);
                break;
            case 5:
                month = atoi(token);
                break;
            case 6:
                year = atoi(token);
                break;
            case 7:
                strcpy(address, token);
                break;
            case 8:
                strcpy(phone, token);
                break;
            case 9:
                strcpy(email, token);
                removeNewLine(email);
                break;
        }

        i++;
        token = strtok(NULL, del);
    }

    return initEmployee(id, first_name, last_name, salary, email, phone, address, day, month, year);
}

void printEmployee(Employee *employee) {
    printf("ID: %d, Name: %s %s, Salary: %.2f, Phone: %s, Address: %s, Birthday: %d/%d/%d\n", employee->id,
           employee->first_name, employee->last_name,
           employee->salary, employee->phone, employee->address,
           employee->birthday->day, employee->birthday->month, employee->birthday->year);
}

Employee ** loadEmployees(int *numberOfRows)
{
    FILE *file = fopen(fileName, "r");
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
    char item[2];
    printf("\nSort By: \n");
    printf("1. Last Name\n");
    printf("2. Date of Birth\n");
    printf("3. Salary\n");

    int isValid;
    do {
        printf("Choose: ");
        getString(item, 1);
        isValid = isdigit(item[0]);
        if(isValid) {
            item[0] = item[0] - '0';
            isValid = item[0] >= 1 && item[0] <= 3;
        }
    } while (!isValid);

    switch (item[0]) {
        case 1:
            printf("\nBy Last Name: \n");
            sortByLname(employees, n);
            break;
        case 2:
            printf("\nBy Date of Birth: \n");
            sortByDOB(employees, n);
            break;
        case 3:
            printf("\nBy Salary: \n");
            sortBySalary(employees, n);
            break;
        default:
            softError("Command not found!");
            return;
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
    FILE *file = fopen(fileName, "w");
    validateFile(file);
    for (int i = 0; i < n; i++) {
        char *employeeStr = serializeEmployee(employees[i]);

        removeNewLine(employeeStr);

        fprintf(file, "%s\n", employeeStr);
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
            unsavedData = 1;
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
void modifyEmployees(int n,Employee **employees)
{
    int i, id, found=0;
    printf("Please enter employee's id: ");
    scanf("%d", &id);

    for (i = 0; i < n; ++i)
    {
        if(employees[i]->id == id) {
            printf("Please enter the new data: \n");
            employees[i] = readEmployee();
            unsavedData = found = 1;
        }
    }

    if(! found) {
        printf("No employee found with that id!\n");
    }
}

void printMenu()
{
    printf("\n");
    printf("1. To Search employee:        Press (1) \n");
    printf("2. To Add employee:           Press (2) \n");
    printf("3. To Delete employee:        Press (3) \n");
    printf("4. To Modify employee:        Press (4) \n");
    printf("5. To Print employees (sort): Press (5) \n");
    printf("6. To Save data:              Press (6) \n");
    printf("7. To Quit:                   Press (7) \n");
}


int main() {
    printf("Welcome To Our Programme, \n");

    int employeesCount, loopCount=0;
    Employee ** employees = loadEmployees(&employeesCount);

    char item[2];
    char quit[2];

    do {
        if(loopCount) {
            printf("\nPress enter to continue...");
            getchar();
        }

        printMenu();
        int isValid;
        do {
            printf("Enter command number: ");
            getString(item, 1);
            isValid = isdigit(item[0]);
            if(isValid) {
                item[0] = item[0] - '0';
                isValid = item[0] >= 1 && item[0] <= 7;
            }
        } while(! isValid);

        switch (item[0]) {
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
                modifyEmployees(employeesCount, employees);
                break;
            case 5:
                sortEmployees(employeesCount, employees);
                break;
            case 6:
                saveData(employeesCount, employees);
                break;
            case 7:
                if(unsavedData) {
                    printf("\nAre you sure you want to quit? You have unsaved data [y/N] ");
                    getString(quit, 1);
                    if(strcasecmp(quit, "y") == 0) {
                        printf("\nSee You Next Time ;) ");
                        exit(0);
                    }

                    item[0] = 8;
                    loopCount = -1;
                } else {
                    printf("\nSee You Next Time ;) ");
                    exit(0);
                }
            default:
                softError("Command not found!");
        }

        loopCount++;
    } while (item[0] != 7);

    return 0;
}