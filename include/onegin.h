#ifndef K_ONEGIN_H
#define K_ONEGIN_H

#include <stdio.h>

struct line {
    char *start = NULL;
    size_t len = 0;
};

// get file size using fseek
// stat() is better
size_t GetFileSize (FILE *file);
size_t CountLines (char *content, size_t fileSize);
char *ReadFile (FILE *file, size_t fileSize);
size_t MakePointers (char *content, line *linesArray);
void PrintText(line *linesArray, const char *message);

#endif // K_ONEGIN_H
