#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "arguments.h"
#include "config.h"
#include "debug.h"
#include "utils.h"

extern config_t config;

static struct option longOptions[] = {
    {"input",   required_argument,  NULL,  'i'},
    {"output",  required_argument,  NULL,  'o'},
    {"help",    no_argument,        NULL,  'h'},
    {"debug",   no_argument,        NULL,  'd'},
    {0,         0,                  0,       0}
};
static const char shortOptions[] = "i:o:hd";

static char defaultInput[]  = "text_onegin.txt";
static char defaultOutput[] = "output.txt";

static const char helpMessage[] = "Usage: program [OPTION] [FILE]\n"
                                  "\n"
                                  "1) Read strings of input file (default is \"textonegin.txt\")\n"
                                  "2) Sort them in alpabetic and rhyme order\n"
                                  "3) Print result to output file (default is \"output.txt\")\n"
                                  "\n"
                                  "\t -h, --help              \t\t\t print this help message\n"
                                  "\t -i, --input [FILE_NAME] \t\t\t specify input file (default is text_onegin.txt)\n"
                                  "\t -o, --output [FILE_NAME] \t\t\t specify input file (default is output.txt)\n"
                                  "\n"
                                  "Exit status:\n"
                                  "0 if OK,"
                                  "1 if any error occur\n";

int CheckArgs (int argc, char *argv[]);
int InitializeFiles (char *inputFileName, char *outputFileName);

int InitializeFiles (char *inputFileName, char *outputFileName)
{
    if (inputFileName == NULL)
    {
        inputFileName = defaultInput;
    }

    config.inputFile = fopen (inputFileName, "r");
    if (config.inputFile == NULL)
    {
        ERROR ("%s: %s", "Error opening input file", strerror (errno));

        return -1;
    }
    
    if (outputFileName == NULL)
    {
        outputFileName = defaultOutput;
    }

    config.outputFile = fopen (outputFileName, "w");
    if (config.outputFile == NULL)
    {
        ERROR ("%s: %s", "Error opening output file", strerror (errno));
        
        return -1;
    }

    return 0;
}

int CheckArgs (int argc, char *argv[])
{
    if (optind < argc) {
        printf ("Not correct command line arguments");

        while (optind < argc)
        {
            optind++;
            printf ("%s ", argv[optind]);
        }
        printf ("\n");
        
        return 1;
    }
    
    return 0;
}

// 0 ok
// 1 some error
int SetConfig (int argc, char *argv[])
{
    char *inputFileName = NULL;
    char *outputFileName = NULL;

    int c = '\0';
    int status = 0;

    DEBUG ("inputFileName = %p", inputFileName);

    while ((c = getopt_long (argc, argv, shortOptions, longOptions, NULL)) != -1) 
    {
        DEBUG ("Option %c has value %s\n", c, optarg);   

        if (c == -1)
            break;

        switch (c) 
        {
            case 'i':
                if (inputFileName != NULL)
                {
                    printf ("Multiple definitions of input file!");
                    status = 1;

                    goto exit;
                }
                inputFileName = strdup (optarg);
                DEBUG ("inputFileName = %p", inputFileName);
                break;
            case 'o':
                if (outputFileName != NULL)
                {
                    printf ("Multiple definitions of output file!");
                    status = 1;

                    goto exit;
                }
                outputFileName = strdup (optarg);
                break;
            case 'h':
                printf ("%s", helpMessage);

                free (inputFileName);
                free (outputFileName);

                exit (0);
            case 'd':
                config.debug = 1;
                break;
            case '?':
                ERROR ("Error in getopt_long(), returned %c : %s", c, strerror(errno));
                status = 1;

                goto exit;
            default:
                ERROR ("%s", "This should never happen i guess");
                ERROR ("getopt_long returned: %d = '%c'", c, c);

                status = 1;
                goto exit;
        }
    }

    status = InitializeFiles (inputFileName, outputFileName);
    if (status != 0)
        goto exit;

    status = CheckArgs (argc, argv);
    if (status != 0)
        goto exit;

exit:
    free (inputFileName);
    free (outputFileName);
    return status;
}

// int SetConfig (config *config, int argc, char **argv)
// {
//     assert (config);

//     DEBUG ("argc %d", argc);

//     if (argc <= 1)
//     {
//         return 0;
//     }

//     for (int i = 0; i < argc; i++)
//     {
//         DEBUG ("argv[%d] = \"%s\"", i, argv[i]);

//         if (strcmp (argv[i], "-h") == 0 || strcmp (argv[i], "--help") == 0)
//         {
//             printf ("%s", helpMessage);
            
//             if (i + 1 < argc)
//             {
//                 printf ("Command line arguments after \"%s\" ignored\n", argv[i]);
//             }

//             return 0;
//         }

//         if (strcmp (argv[i], "-i") == 0 || strcmp (argv[i], "--input") == 0)
//         {
//             if (i + 1 < argc)
//             {
//                 printf ("%s", "Please, cpecify input file in command line arguments\n");
//             }
//             else
//             {
//                 FILE *inputFile = fopen (argv[i + 1], "r");
//                 if (inputFile == NULL)
//                 {
//                     ERROR ("%s %s", "Error opening input file", strerror (errno));
//                     return 1;
//                 }
//             }
//         }
//     }

//     return 0;
// }