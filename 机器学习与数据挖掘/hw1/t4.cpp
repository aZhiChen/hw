#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
using namespace std;
#define N 7

bool canVisit[N][N];
bool next_step[4]; //0 for i-1, 1 for i+1, 2 for j-1, 3 for j+1

int check(int i, int j); //检查此状态下可以选择的个数,这取决于其邻接点有几个是允许访问的
int choice( int n ); //从可选的动作里随机选择一个动作
bool loop(); // return true if it achieve the end point, else return false

int check( int i, int j ){
    int res = 0;
    if( i - 1 >= 0 && canVisit[i-1][j] ){
        res ++;
        next_step[0] = true;
    }else next_step[0] = false;
    if( i + 1 < N && canVisit[i+1][j] ){
        res ++;
        next_step[1] = true;
    }else next_step[1] = false;
    if( j - 1 >= 0 && canVisit[i][j-1] ){
        res ++;
        next_step[2] = true;
    }else next_step[2] = false;
    if( j + 1 < N && canVisit[i][j+1] ){
        res ++;
        next_step[3] = true;
    }else next_step[3] = false;
    return res;
}

//传进去的n一定大于等于1
int choice( int n ){
    if( n == 1 ) {
        for( int i = 0; i < 4; i++ ){
            if( next_step[i] )
                return i;
        }
    }
    int n_rand = rand() % n + 1;
    int i = 0;
    for( int k = 0; i < 4 && k < n_rand; i++ ){
        if( next_step[i] ) k++;
    }
    return i-1;
}

bool loop(){
    for( int i = 0; i < N; i++ ){
        for (int j = 0; j < N; j++){
            canVisit[i][j] = true;
        }
    }
    int x = 0, y = 0;
    int times_cen = 0;
    int num_choice;
    num_choice = check(x,y);
    while( num_choice > 0 ){
        if( !( x == 3 && y == 3 ) ) { //if the point is not the center
            canVisit[x][y] = false;
        }
        else{
            times_cen ++;
            if( times_cen == 2 ) canVisit[x][y] = false;
        } 
        int act;
        act = choice(num_choice);
        switch( act ){
            case 0: 
                x--;
                break;
            case 1:
                x++;
                break;
            case 2:
                y--;
                break;
            case 3:
                y++;
                break;
        }
        if( x == N-1 && y == N-1 ) return true;
        num_choice = check(x,y);
    }
    return false;
}

int main(){
    int runTimes = 20000;
    int res = 0;
    for ( int i = 0; i < runTimes; i++){
        if( loop() ) res ++;
    } 
    cout << "P is " << (float) res / runTimes << endl;
    system("PAUSE");
    return 0;
}
