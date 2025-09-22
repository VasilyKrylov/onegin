/**
 * @file
 * @brief Usefull functions \n
 *        More detailed description will be in future
 */

#ifndef K_UTILS_H
#define K_UTILS_H

#include "onegin.h"

/**
 * @brief Swap values of two size_t's
 * @param [in] a First number to swap
 * @param [in] b Second number to swap
 */
void Swap (size_t *a, size_t *b);
void LineSwap (line *a, line *b);

int Cmp (const void *param1, const void *param2);

void BubbleSortCmp (line *linesArray, size_t size, int (*Cmp)(const void *, const void *));
void RhymeSort (line *linesPrts, size_t n, size_t size);

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )

#endif // K_UTILS_H
