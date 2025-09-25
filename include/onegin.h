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
size_t CountLines (char *content, size_t fileSize, char lineSeparator);
char *ReadFile (FILE *file, size_t fileSize);
size_t MakePointers (char *content, line *linesArray, char lineSeparator);
void PrintText(line *linesArray, const char *message);
void PrintTextOriginal (const char *content, const char *message);
int InputText (FILE *inputFilePtr, char **contentPtr, line **linesArray, size_t *linesCountPtr);

#endif // K_ONEGIN_H
