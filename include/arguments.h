#ifndef K_ARGUMENTS_H
#define K_ARGUMENTS_H

#include <stdio.h>

struct config_t {
    FILE *inputFile;
    FILE *outputFile;
    
    bool debug;
};

int SetConfig (int argc, char **argv);
config_t GetConfig();

#endif // K_ARGUMENTS_H