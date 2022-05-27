#include<iostream>
#include<iomanip>
//#include<string.h>
#include<fstream>
#include<vector>
#include<sstream>
using namespace std;

fstream inFile; //需要读取的源文件
char ch; //当前读取到的字符

bool isW0(char a){
    if( a == '+' || a == '-' )
        return true;
    return false;
}

bool isW0(int num){
    if( num == 35 || num == 36 )
        return true;
    return false;
}

bool isW1(char a){
    if( a == '*' || a == '/' )
        return true;
    return false;
}

bool isW1( int num ){
    if( num == 37 || num == 38 )
        return true;
    return false;
}

bool isI(char a){
    if( (a <= '9' && a >='0') || (a >= 'a' && a <= 'z') || (a >='A' && a <= 'Z') )
        return true;
    return false;
}

bool isI(int num){
    return num == 0;
}

void Getchar(){
    if( !inFile.eof() ){
        inFile >> ch;
        cout << "Get char: " << ch << endl;
    }
}

void Getnum(int& num, vector<int>& nums, int& index){
    if( index < nums.size() ){
        num = nums[index++];
    }
}

void Getcons( double& constant, vector<double>& cons, int& index ){
    if( index < cons.size() ){
        constant = cons[index++];
    }
}

struct mydata{
    vector<int> nums;
    vector<double> cons;
};
//1. Read message from LexicalAnalyzerOut.txt
//2. Produce key message for Parsing
struct mydata readLexicalAnalyzerOut( char* fileName ){
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
    struct mydata data;
    string line;
    getline(infile,line);
    //vector<int> nums;
    string tmp1, tmp2;  
    while( getline(infile, line) ){
        //getline(infile, line);
        //line = infile.getline();
        stringstream input(line);
        input >> tmp1;
        input >> tmp2;
        int i = stoi(tmp2);
        if( i == 0 )
            data.cons.push_back(stod(tmp1));
        data.nums.push_back(i); 
    }
    //for(int i = 0; i < nums.size(); i++){
     //   cout << nums[i] << endl;
    //}
    infile.close();
    outfile.close();
    return data;
}

