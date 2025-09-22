#ifndef K_DEBUG_H
#define K_DEBUG_H

#include "config.h"

extern config_t config;

#define RED_BOLD_COLOR "\33[1;31m"
#define BLUE_BOLD_COLOR "\33[1;34m" // blue
#define COLOR_END "\33[0m" // To flush out prev colors

#define ERROR(format, ...) fprintf(stderr, RED_BOLD_COLOR "[ERROR] %s:%d:%s: " format "\n" COLOR_END, __FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef PRINT_DEBUG
    #define DEBUG(format, ...)  { \
                                    if (config.debug) \
                                    { \
                                        fprintf(stderr, BLUE_BOLD_COLOR "[DEBUG] %s:%d:%s(): " format "\n" COLOR_END, \
                                                __FILE__, __LINE__, __func__, __VA_ARGS__); \
                                    } \
                                }

    #define PRINT(format, ...)  { \
                                    if (config.debug) \
                                    { \
                                        fprintf(stderr, BLUE_BOLD_COLOR format COLOR_END, __VA_ARGS__); \
                                    } \
                                }
                                
#endif // PRINT_DEBUG

#ifndef PRINT_DEBUG
    #define DEBUG(format, ...)
    #define PRINT(format, ...)
#endif // PRINT_DEBUG

#endif // K_DEBUG_H