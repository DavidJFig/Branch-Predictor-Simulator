// Author: David Figueras-Sosa
// Project 2: Branch Predictor Simulator

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Constants


// Global variables
char* gshare; // gshare mode

int M; // number of bits to index the gshare table
int N; // number of bits for the global history register

int GHR; // global history register
int *predictionTable; // holds the taken / not taken prediction of each entry






// Global variables to maintain the simulation statistics



void predictBranch(unsigned long long int address, bool taken)
{
    return;
}



int main(int argc, char **argv)
{
    // check if the number of arguments is correct
    if (argc != 5) {
        printf("Error: Must run with 4 arguments: 'gshare' <GPB> <RB> <Trace_File>\n");
        return 1;
    }

    // read the cache configuration from the command line arguments
    gshare = argv[1];
    M = atoi(argv[2]);
    N = atoi(argv[3]);
    char* TRACE_FILENAME = argv[4];


    // print out the command line arguments
    printf("gshare: %s\n", gshare);
    printf("GPB (M): %d\n", M);
    printf("RB (N): %d\n", N);
    printf("Trace file: %s\n", TRACE_FILENAME);



    // check if the trace file opened successfully
    FILE *file = fopen(TRACE_FILENAME, "r");

    if (!file) {
        printf("Error: Could not read the trace file. Check file name.\n");
        return 1;
    }


    // initialize the global history register and the prediction table
    GHR = 0; 
    predictionTable = (int*) malloc((1 << M) * sizeof(int)); // 2^M entries

    for (int i = 0; i < (1 << M); i++) {
        predictionTable[i] = 2; // initialize all entries to weakly taken
    }


    unsigned long long int address;
    char takenChar; // 't' or 'n'
    bool taken; // true for taken, false for not taken

    // read until end of file
    while (!feof(file)) {
        // read operation and address
        fscanf(file, "%llx %c", &address, &takenChar);

        taken = (takenChar == 't') ? true : false; // convert char to bool        

        // simulate each branch prediction
        predictBranch(address, taken);
    }

    // close the trace file
    fclose(file);

    // free the allocated memory
    free(predictionTable);

    // print out the statistics

    return 0;
}