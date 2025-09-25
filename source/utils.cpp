#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "onegin.h"
#include "utils.h"

char *skipNotAlphabet (char *s);
char *skipNotAlphabetReversed (char *startPtr, char *s);
int RhymeCmp (const void *param1, const void *param2);

void Swap (size_t *a, size_t *b)
{
    assert (a);
    assert (b);

    size_t c = *b;
               *b = *a;
                    *a = c;
}

void LineSwap (line *a, line *b)
{
    assert (a);
    assert (b);

    line c = *b;
             *b = *a;
                  *a = c;
}

// skip not alphabetic letters.
// doesn't skip \n
char *skipNotAlphabet (char *s)
{
    assert (s);

    while (!isalpha (*s) && *s != '\n' && *s != '\0')
    {
        s++;
    }
    return s;
}

char *skipNotAlphabetReversed (char *startPtr, char *s)
{
    assert (startPtr);
    assert (s);

    while (!isalpha (*s) && s != startPtr)
    {
        s--;
    }
    return s;
}

int Cmp (const void *param1, const void *param2)
{
    assert (param1);
    assert (param2);

    const line *left  = (const line *)param1;
    const line *right = (const line *)param2;

    char *str1 = left  -> start;
    char *str2 = right -> start;

    while (1)
    {
        str1 = skipNotAlphabet (str1);
        str2 = skipNotAlphabet (str2);

        char val1 = (char)tolower (*str1);
        char val2 = (char)tolower (*str2);

        if (val1 > val2)
            return 1;

        if (val1 < val2)
            return -1;

        if (str1 == left->start + left->len)
            break;

        if (str2 == right->start + right->len)
            break;

        str1++;
        str2++;
    }

    return 0;
}

void BubbleSortCmp (line *linesArray, size_t size, int (*Cmp)(const void *, const void *))
{
    assert (linesArray);
    assert (Cmp);

    DEBUG ("size: %lu", size)


    for (size_t i = 0; linesArray[i].start != NULL; i++)
    {
        bool swapped = false;
        
        DEBUG ("i: %lu", i)

        for (size_t j = 1; j < size - i; j++)
        {
            DEBUG ("\tj: %lu", j)

            int comparison = (*Cmp) ((void *)&linesArray[j - 1], (void *)&linesArray[j]);

            if (comparison > 0)
            {
                DEBUG ("\t\tswapping elements [%lu], [%lu]", j - 1, j)

                LineSwap (&linesArray[j - 1], &linesArray[j]);

                swapped = true;
            }

            DEBUG ("\t\tCmp() returned %d for elements [%lu] [%lu]", comparison, j - 1, j)
        }

        if (!swapped) break;
    }

    PRINT("%s", "\n")
}


int RhymeCmp (const void *left, const void *right)
{
    assert (left);
    assert (right);

    const line *l1 = (const line *)left;
    const line *l2 = (const line *)right;

    // + len is next char
    // + len - 1 is \n
    char *str1 = l1->start + l1->len - 1;
    char *str2 = l2->start + l2->len - 1;

    DEBUG ("l1 -> start is %p", l1 -> start)
    DEBUG ("l2 -> start is %p", l2 -> start)

    while (1)
    {
        DEBUG ("\t\toldVal1 is : '%c'", *(str1))
        DEBUG ("\t\toldVal2 is : '%c'", *(str2))

        str1 = skipNotAlphabetReversed (l1 -> start, str1);
        str2 = skipNotAlphabetReversed (l2 -> start, str2);

        char val1 = (char)tolower (*str1);
        char val2 = (char)tolower (*str2);

        DEBUG ("\t\tval1 is : '%c'", val1)
        DEBUG ("\t\tval2 is : '%c'", val2)

        DEBUG ("\t\trelative position: %ld", str1 - l1->start)
        DEBUG ("\t\trelative position: %ld", str2 - l2->start)

        if (val1 > val2)
            return 1;

        if (val1 < val2)
            return -1;

        if (str1 == l1 -> start)
            return 1;

        if (str2 == l2 -> start)
            return -1;

        str1--;
        str2--;

        assert (str1 >= l1 -> start);
        assert (str2 >= l2 -> start);
    }

    DEBUG (" return 0 for '%c' and '%c'", *(l1 -> start), *(l2 -> start))

    return 0;
}

void RhymeSort (line *linesPrts, size_t n, size_t size)
{
    assert (linesPrts);

    qsort (linesPrts, n, size, RhymeCmp);
}
