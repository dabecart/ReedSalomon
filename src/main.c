/***************************************************************************************************
 * @file main.c
 * @brief Launch point for the program.
 *
 * @version   1.0
 * @date      2024-07-23
 * @author    @dabecart
 *
 * @license
 * This project is licensed under the MIT License - see the LICENSE file for details.
 **************************************************************************************************/

#include "SimulationTools.h"
#include "FileTools.h"

#define DEFAULT_TOTAL_TESTS 10000
#define DEFAULT_MIN_ERRORS  0
#define DEFAULT_MAX_ERRORS  EXTRA_POINTS
#define DEFAULT_OUT_ENCODE  "encode.out"
#define DEFAULT_OUT_VERIFY  "fixed.out"

void print_help(const char* programName){
    printf("Usage: %s [-h] [-t <TOTAL> <MIN> <MAX>] [-e <FILE> <OUTPUT>] -v <DATA> <REC> <OUTPUT>\n\n", 
            programName);

    printf("This program error proofs files with an error correction algorithm based on the\n"
           "Reed-Salomon's algorithm. You may use this as a tesbench for the algorithm with [-t]\n"
           "or to run it on your own files with [-e]. Try corrupting the original file and then\n"
           "recover the original data with [-v].\n");
    
    printf("\nOptions:\n"
           "  -h  --help\n"
           "                          Print this help message.\n\n"
           
           "  -t [<TOTAL> <MIN> <MAX>]  --testbench [<TOTAL> <MIN> <MAX>]\n"
           "                          Run the algorithm with random data a <TOTAL> of times, with\n"
           "                          a minimum of <MIN> errors and a maximum of <MAX> errors.\n"
           "                          By default, it runs a <TOTAL> of %d times, with an error\n"
           "                          count of rand(<MIN> = %d,  <MAX> = %d).\n\n"
           
           "  -e <FILE> [<OUTPUT>]  --encode <FILE> [<OUTPUT>]\n"
           "                          Create the recuperation file for a given <FILE>. You may \n"
           "                          also specify the <OUTPUT> file (by default: %s).\n\n"

           "  -v <DATA> <REC> [<OUTPUT>]  --verify <DATA> <REC> [<OUTPUT>]\n"
           "                          Recuperate a <DATA> file using the <REC>uperation file. You\n"
           "                          may also specify the <OUTPUT> file (by default: %s).\n",
           DEFAULT_TOTAL_TESTS, DEFAULT_MIN_ERRORS, DEFAULT_MAX_ERRORS, 
           DEFAULT_OUT_ENCODE, DEFAULT_OUT_VERIFY);

    printf("\nCreated under MIT license by @dabecart, 2024.\n");
}

/***************************************************************************************************
 * MAIN
 **************************************************************************************************/
int main(int argc, char *argv[]){
    int totalTests  = DEFAULT_TOTAL_TESTS;
    int minErrors   = DEFAULT_MIN_ERRORS;
    int maxErrors   = DEFAULT_MAX_ERRORS;

    if (argc == 1) print_help(argv[0]);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
            print_help(argv[0]);
        }else if (strcmp(argv[i], "-t") == 0){
            if (i + 1 < argc) totalTests = atoi(argv[++i]);
            if (i + 1 < argc) minErrors = atoi(argv[++i]);
            if (i + 1 < argc) maxErrors = atoi(argv[++i]);
            testBench(totalTests, minErrors, maxErrors);
            return 0;

        }else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--encode") == 0){
            if (i + 2 < argc){
                i++;
                createRecuperationFile(argv[i], argv[i+1]);
            }else if (i + 1 < argc){
                i++;
                createRecuperationFile(argv[i], DEFAULT_OUT_ENCODE);
            }else{
                fprintf(stderr, "Error: -e requires a file path\n");
                return 1;
            }
            return 0;

        }else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verify") == 0){
            if(i + 3 < argc){
                i++;
                recuperateFile(argv[i], argv[i+1], argv[i+2]);
            }else if(i + 2 < argc){
                i++;
                recuperateFile(argv[i], argv[i+1], DEFAULT_OUT_VERIFY);
            }else{
                fprintf(stderr, "Error: -v requires two file paths\n");
                return 1;
            }
            return 0;

        }else{
            fprintf(stderr, "Unknown argument: %s\nUse -h for the help menu.\n", argv[i]);
            return 1;
        }
    }

    return 1;
}