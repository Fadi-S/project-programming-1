#include <string.h>
#include <ctype.h>

int isPhoneValid(char *str) {
    int i;
    int valid = 1;

    if (strlen(str) < 11 || strlen(str) > 12)
        return 0;

    for (i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i])) {
            valid = 0;
            break;
        }
    }

    return valid;
}

int isAlphaNumeric(char str) {
    return isalnum(str) || (str == '_') || (str == '.');
}

int isEmailValid(char *email) {
    int at = 0, username = 0, domain = 0, ending = 0;

    for (int i = 0; i < strlen(email); ++i) {

        if (username && !at) {
            if (email[i] == '@') {
                at = 1;
                continue;
            }
        }

        if (!username && !at) {
            if (isAlphaNumeric(email[i])) {
                username = 1;
                continue;
            } else {
                return 0;
            }

        }

        if (at) {
            if (isAlphaNumeric(email[i])) {
                domain = 1;

                continue;
            } else {
                return 0;
            }
        }

        if (isAlphaNumeric(email[i])) {
            ending = 1;
            continue;
        } else {
            return 0;
        }
    }

    return domain && at && username && ending;
}

int isDateValid(unsigned int day, unsigned int month, unsigned int year) {
    if (month < 1 || month > 12) {
        return 0;
    }

    if (day < 1 || day > 31) {
        return 0;
    }

    int leap = 0;
    if (year % 400 == 0)
        leap = 1;
    else if (year % 100 == 0)
        leap = 0;
    else if (year % 4 == 0)
        leap = 1;


    if (month <= 7) {
        if (month % 2 == 0) {
            if (day > 30) {
                return 0;
            }

            if (month == 2 && leap == 1) {
                if (day > 29) {
                    return 0;
                }
            }
            if (month == 2 && leap == 0) {
                if (day > 28) {
                    return 0;
                }
            }
        } else if (day > 31) {
            return 0;
        }
    }else if (month >= 7) {
        if (month % 2 == 0) {
            if (day > 31) {
                return 0;
            }
        } else if (day > 30){
            return 0;
        }
    }

    return 1;
}