bool LL1(char* fileName){
    vector<string> stack; 
    vector<double> SEM;
    //e = E1, t = T1, W = w0, w = w1
    //vector<int> nums = readLexicalAnalyzerOut(fileName);
    struct mydata data = readLexicalAnalyzerOut(fileName);
    //inFile.open(fileName, ios::in);
    //if( !inFile.is_open() ){
    //    cout << "Could not find the file!\n" << endl;
    //    getchar();
    //    exit(EXIT_FAILURE);
    //}
    //Getchar();
    for(int i = 0; i < data.nums.size(); i++){
        cout <<data.nums[i] << " ";
    }
    cout << endl;
    int num;
    double constant;
    int index = 0;
    int cons_index = 0;
    Getnum(num, data.nums, index);
    stack.push_back("#");
    stack.push_back("E");
    cout << "begin" << endl;
    while( stack.size() > 0 ){
        string s = stack.back();
        cout << stack.size() << endl;
        cout << "SYN: ";
        for( int i = 0; i < stack.size(); i++ ){
            cout  << stack[i] << " ";
        }
        cout << endl;
        cout << "SEM: ";
        for( int i = 0; i < SEM.size(); i++ ){
            cout << SEM[i] << " ";
        }
        cout << endl;
        cout << "Input: ";
        for( int i = index; i < data.nums.size(); i++){
            cout << data.nums[i] << " ";
        }
        cout << endl;
        getchar();
        if( s == "E" ){
            cout << "num is " << num << endl;
            if( num == 0 || num == 71 ){
            //if( isI(ch) || ch == '(' ){
                stack.pop_back();
                stack.push_back("e");
                stack.push_back("T");
            }
            else{
                cout << "Error in E" << endl;
                getchar();
                exit(EXIT_FAILURE);
            } 
        }
        else if( s == "e" ){
            if( isW0(num) ){
            //if( isW0(ch) ){
                stack.pop_back();
                stack.push_back("e");
                if(num == 35)
                    stack.push_back("GEQ(+)");
                else
                    stack.push_back("GEQ(-)");
                stack.push_back("T");
                stack.push_back("W");
            }
            else if( num == 68 || num == 72 ){
                stack.pop_back();
            }
            else{
                cout << "Error in E1" << endl;
                getchar();
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "T"){ 
            //cout << "ch is " << ch << endl;
            if( isI(num) || num == 71 ){
            //if( isI(ch) || ch == '(' ){
                stack.pop_back();
                stack.push_back("t");
                stack.push_back("F");
            }
            else{
                cout << "Error in T" << endl;
                getchar();
                exit(EXIT_FAILURE);
            }   
        }
        else if( s == "t" ){
            if( isW0(num) || num == 72 || num == 68 ){
            //if( isW0(ch) || ch == ')' || ch == ';' ){
                stack.pop_back();
            }
            else if( isW1(num) ){
            //else if( isW1(ch) ){
                stack.pop_back();
                stack.push_back("t");
                if( num == 37 )
                    stack.push_back("GEQ(*)");
                else    stack.push_back("GEQ(/)");
                stack.push_back("F");
                stack.push_back("w");
            }
            else{
                cout << "Error in t" << endl;
                getchar();
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "F" ){
            if( num == 71 ){
            //if( ch == '(' ){
                stack.pop_back();
                stack.push_back(")");
                stack.push_back("E");
                stack.push_back("(");
            }
            else if( isI(num) ){
            //else if( isI(ch) ){
                stack.pop_back();
                //read next word
                //Getchar();
                Getcons(constant, data.cons, cons_index);
                SEM.push_back(constant);
                cout << "PUSH(" << constant << ")" << endl;
                Getnum(num,data.nums, index);
                //inFile >> ch;
            }
            else{
                cout << "Error in F" << endl;
                getchar();
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "#" ){
            if( num == 68 )
            //if( ch == ';' )
                return true;
            else{
                cout << "Error in #" << endl;
                getchar();
                return false;
            }
        }
        else if( ( s == "(" && num == 71) || ( s == ")" && num == 72) || ( s == "W" && isW0(num)) || ( s == "w" && isW1(num)) ){
        //else if(s == ch || ( s == 'W' && isW0(ch)) || ( s == 'w' && isW1(ch) ) ){
            stack.pop_back();
            //Getchar();
            Getnum(num, data.nums, index);
        }
        else if( s == "GEQ(+)" ){
            stack.pop_back();
            double a, b;
            double c;
            if( SEM.size() >= 2 ){
                a = SEM.back();
                SEM.pop_back();
                b = SEM.back();
                SEM.pop_back();
                c = a + b;
                SEM.push_back(c);
                cout << endl;
                cout << "Produce: (+ " << b << ", " << a << ", c)" << endl;
                cout << "c = " << c << endl;
                cout << endl;
            }
            else{
                cout << "Error in GEQ(+)" << endl;
                cout << "SEM size is " << SEM.size() << endl; 
                return false;
            }
        }
        else if( s == "GEQ(-)" ){
            stack.pop_back();
            double a, b;
            double c;
            if( SEM.size() >= 2 ){
                a = SEM.back();
                SEM.pop_back();
                b = SEM.back();
                SEM.pop_back();
                c = b - a;
                SEM.push_back(c);
                cout << endl;
                cout << "Produce: (- " << b << ", " << a << ", c)" << endl;
                cout << "c = " << c << endl;
                cout << endl;
            }
            else{
                cout << "Error in GEQ(-)" << endl;
                cout << "SEM size is " << SEM.size() << endl; 
                return false;
            }
        }
        else if( s == "GEQ(*)" ){
            stack.pop_back();
            double a, b;
            double c;
            if( SEM.size() >= 2 ){
                a = SEM.back();
                SEM.pop_back();
                b = SEM.back();
                SEM.pop_back();
                c = a * b;
                SEM.push_back(c);
                cout << endl;
                cout << "Produce: (* " << b << ", " << a << ", c)" << endl;
                cout << "c = " << c << endl;
                cout << endl;
            }
            else{
                cout << "Error in GEQ(*)" << endl;
                cout << "SEM size is " << SEM.size() << endl; 
                return false;
            }
        }
        else if( s == "GEQ(/)" ){
            stack.pop_back();
            double a, b;
            double c;
            if( SEM.size() >= 2 ){
                a = SEM.back();
                SEM.pop_back();
                b = SEM.back();
                SEM.pop_back();
                c = b / a;
                SEM.push_back(c);
                cout << endl;
                cout << "Produce: (/ " << b << ", " << a << ", c)" << endl;
                cout << "c = " << c << endl;
                cout << endl;
            }
            else{
                cout << "Error in GEQ(/)" << endl;
                cout << "SEM size is " << SEM.size() << endl; 
                return false;
            }
        }
        else{
            cout << "Error in " << s << endl;
            getchar();
            return false;
        }
    }
}


int main(){
    //char* fileName = "LL(1)test.txt";
    char* fileName = (char*)"LexicalAnalyzerOut.txt";
    bool legal = LL1(fileName);
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
