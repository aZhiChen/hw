#include <stdio.h>
#include <stdlib.h>
#include "config.h"

//**************************************************************************
//
//  Finish your code here if need.
//
//**************************************************************************

int main(int argc, char* argv[]) {
    if (argc == 3) {
        inputPath = argv[1];
        outputPath = argv[2];
    }

    // Open the input file
    FILE *stream = fopen(inputPath, "rb");
    if (stream == NULL) {
        printf("failed to open the data file %s\n", inputPath);
        return -1;
    }

    // Open a stream to write out results in text
    FILE *outStream = fopen(outputPath, "wb");
    if (outStream == NULL) {
        printf("failed to open the output file %s\n", outputPath);
        return -1;
    }

    // Read in and process the input matrix one-by-one
    int width, height, size;
    float *input1, *input2, *result;
    loadMatrix(stream, &width, &height, &input1);
    loadMatrix(stream, &width, &height, &input2);
    size = width * height;
    result = (float*)malloc(sizeof(float) * size);

    //**************************************************************************
    //
    //  Finish your code here.  Node that the array is 1D, so you should 
    //  visit the element of matrix with the way such as input[i*width+j].
    //
    //**************************************************************************

    





    saveMatrix(outStream, &width, &height, &result);

    // De-allocate the nput and the result
    free(input1);
    free(input2);
    input1 = input2 = NULL;
    free(result);
    result = NULL;
    

    // Close the output stream
    fclose(outStream);
    return 0;
}
