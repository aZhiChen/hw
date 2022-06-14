#include<stdio.h>
#include<stdlib.h>
#define SIZE 16384
int main(){
    FILE *pd = NULL;
    int a;
    //a = (int*)malloc(4096*4096);

    pd = fopen("input5.bin","wb");
    int w = SIZE;
    fwrite(&w,sizeof(int),1,pd);
    fwrite(&w,sizeof(int),1,pd);
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            a = rand()%16;
            fwrite(&a,sizeof(int),1,pd);
        }
    }
    
    fclose(pd);
    return 0;
}
