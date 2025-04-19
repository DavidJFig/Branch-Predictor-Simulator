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

unsigned int GHR; // global history register
int *predictionTable; // holds the taken / not taken prediction of each entry






// Global variables to maintain the simulation statistics
int mispredictions = 0; // number of mispredictions
int predictions = 0; // number of predictions made


void predictBranch(unsigned long long int address, bool takenOutcome)
{
    printf("Predicting branch for address: %llx\n", address);
    // remove the first 2 bits of the address
    address = address >> 2; 


    // get the index of the prediction table using the GHR and the address
    unsigned int index;
    if (N > 0)
    {
        index = (address ^ (GHR << (M - N)));
    }
    else
    {
        index = address;
    }

    bool prediction = predictionTable[index] >= 2; // true for taken, false for not taken

    // update the prediction table
    if (takenOutcome) {
        if (predictionTable[index] < 3) {
            predictionTable[index]++;
        }
    } else {
        if (predictionTable[index] > 0) {
            predictionTable[index]--;
        }
    }
    if (prediction != takenOutcome)
    {
        mispredictions++; // increment the number of mispredictions
    }
    predictions++; // increment the number of predictions made

    // update the GHR
    GHR = GHR & (1 << N);
    GHR = GHR >> 1;
    
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

    printf("Prediction table initialized with %d entries.\n", (1 << M));

    unsigned long long int address;
    char takenChar; // 't' or 'n'
    bool taken; // true for taken, false for not taken

    // read until end of file
    while (!feof(file)) {
        printf("Looping...\n");
        // read operation and address
        fscanf(file, "%llx %c", &address, &takenChar);
        // print out the operation and address
        printf("Operation: %c, Address: %llx\n", takenChar, address);

        taken = (takenChar == 't') ? true : false; // convert char to bool        

        // simulate each branch prediction
        predictBranch(address, taken);
    }

    // close the trace file
    fclose(file);

    // free the allocated memory
    free(predictionTable);

    // print out the statistics
    printf("Number of predictions: %d\n", predictions);
    printf("Number of mispredictions: %d\n", mispredictions);
    printf("Misprediction rate: %.2f%%\n", (float)mispredictions / predictions * 100.0);

    return 0;
}