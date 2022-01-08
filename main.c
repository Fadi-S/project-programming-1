#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validation.h"
#include "helpers.h"

char *del = ",";
char *format = "%d,%s,%s,%d,%s,%s,%s,%d,%d,%d\n";
int unsavedData = 0;

typedef struct {
    unsigned int year: 12;
    unsigned int month: 4;
    unsigned int day: 5;
} Date;

typedef struct {
    int id;
    char first_name[20];
    char last_name[20];
    int salary;
    char email[30];
    char phone[20];
    char address[60];
    Date *birthday;
} Employee;

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

void freeEmployee(Employee *employee) {
    free(employee->birthday);
    free(employee);
}

Employee *readEmployee() {
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

    printf("ID: \n");
    scanf("%d", &id);

    printf("Last Name: \n");
    scanf("%s", last_name);

    printf("First Name: \n");
    scanf("%s", first_name);

    int valid;
    do {
        printf("Email: \n");
        scanf("%s", email);
        valid = isEmailValid(email);
        if(!valid) {
            softError("Email is invalid\n");
        }
    } while (! valid);

    printf("Address: \n");
    getchar();
    fgets(address, 59, stdin);
    if ((strlen(address) > 0) && (address[strlen (address) - 1] == '\n'))
        address[strlen (address) - 1] = '\0';

    printf("Salary: \n");
    scanf("%d", &salary);

    do {
        printf("Phone: \n");
        scanf("%s", phone);
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

    return initEmployee(id, first_name, last_name, salary, email, phone, address, day, month, year);
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
void Merge(char* arr[],int low,int mid,int high)
{
    int nL= mid-low+1;
    int nR= high-mid;

    char** L=malloc(sizeof(char *)*nL);
    char** R=malloc(sizeof(char *)*nR);
    int i;
    for(i=0;i<nL;i++)
    {
        L[i]=malloc(sizeof(arr[low+i]));
        strcpy(L[i],arr[low+i]);
    }
    for(i=0;i<nR;i++)
    {
        R[i]=malloc(sizeof(arr[mid+i+1]));
        strcpy(R[i],arr[mid+i+1]);
    }
    int j=0,k;
    i=0;
    k=low;
    while(i<nL&&j<nR)
    {
        if(strcmp(L[i],R[j])<0)strcpy(arr[k++],L[i++]);
        else strcpy(arr[k++],R[j++]);
    }
    while(i<nL)strcpy(arr[k++],L[i++]);
    while(j<nR)strcpy(arr[k++],R[j++]);

}


void MergeSortS(Employee*arr,int low,int high)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        MergeSortS(arr->salary,low,mid);
        MergeSortS(arr->salary,mid+1,high);
        Merge(arr->salary,low,mid,high);
    }
}
void MergeSortL(Employee*arr,int low,int high)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        MergeSortL(arr->last_name,low,mid);
        MergeSortL(arr->last_name,mid+1,high);
        Merge(arr->last_name,low,mid,high);
    }
}
void MergeSortdob(Employee*arr,int low,int high)
{
    if(low<high)
    {
        int mid=(low+high)/2;
        MergeSortdob(arr->birthday,low,mid);
        MergeSortdob(arr->birthday,mid+1,high);
        Merge(arr->birthday,low,mid,high);
    }
}
void printEmployee(Employee *employee) {
    int item;
    printf("which kind of sort do yo want \n ");
    printf("press 1 for last name sorted\n");
    printf("press 2 for date of birth name sorted\n");
    printf("press 3 for salary sorted\n");
    scanf("%d",&item);
    switch (item) {
        case 1:MergeSortL(employee,0, sizeof(employee));
            break;
        case 2:MergeSortdob(employee,0, sizeof(employee));
            break;
        case 3:MergeSortS(employee,0, sizeof(employee));
            break;
        default:
            break;
    }



    printf("ID: %d, Name: %s %s, Salary: %d, Phone: %s, Address: %s, Birthday: %d/%d/%d\n", employee->id,
           employee->first_name, employee->last_name,
           employee->salary, employee->phone, employee->address,
           employee->birthday->day, employee->birthday->month, employee->birthday->year);
}

Employee ** loadEmployees(int *numberOfRows)
{
    FILE *file = fopen("employees.txt", "r");
    if(! file) {
        return NULL;
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

void saveData(int n, Employee **employees)
{
    FILE *file = fopen("employees.txt", "w");
    validateFile(file);
    for (int i = 0; i < n; i++) {
        char *employeeStr = serializeEmployee(employees[i]);

        fprintf(file, "%s", employeeStr);
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
                break;
            case 2:
                employees = addEmployee(&employeesCount, employees);

                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                printEmployees(employeesCount, employees);
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
