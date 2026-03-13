#include "../include/args.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

// Stops the program if the args have an error
void error_args(char* program_path) {
    printf("Correct usage: '%s -i {input_path} -o {output_path}'\n", program_path);
    exit(EXIT_FAILURE);
}

// Receives input_path and output_path from user in command line using flags
void get_args(char** input_path, char** output_path, int argc, char* argv[]) {
    *input_path = NULL;
    *output_path = NULL;

    int option;
    while ((option = getopt(argc, argv, "i:o:")) != -1) {
        switch (option) {
            case 'i':
                *input_path = optarg;
                break;
            case 'o':
                *output_path = optarg;
                break;
            default:
                error_args(argv[0]);
        }
    }

    if (*input_path == NULL || *output_path == NULL) {
        error_args(argv[0]);
    }
}