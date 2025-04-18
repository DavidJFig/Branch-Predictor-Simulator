// Author: David Figueras-Sosa
// Project 2: Branch Predictor Simulator

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Constants


// Global variables
int gshare; // global history register size
int GPB; // global pattern buffer size
int RB; // return buffer size




// Global variables to maintain the simulation statistics



void simulatePrediction(unsigned long long int address, char taken)
{
    return;
}



int main(int argc, char **argv)
{
    // check if the number of arguments is correct
    if (argc != 4) {
        printf("Error: Must run with 4 arguments: 'gshare' <GPB> <RB> <Trace_File>\n");
        return 1;
    }

    // read the cache configuration from the command line arguments
    gshare = atoi(argv[1]);
    GPB = atoi(argv[2]);
    RB = atoi(argv[3]);
    char* TRACE_FILENAME = argv[4];


    // print out the command line arguments
    printf("gshare: %d\n", gshare);
    printf("GPB: %d\n", GPB);
    printf("RB: %d\n", RB);
    printf("Trace file: %s\n", TRACE_FILENAME);



    // check if the trace file opened successfully
    FILE *file = fopen(TRACE_FILENAME, "r");

    if (!file) {
        printf("Error: Could not read the trace file. Check file name.\n");
        return 1;
    }

    unsigned long long int address;
    char taken;

    // read until end of file
    while (!feof(file)) {
        // read operation and address
        fscanf(file, "%llx %c", &address, &taken);
        printf("Address: %llx\n", address);
        printf("Taken: %c\n", taken);

        // simulate each branch prediction
        simulatePrediction(address, taken);
    }

    // close the trace file
    fclose(file);

    // free the allocated memory
    

    // print out the statistics

    return 0;
}