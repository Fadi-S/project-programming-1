#ifndef PROJECTPROG_HELPERS_H
void validateFile(FILE *file);
void softError(char *msg);
void error(char *msg);
void errorWithCode(char *msg, int errorCode);
void getString(char *string, int limit);
#define PROJECTPROG_HELPERS_H

#endif //PROJECTPROG_HELPERS_H
