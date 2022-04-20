#include<iostream>
#include<iomanip>
//#include<string.h>
#include<fstream>
#include<vector>
#include<sstream>
using namespace std;

vector<int> nums;
int index; // index for nums
vector<char> states;
char state;//当前状态
vector<char> characters;
int num; // 当前的编码
char a; // 当前字符
//1. Read message from LexicalAnalyzerOut.txt
//2. Produce key message for Parsing
vector<int> readLexicalAnalyzerOut( char* fileName ){
    ifstream infile(fileName);
    if( !infile ){
        cout << "Unable to open LexicalAnalyzerOut.txt" << endl;
        exit(EXIT_FAILURE);
    }
    ofstream outfile("./ParsingIn.txt");
    if( !outfile ){
        cout << "Unable to open outfile" << endl;
        exit(EXIT_FAILURE);
    }
    //infile.getline(); // 第一行
    string line;
    getline(infile,line);
    vector<int> nums;
    string tmp;  
    while( getline(infile, line) ){
        //getline(infile, line);
        //line = infile.getline();
        stringstream input(line);
        input >> tmp;
        input >> tmp;
        int i = stoi(tmp);
        nums.push_back(i); 
    }
    //for(int i = 0; i < nums.size(); i++){
     //   cout << nums[i] << endl;
    //}
    infile.close();
    outfile.close();
    return nums;
}

bool isW0(int num){
    if( num == 35 || num == 36 )
        return true;
    return false;
}

bool isW1( int num ){
    if( num == 37 || num == 38 )
        return true;
    return false;
}

bool isI(int num){
    return num == 0;
}

void num2char(){
    if( isW0(num) )
        a = 'W';
    else if( isW1(num) )
        a = 'w';
    else if( isI(num) )
        a = 'I';
    else if( num == 71 )
        a = '(';
    else if( num == 72 )
        a = ')';
    else if( num == 68 )
        a = '#';
}

void Getnum( ){
    if( index < nums.size() ){
        num = nums[index++];
    }
}

void nextw(){
    Getnum();
    num2char();
}

char Goto( char s, char sentence ){
    if( s == '0' ){
        if( sentence == 'E' ){
            return '1';
        }
        else if( sentence == 'T' ){
            return '2';
        }
        else if( sentence == 'F' ){
            return '3';
        }
    }
    else if( s == '4' ){
        if( sentence == 'E' ){
            return '8';
        }
        else if( sentence == 'T' ){
            return '2';
        }
        else if( sentence == 'F' ){
            return '3';
        }
    }
    else if( s == '6' ){
        if( sentence == 'T' ){
            return '9';
        }
        else if( sentence == 'F' )
            return '3';
    }
    else if( s == '7' ){
        if( sentence == 'F' )
            return 'a';
    }
    else 
        return 'n';  
}
//用16进制表示state
bool LR(){
    states.push_back('0');
    nextw();
    while(1){
        //打印
        cout << "states: ";
        for( int i = 0; i < states.size(); i++ ){
            cout << states[i] << ' ' ;
        }
        cout << endl;
        cout << "Characters: ";
        for( int i = 0 ; i < characters.size(); i++ ){
            cout << characters[i] << ' ' ;
        }
        cout << endl; 
        getchar();
        state = states.back();
        if( state == '0' ){
            if( a == 'I' ){
                states.push_back('5');
                characters.push_back(a);//移进
                nextw();
            }
            else if( a == '(' ){
                states.push_back('4');
                characters.push_back(a);//移进
                nextw();
            }
        }
        
        else if( state == '1' ){
            if( a == 'W' ){
                states.push_back('6');
                characters.push_back(a);
                nextw();
            }
            else if( a == '#' ){
                return true;
            }
            else{
                cout << "Error in state 1" << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '2' ){
            if( a == 'W' || a == ')' || a == '#' ){
                //规约E->T
                states.pop_back();
                char tmp = Goto(states.back(),'E');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.push_back('E');//规约
            }
            else if( a == 'w' ){
                //移进
                states.push_back('7');
                characters.push_back(a);
                nextw();
            }
            else{
                cout << "Error in state 2" << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '3' ){
            if( a == 'W' || a == 'w' || a == ')' || a == '#' ){
                //规约 T->F
                states.pop_back();
                char tmp = Goto(states.back(),'T');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.push_back('T');//规约
            }
            else{
                cout << "Error in state 3" << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '4' ){
            if( a == 'I' ){
                //移进
                states.push_back('5');
                characters.push_back(a);
                nextw();
            }
            else if( a == '(' ){
                //移进
                states.push_back('4');
                characters.push_back(a);
            }
            else{
                cout << "Error in state 4" << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '5' ){
            if( a == 'W' || a == 'w' || a == ')' || a == '#' ){
                //规约 F->I
                states.pop_back();
                char tmp = Goto(states.back(),'F');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.push_back('F');//规约
            }
            else{
                cout << "Error in state " << state << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '6' ){
            if( a == 'I' ){
                //移进
                states.push_back('5');
                characters.push_back(a);
                nextw();
            }
            else if( a == '(' ){
                //移进
                states.push_back('4');
                characters.push_back(a);
                nextw();
            }
            else{
                cout << "Error in state " << state << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '7' ){
            if( a == 'I' ){
                states.push_back('5');
                characters.push_back(a);
                nextw();
            }
            else if( a == '(' ){
                states.push_back('4');
                characters.push_back(a);
                nextw();
            }
            else{
                cout << "Error in state " << state << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '8' ){
            if( a == 'W' ){
                //移进
                states.push_back('6');
                characters.push_back(a);
                nextw();
            }
            else if( a == ')' ){
                states.push_back('b');
                characters.push_back(a);
                nextw();
            }
            else{
                cout << "Error in state " << state << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }

        else if( state == '9' ){
            if( a == 'W' || a == '#' || a == ')' ){
                //规约 E->EWT
                states.pop_back();
                states.pop_back();
                states.pop_back();
                char tmp = Goto(states.back(),'E');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.pop_back();
                characters.pop_back();
                characters.push_back('E');//规约
            }
            else if( a == 'w' ){
                states.push_back('7');
                characters.push_back(a);
                nextw();
            }
            else{
                cout << "Error in state " << state << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }
        else if( state == 'a' ){
            if( a == 'W' || a == 'w' || a == ')' || a == '#'){
                //规约T->TwF
                states.pop_back();
                states.pop_back();
                states.pop_back();
                char tmp = Goto(states.back(),'T');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.pop_back();
                characters.pop_back();
                characters.push_back('T');//规约
            }
            else{   
                cout << "Error in state 10"  << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }
        else if( state == 'b' ){
            if( a == 'W' || a == 'w' || a == ')' || a == '#'){
                //规约F->(E)
                states.pop_back();
                states.pop_back();
                states.pop_back();
                char tmp = Goto(states.back(),'F');
                if( tmp != 'n' )
                    states.push_back(tmp); 
                characters.pop_back();
                characters.pop_back();
                characters.pop_back();
                characters.push_back('F');//规约
            }
            else{
                cout << "Error in state 11"  << endl;
                return false;
                //exit(EXIT_FAILURE);
            }
        }
    }
}


int main(){
    char* fileName = (char*)"LexicalAnalyzerOut.txt";
    nums = readLexicalAnalyzerOut( fileName );
    index = 0;
    bool legal = LR();
    if( legal ){
        cout << "Arithmetic expression is legal." << endl;
    }
    else{
        cout << "Arithmetic expression is not legal" << endl;
    }
    
    //readLexicalAnalyzerOut("LexicalAnalyzerOut.txt");
    getchar();
    return 0;
}