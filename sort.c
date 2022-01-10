#include <string.h>
#include "employee.h"

int compareDates(Date * date1, Date *date2) {
    if(date1->year > date2->year)
        return 1;

    if(date1->year < date2->year)
        return -1;

    if (date1->month > date2->month)
        return 1;

    if (date1->month < date2->month)
        return -1;

    if (date1->day > date2->day)
        return 1;

    if (date1->day < date2->day)
        return -1;

    return 0;
}

void mergeSort(Employee **arr, int low, int mid, int high, char *key) {
    int nL = mid - low + 1;
    int nR = high - mid;

    Employee *L[nL];
    Employee *R[nR];
    int i;
    for (i = 0; i < nL; i++) {
        L[i] = arr[low + i];
    }
    for (i = 0; i < nR; i++) {
        R[i] = arr[mid + i + 1];
    }

    int j = 0, k = low;
    i = 0;

    while (i < nL && j < nR) {
        int check = 0;
        if(strcasecmp(key, "last_name") == 0) {
            check = strcasecmp(L[i]->last_name, R[j]->last_name) < 0;
        }else if(strcasecmp(key, "salary") == 0) {
            check = L[i]->salary < R[j]->salary;
        }else if(strcasecmp(key, "birthday") == 0) {
            check = compareDates(L[i]->birthday, R[j]->birthday) < 0;
        }

        if (check)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < nL)
        arr[k++] = L[i++];

    while (j < nR)
        arr[k++] = R[j++];
}

void sort(Employee **employees, int low, int high, char *key) {
    if (low >= high)
        return;

    int mid = (low + high) / 2;
    sort(employees, low, mid, key);
    sort(employees, mid + 1, high, key);
    mergeSort(employees, low, mid, high, key);
}

void sortByLname(Employee** employees, int n) {
    sort(employees, 0, n-1, "last_name");
}

void sortByDOB(Employee** employees, int n) {
    sort(employees, 0, n-1, "birthday");
}

void sortBySalary(Employee** employees, int n) {
    sort(employees, 0, n-1, "salary");
}