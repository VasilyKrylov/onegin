#include <stdio.h>

#include "debug.h"
#include "onegin.h"

void PrintContent (char *content)
{
    PRINT ("%s", "\ncontent:\n")
    for (size_t i = 0; content[i] != '\0'; i++)
    {
        PRINT ("[%lu] = \t %d, \t '%c'\n", i, content[i], content[i])
    }
}

void PrintLinesArray (line *linesArray, size_t linesCount)
{
    PRINT ("%s", "\nlinesArray:\n")
    for (size_t i = 0; i < linesCount; i++)
    {
        PRINT ("[%lu].len: %lu\n", i, linesArray[i].len)
        PRINT ("[%lu].start: %p\n", i, linesArray[i].start)
        PRINT ("[%lu] relative start: %ld\n", i, linesArray[i].start - linesArray[0].start)

        PRINT ("[%lu]: ", i);
        for (size_t j = 0; j < linesArray[i].len; ++j)
        {
            PRINT ("\t [%lu]: '%c'\n", j, *(linesArray[i].start + j))
        }
        PRINT ("%s", "\n")
    }
}