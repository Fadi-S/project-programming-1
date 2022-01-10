#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Some helper functions to help us create organized code :)

void softError(char *msg) {
    printf("%s", msg);
}

void errorWithCode(char * msg, int code) {
    fprintf(stderr, "%s", msg);

    exit(code);
}

void error(char *msg) {
    errorWithCode(msg, -1);
}

void validateFile(FILE *file) {
    if (!file) {
        error("File not found!");
    }
}

void removeNewLine(char * string) {
    if ((strlen(string) > 0) && (string[strlen (string) - 1] == '\n')) // Remove last character if n line
        string[strlen (string) - 1] = '\0';
}

void getString(char * string, int limit)
{
    char empty;
    if((empty = getchar()) != '\n') { // Don't get the first character if new line
        string[0] = empty;
        string++;
        limit--;
    }

    fgets(string, limit, stdin);
    removeNewLine(string);

    fflush(stdin);
}