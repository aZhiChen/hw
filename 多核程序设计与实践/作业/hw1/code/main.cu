#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include<time.h>
//**************************************************************************
//
//  Finish your code here if need.
//
//**************************************************************************
//1维
__global__ void Matrix_add(float* A, float* B, float* C, int n){
    int tid = blockDim.x * blockIdx.x + threadIdx.x;
    if( tid <  n)
        C[tid] = A[tid] + B[tid];
}

int divup( int n, int m ){
    return (if(n%m)? (n/m+1):(n/m));
}

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
    clock_t start,end;
    loadMatrix(stream, &width, &height, &input1);
    loadMatrix(stream, &width, &height, &input2);
    size = width * height;
    result = (float*)malloc(sizeof(float) * size);
    int block_size;
    printf("Please enter block_size!\n");
    scanf("%d\n",block_size);
    //**************************************************************************
    //
    //  Finish your code here.  Node that the array is 1D, so you should 
    //  visit the element of matrix with the way such as input[i*width+j].
    //
    //**************************************************************************
    float *input1_d, *input2_d, *result_d;
    int n_bytes = sizeof(float)*size;
    cudaMalloc((void**)&input1_d, sizeof(float)*size);
    cudaMalloc((void**)&input2_d, sizeof(float)*size);
    cudaMalloc((void**)&result_d, sizeof(float)*size);
    start=clock();//开始计时
    cudaMemcpy(input1_d, input1, n_bytes, cudaMemcpyHostToDevice);
    cudaMemcpy(input2_d, input2, n_bytes, cudaMemcpyHostToDevice);
    Matrix_add<<<divup(size, block_size), block_size>>>(input1_d, input2_d, result_d);
    cudaDeviceSynchronize();
    cudaMemcpy( result, result_d, n_bytes, cudaMemcpyDeviceToHost);
    end = clock();//计时结束
    cudaFree(result_d);
    cudaFree(input1_d);
    cudaFree(input2_d);

    saveMatrix(outStream, &width, &height, &result);
    float time1=(float)(end-start)/CLOCKS_PER_SEC;
    printf("Time of GPU: %f\n", time1);
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
