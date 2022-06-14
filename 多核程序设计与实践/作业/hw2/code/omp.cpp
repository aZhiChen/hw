#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<cmath>
#define TIMES 10

using namespace std;
//**************************************************************************
//
//  omp用寄存器存储对数表
//
//**************************************************************************

int main(int argc, char* argv[]) { 
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
    float time = 0;
    loadMatrix(stream, &width, &height, &input);
    size = width * height;
    printf("width = %d\n",width);
    printf("height = %d\n",height);
    printf("size = %d\n",size);
    result = (float*)malloc(sizeof(float)*size);
    start = clock();
    for(int i = 0; i < TIMES; i++){
    #pragma omp parallel for
    for(int pos = 0; pos < size; pos++){
        const int idy = pos / width;
        const int idx = pos - idy * width;
        int cnt[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        for(int dy = -2; dy <= 2; dy++){
            const int py = dy + idy;
            if(py >= 0 && py < height){
                for(int dx = -2; dx <= 2; dx ++){
                    const int px = dx + idx;
                    if( px >= 0 && px < width )
                        cnt[(int)input[py * width + px]]++;
                }
            }
        }
        //用寄存器存储对数表
        const double logcons[26]={
            0.0,
            log(1.0),
            log(2.0),
            log(3.0),
            log(4.0),
            log(5.0),
            log(6.0),
            log(7.0),
            log(8.0),
            log(9.0),
            log(10.0),
            log(11.0),
            log(12.0),
            log(13.0),
            log(14.0),
            log(15.0),
            log(16.0),
            log(17.0),
            log(18.0),
            log(19.0),
            log(20.0),
            log(21.0),
            log(22.0),
            log(23.0),
            log(24.0),
            log(25.0)
        };
        //计算该块内元素个数, （左+中+右）*（上+中+下）
        double n = (min(2,idx) + 1 + min(2,width-idx)) * (min(2,idy) + 1 + min(2, height-idy));
        double n_div = 1/n;
        double ans = logcons[(int)n]; 
        for( int i = 0; i < 16; i++ ){
            if( cnt[i] )
                ans -= logcons[((int)cnt[i])] * cnt[i] * n_div;
        }
        result[idy * width + idx] = ans;
    }
    end = clock();
    time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    
    saveMatrix(outstream, &width, &height, &result);
    printf("Time of OpenMP: %f\n",time/TIMES);
    
    free(input);
    free(result);
    input = NULL;
    result = NULL;
       
    fclose(outstream);
    fclose(stream);
    return 0;
}