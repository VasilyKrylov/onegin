#ifndef K_CONFIG_H
#define K_CONFIG_H

#include <stdio.h>

struct config_t {
    FILE *inputFile;
    FILE *outputFile;
    
    bool debug;
};

#endif // K_CONFIG_H