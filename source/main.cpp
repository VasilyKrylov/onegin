
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arguments.h"
#include "debug.h"
#include "onegin.h"
#include "utils.h"

extern struct config_t config;

int main(int argc, char **argv)
{
    int setConfigRes = SetConfig(argc, argv);
    if (setConfigRes != 0)
        return setConfigRes;

    size_t fileSize = GetFileSize (config.inputFile);
    if (fileSize == 0)
    {
        ERROR ("%s", "Empty input file or error while getting file size");
        return 1;
    }
    PRINT ("fileSize: %lu\n", fileSize);

    char *content = ReadFile (config.inputFile, fileSize);
    if (content == NULL)
    {
        ERROR ("%s: %s", "error allocating memory for content", strerror (errno));
        return 1;
    }

    size_t linesCount = CountLines (content, fileSize + 1);
    PRINT ("Total number of lines: %lu\n", linesCount);

    line *linesArray = (line *) calloc (linesCount + 1, sizeof(line));
    if (linesArray == NULL)
    {
        ERROR ("%s: %s", "error allocating memory for linesArray", strerror (errno));
        return 1;
    }

    size_t ptrsMade = MakePointers (content, linesArray);
    if (ptrsMade - 1 != linesCount)
    {
        ERROR ("%s", "number of strings in file and number of initialized pointers doesn't match!");
        ERROR ("%s", "One extra ptr in *linesArray for empty structure");
        ERROR ("%s %lu", "linesCount: ", linesCount);
        ERROR ("%s %lu", "ptrsMade: ", ptrsMade);

        return 1;
    }

    line *linesArrayOrig = (line *) calloc (linesCount + 1, sizeof(line));
    if (linesArrayOrig == NULL)
    {
        ERROR ("%s: %s", "error allocating memory for linesArrayOrig", strerror (errno));

        return 1;
    }
    linesArrayOrig = (line *) memcpy (linesArrayOrig, linesArray, (linesCount + 1) * sizeof(line));

    // -------------------------------------------- DEBUG --------------------------------------------
    PRINT ("%s", "\ncontent:\n");
    for (size_t i = 0; i < fileSize + 2; i++)
    {
        PRINT ("[%lu] = \t %d, \t '%c'\n", i, content[i], content[i]);
    }

    PRINT ("%s", "\nlinesArray:\n");
    for (size_t i = 0; i < linesCount; i++)
    {
        PRINT ("[%lu].len: %lu\n", i, linesArray[i].len);
        PRINT ("[%lu].start: %p\n", i, linesArray[i].start);
        PRINT ("[%lu] relative start: %ld\n", i, linesArray[i].start - linesArray[0].start);

        PRINT ("[%lu]: ", i);
        for (size_t j = 0; j < linesArray[i].len; ++j)
        {
            PRINT ("\t [%lu]: '%c'\n", j, *(linesArray[i].start + j));
        }
        PRINT ("%s", "\n");
    }
    // -----------------------------------------------------------------------------------------------

    BubbleSortCmp (linesArray, linesCount, &Cmp);

    FILE *outputFile = fopen ("output.txt", "w");
    if (outputFile == NULL)
    {
        ERROR ("%s %s", "Error opening input file", strerror( errno));
    }

    DEBUG ("%s", "PRINTING STRAIGHT SORTED POEM");

    PrintText (linesArray,      "------------- STRAIGHT SORT -------------\n\n");

    RhymeSort (linesArray, linesCount, sizeof(line));

    DEBUG ("%s", "PRINTING REVERES SORTED POEM");

    PrintText (linesArray,      "\n------------- REVERSED SORT -------------\n\n");

    DEBUG ("%s", "PRINTING ORIGINAL POEM");

    PrintText (linesArrayOrig, "\n------------- ORIGINAL POEM -------------\n\n");

    free (content);
    free (linesArray);
    free (linesArrayOrig);

    fclose (config.inputFile); // should I check and this return code?
    fclose (outputFile);

    return 0;
}
