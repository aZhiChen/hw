#include<iostream>
#include<stdlib.h>
#include <bits/stdc++.h>
using namespace std;
#define N 50000
#define M 99
int main(){
    int res = 0;
    srand(time(NULL)); //设置随机种子，使每次产生的随机序列不同
    //模拟
    for( int i = 0; i < N; i++ ){
        float pa, pb, pc;
        pa =  rand() % (N+1) / (float)(N+1);
        pb = rand() % (N+1) / (float)(N+1);
        pc = rand() % (N+1) / (float)(N+1);
        if( pa < 0.85 || ( pb < 0.95 && pc < 0.90 ))
            res ++;
    }
    cout << "Reliability is " << (float) res/N << endl;
    system("PAUSE");
    return 0;
}