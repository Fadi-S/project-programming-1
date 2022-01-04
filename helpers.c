#include <stdio.h>
#include <stdlib.h>

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