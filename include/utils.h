/**
 * @file
 * @brief Usefull functions \n
 *        More detailed description will be in future
 */

#ifndef K_UTILS_H
#define K_UTILS_H

#define RED_BOLD_COLOR "\33[1;31m"
#define BLUE_BOLD_COLOR "\33[1;34m" // blue
#define COLOR_END "\33[0m" // To flush out prev colors

#define ERROR(format, ...) fprintf(stderr, RED_BOLD_COLOR "[ERROR] %s:%d:%s: " format "\n" COLOR_END, __FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef PRINT_DEBUG
#define DEBUG(format, ...) fprintf(stderr, BLUE_BOLD_COLOR "[DEBUG] %s:%d:%s: " format "\n" COLOR_END, __FILE__, __LINE__, __func__, __VA_ARGS__)
#define PRINT(format, ...) fprintf(stderr, BLUE_BOLD_COLOR format COLOR_END, __VA_ARGS__)
#endif // PRINT_DEBUG

#ifndef PRINT_DEBUG
#define DEBUG(format, ...)
#define PRINT(format, ...)
#endif // PRINT_DEBUG

/**
 * @brief Swap values of two size_t's
 * @param [in] a First number to swap
 * @param [in] b Second number to swap
 */
void Swap(size_t *a, size_t *b);
void LineSwap (line *a, line *b);

int Cmp (const void *param1, const void *param2);

// void BubbleSort (line *linesArray, size_t size);
void BubbleSortCmp (line *linesArray, size_t size, int (*Cmp)(const void *, const void *));
void RhymeSort (line *linesPrts, size_t n, size_t size);

#define MAX(x,y) ( (x) > (y) ? (x) : (y) )

#endif // K_UTILS_H
