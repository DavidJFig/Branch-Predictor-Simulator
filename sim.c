// Author: David Figueras-Sosa
// Project 2: Branch Predictor Simulator

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



// Global variables
char* gshare; // gshare mode, not used (it's always gshare)

int M; // number of bits to index the gshare table
int N; // number of bits for the global history register

unsigned int GHR; // global history register
int *predictionTable; // holds the taken / not taken prediction of each entry




// Simulation statistics
int mispredictions = 0; // number of mispredictions
int predictions = 0; // total number of predictions made


void predictBranch(unsigned long long int address, bool takenOutcome)
{
    // remove the first 2 bits of the address
    address = address >> 2; 


    // get the index of the prediction table using the GHR and the address
    unsigned long long int index;
    if (N > 0)
    {
        index = (address ^ (GHR << (M - N)));
    }
    else
    {
        index = address;
    }

    // take the last M bits of the index
    index = index & ((1 << M) - 1); 

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

    // update statistics
    if (prediction != takenOutcome)
    {
        mispredictions++; 
    }
    predictions++; 

    // update the GHR
    GHR = GHR | ((takenOutcome == true ? 1 : 0) << N);
    GHR = GHR >> 1;   
}



int main(int argc, char **argv)
{
    // check if the number of arguments is correct
    if (argc != 5) {
        printf("Error: Must run with 4 arguments: 'gshare' <GPB> <RB> <Trace_File>\n");
        return 1;
    }

    // read the configuration from the command line arguments
    gshare = argv[1];
    M = atoi(argv[2]);
    N = atoi(argv[3]);
    char* TRACE_FILENAME = argv[4];


    // print out the command line arguments
    // printf("gshare: %s\n", gshare);
    // printf("GPB (M): %d\n", M);
    // printf("RB (N): %d\n", N);
    // printf("Trace file: %s\n", TRACE_FILENAME);



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

    // printf("Prediction table initialized with %d entries.\n", (1 << M));

    unsigned long long int address;
    char takenChar; // 't' or 'n'
    bool taken; // true for taken, false for not taken

    // read until end of file
    while (!feof(file)) {
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
    printf("%d %d %.2f\n", M, N, (float)mispredictions / predictions * 100.0);

    return 0;
}