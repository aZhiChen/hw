//#include<cuda_fp16.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include<time.h>
#define TIMES 10
/*
该函数为核心计算函数
width为输入矩阵的宽
height为输入矩阵的高
input为输入的矩阵
output为输出的矩阵
*/
static __global__ void cudaBaseline(
    const int width, //输入矩阵的宽
    const int height, //输入矩阵的高
    const float * __restrict__ input, //输入矩阵
    float* __restrict__ output //输出矩阵
    ){
    const int idy = blockIdx.y * blockDim.y + threadIdx.y; //该线程对应的行坐标
    const int idx = blockIdx.x * blockDim.x + threadIdx.x; //该线程对应的列坐标
    if( idy < height && idx < width ){
        int cnt[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for(int dy = -2; dy <= 2; ++ dy ){
            int ty = idy + dy;
            if( ty >= 0 && ty < height ){
                for( int dx = -2; dx <= 2; ++ dx ){
                    int tx = idx + dx;
                    if( tx >= 0 && tx < width)
                        ++cnt[(int)input[ty * width + tx]];
                }
            }
        }
        //计算该块内元素个数, （左+中+右）*（上+中+下）
        double n = (min(2,idx) + 1 + min(2,width-idx)) * (min(2,idy) + 1 + min(2, height-idy));
        double n_div = 1/n;
        double ans = log(n); 
        for( int i = 0; i < 16; i++ ){
            if( cnt[i] )
                ans -= log((double)cnt[i]) * cnt[i] * n_div;
        }
        output[idy * width + idx] = ans;
    }
}


int main(int argc, char* argv[]){
    if( argc == 3 ){
        inputPath = argv[1];
        outputPath = argv[2];
    }
    //Open the input file
    FILE* stream = fopen(inputPath, "rb");
    if(stream == NULL){
        printf("failed to open the data file %s\n", inputPath);
        return -1;
    }
    
    //Open a stream to write out results in text
    FILE* outstream = fopen(outputPath, "wb");
    if(outstream == NULL){
        printf("failed to open the output file %s\n", outputPath);
        return -1;
    }
    
    //Read in and process the input matrix one-by-one
    int width, height, size;
    int *input;
    float *result;
    clock_t start, end;
    loadMatrix(stream, &width, &height, &input);
    size = width * height;
    printf("width = %d\n",width);
    printf("height = %d\n",height);
    printf("size = %d\n",size);
    result = (float*)malloc(sizeof(float)*size);
    
    float *input_d, *output_d;
    //在GPU上分配内存
    CHECK(cudaMalloc((void**)&output_d, sizeof(float) * size));
    CHECK(cudaMalloc((void**)&input_d, sizeof(float) * size));
    CHECK(cudaMemcpy( input_d, input, sizeof(float) * size, cudaMemcpyHostToDevice));
    
    const int BLOCK_DIM_X = 32;
    const int BLOCK_DIM_Y = 32;
    
    start = clock();
    float time = 0;
    for( int i = 0; i < TIMES; i++ ){
         dim3 blockDim(BLOCK_DIM_X, BLOCK_DIM_Y);
         dim3 gridDim(divup(width, BLOCK_DIM_X), divup(height, BLOCK_DIM_Y));
    
        cudaBaseline<<<gridDim, blockDim>>>(
            width, 
            height, 
            input_d,
            output_d);
        cudaDeviceSynchronize();
        end = clock();//计时结束  
        time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    //将结果写回host_result数组
    CHECK(cudaMemcpy(result, output_d, sizeof(float)*size, cudaMemcpyDeviceToHost));
    
    CHECK(cudaFree(input_d));
    CHECK(cudaFree(output_d));
    
    saveMatrix(outstream, &width, &height, &result);
    printf("Time of Cuda Baseline: %f\n",time/TIMES);
    
    free(input);
    free(result);
    input = NULL;
    result = NULL;
       
    fclose(outstream);
    fclose(stream);
    return 0;
}