
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arguments.h"
#include "debug.h"
#include "onegin.h"
#include "utils.h"

int InputTextError(char *content, line *linesArray, size_t linesCount);
void Clear(char *content, line *linesArray, FILE *inputFile, FILE *outputFile);

int main(int argc, char **argv)
{
    int setConfigRes = SetConfig (argc, argv);
    if (setConfigRes != 0)
        return setConfigRes;
    config_t config = GetConfig();

    int status = 0;
    char *content = NULL;
    line *linesArray = NULL;
    size_t linesCount = 0;

    status = InputText (config.inputFile, &content, &linesArray, &linesCount);
    if (status != 0)
    {
        Clear (content, linesArray, config.inputFile, config.outputFile);

        return status;
    }
        
    status = InputTextError (content, linesArray, linesCount);
    if (status != 0)
    {
        Clear (content, linesArray, config.inputFile, config.outputFile);

        return status;
    }

    PRINT_CONTENT (content)
    PRINT_LINES_ARRAY (linesArray, linesCount)

    BubbleSortCmp (linesArray, linesCount, &AlphabeticalCmp);

    DEBUG ("%s", "PRINTING STRAIGHT SORTED POEM")

    PrintText (linesArray, "------------- STRAIGHT SORT -------------\n\n");

    RhymeSort (linesArray, linesCount, sizeof(line));

    DEBUG ("%s", "PRINTING REVERES SORTED POEM")

    PrintText (linesArray, "\n------------- REVERSED SORT -------------\n\n");

    DEBUG ("%s", "PRINTING ORIGINAL POEM")

    PrintTextOriginal (content, "\n------------- ORIGINAL POEM -------------\n\n");

    Clear (content, linesArray, config.inputFile, config.outputFile);

    return status;
}

int InputTextError (char *content, line *linesArray, size_t linesCount)
{
    if (content == NULL)
    {
        ERROR("%s", "File content is empty!");
        return 1;
    }
    if (linesArray == NULL)
    {
        ERROR("%s", "linesArray structure is NULL");
        return 1;
    }
    if (linesCount == 0)
    {
        ERROR("%s", "File doesn't contain lines");
        return 1;
    }

    return 0;
}

void Clear (char *content, line *linesArray, FILE *inputFile, FILE *outputFile)
{
    free(content);
    free(linesArray);
    fclose(inputFile);
    fclose(outputFile);
}