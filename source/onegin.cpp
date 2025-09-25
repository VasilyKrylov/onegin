#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "arguments.h"
#include "onegin.h"
#include "debug.h"
#include "utils.h"

size_t GetFileSize (FILE *file)
{
    if (file == NULL)
        return 0;

    int fseekRes = fseek (file, 0, SEEK_END);
    if (fseekRes != 0)
    {
        ERROR ("%s %s", "Error in fseek(): ", strerror (errno));
    }

    long fileSize = ftell (file);
    if (fileSize == -1)
    {
        ERROR ("%s %s", "Error in ftell(): ", strerror (errno));
    }

    rewind (file);
    return (size_t) fileSize;
}

// creat content buffer fileSize + 2
// content[fileSize] = '\n'
// content[fileSize + 1] = '\0'
char *ReadFile (FILE *file, size_t fileSize)
{
    if (file == NULL)
    {
        return NULL;
    }

    char *content = (char *) calloc (fileSize + 2, sizeof(char));

    if (content == NULL)
    {
        perror ("Failed to allocate memory for text");
        return NULL;
    }

    size_t bytesRead = fread (content, 1, fileSize, file);

    if (bytesRead != fileSize)
    {
        ERROR ("fread() status code(how many bytes read) is: %lu", bytesRead);
        ERROR ("fileSize is: %lu", fileSize);

        free (content);

        return NULL;
    }

    content[fileSize] = '\n';
    content[fileSize + 1] = '\0';

    return content;
}

// counts lines using lineSeparator
size_t CountLines (char *content, size_t fileSize, char lineSeparator)
{
    assert(content);

    size_t lines = 0;

    for (size_t i = 0; i < fileSize; i++)
    {
        if (content[i] == lineSeparator)
        {
            lines++;
        }
    }

    return lines;
}

// innitializing linesArray array
// return number of initialized pointers
size_t MakePointers (char *content, line *linesArray, char lineSeparator)
{
    assert (content);
    assert (linesArray);

    size_t curIdx = 0;

    linesArray[0].start = content;

    for (size_t i = 0; content[i] != '\0'; i++)
    {
        if (content[i] == lineSeparator)
        {
            if (curIdx == 0)
            {
                linesArray[curIdx].len = i + 1;
            }
            else
            {
                const line prev = linesArray[curIdx - 1];
                linesArray[curIdx].len = size_t (&content[i] - (prev.start + prev.len) + 1);
            }

            linesArray[curIdx + 1].start = &content[i] + sizeof(char);

            curIdx++;
        }
    }
    linesArray[curIdx].start = NULL;

    return curIdx + 1;
}

void PrintText (line *linesArray, const char *message)
{
    config_t config = GetConfig();
    assert (config.outputFile);
    assert (linesArray);
    assert (message);

    fprintf (config.outputFile, "%s", message);

    DEBUG ("linesArray: %p", linesArray)

    for (size_t i = 0; linesArray[i].start != NULL; i++)
    {
        DEBUG ("linesArray[%lu].start = %p", i, linesArray[i].start)
        DEBUG ("linesArray[%lu].len = %lu", i, linesArray[i].len)

        fprintf (config.outputFile, "%.*s", (int)linesArray[i].len, (linesArray[i].start));
    }
}

void PrintTextOriginal (const char *content, const char *message)
{
    config_t config = GetConfig();
    assert (config.outputFile);
    assert (content);
    assert (message);

    fprintf (config.outputFile, "%s", message);

    fprintf (config.outputFile, "%s", content);
}

int InputText (FILE *inputFile, char **contentPtr, line **linesArrayPtr, size_t *linesCountPtr)
{
    size_t ptrsMade = 0;

    size_t fileSize = GetFileSize (inputFile);
    if (fileSize == 0)
    {
        ERROR ("%s", "Empty input file or error while getting file size");

        return 1;
    }
    PRINT ("fileSize: %lu\n", fileSize)

    *contentPtr = ReadFile (inputFile, fileSize);
    char *content = *contentPtr;
    if (content == NULL)
    {
        ERROR ("%s: %s", "Error allocating memory for content", strerror (errno));

        return 1;
    }

    const char lineSeparator = '\n';
    *linesCountPtr = CountLines (content, fileSize + 1, lineSeparator);
    size_t linesCount = *linesCountPtr;

    PRINT ("Total number of lines: %lu\n", linesCount);

    *linesArrayPtr = (line *) calloc (linesCount + 1, sizeof(line));
    line *linesArray = *linesArrayPtr;

    if (linesArray == NULL)
    {
        ERROR ("%s: %s", "Error allocating memory for linesArray", strerror (errno));

        return 1;
    }

    ptrsMade = MakePointers (content, linesArray, lineSeparator);
    if (ptrsMade - 1 != linesCount)
    {
        ERROR ("%s", "number of strings in file and number of initialized pointers doesn't match!");
        ERROR ("%s", "One extra ptr in *linesArray for empty structure");
        ERROR ("%s %lu", "linesCount: ", linesCount);
        ERROR ("%s %lu", "ptrsMade: ", ptrsMade);

        return 1;
    }

    DEBUG ("%s", "InputText() returns 0. Everything is good");

    return 0;
}