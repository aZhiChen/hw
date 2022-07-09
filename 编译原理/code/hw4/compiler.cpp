#include<iostream>
#include <iomanip>
#include<string.h>
#include<fstream>
using namespace std;

fstream inFile; //需要编译的文件
const char *keyword[] = //定义关键字
{"int","long","short","float","double","char","unsigned",
"signed","const","void","volatile","enum","struct","union",
"if", "else", "goto", "switch", "case", "do", "while",
"for", "continue", "break", "return", "default", 
"typedef", "auto", "register", "extern", "static", "sizeof"};
int keyword_length = sizeof(keyword) / sizeof(char*);
const char *Opera[] = //定义运算符
{ "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", 
"<", ">=", "<=", "&&", "||", "!", "=", "+=", "-=", "*=", 
"/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "&", "|", "^",
"~", "<<", ">>"};
const char *BS[] = {";","{","}","(",")","[","]","\"", "'",",","#","."};  //定义界符
int BS_length = sizeof( BS ) / sizeof(char*);
int operator_length = sizeof(Opera) / sizeof(char*);
char id_table[255][255];//标识符表，存放构成的标识符
int ptr_id = 0; //标识符表的下标
char num_table[255][255];//常数表，存放构成的常熟
int ptr_num = 0; //常数表的下标
char string_table[255][255];
int ptr_string = 0;
char ch;//字符变量，存放最新读入的源程序字符
int ptr_token = 0; //token数组的下标
char token[255] = ""; //存放当前构成单词符号的字符串
bool iserror = false; // 发生错误的标志
bool isdigit(); //判断当前读取的字符是否是数字
bool isletter(); //判断当前读取的字符是否是字母
bool isoperator(); //判断当前字符是否是运算符
bool isbp();//判断当前字符是否是界符
void getbe(); //过滤无用符号
void concatenation(); //将ch中的字符链接到token后作为新的字符串
void error(int num); // 发现错误，显示错误
int reserve(); //判断token是否是keyword，若是则返回它的种别编码，若不是返回-1
int opra(); //判断token是否是操作符，若是，则返回它的种别编码，若不是返回-1
int bp(); //判断token是否是界符，若是，则返回它的种别编码，若不是返回-1 
void buildtable();//建表（标识符表或常数表）
void automata(); //自动机
void LexicalAnalyzer(char*);//整个词法分析器
void Getchar();
int isString = 0;//用于标记token是字符串还是标识符，根据前一个字符是否是'或"决定，若是字符串isString=1,否则为0
char pre; //保存上一个字符
bool useNext = false;
char nextCh;

bool isdigit(){ 
    if( ch >= '0' && ch <= '9' )
        return true;
    else
        return false;
}
 
bool isletter(){
    if( (ch >= 'a' && ch <= 'z') || (ch >='A' && ch <= 'Z') )
        return true;
    else 
        return false;
}

bool isoperator(){
    if( ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
    ch == '=' || ch == '!' || ch == '>' || ch == '<' || ch == '&' || 
    ch == '|' || ch == '^' || ch == '~')
        return true;
    return false;
}

bool isbp(){
    if( ch == ';' || ch == '{' || ch == '}' ||ch == '(' || ch == ')' || ch == '['
    || ch ==']' || ch == '"' || ch == '\'' || ch == ',' || ch == '#'|| ch == '.')
        return true;
    return false;
}

void getbe(){
    while(ch == ' ' || ch == '\t' || ch == '\n')
        Getchar();
}

void error(int num){
    if( num == 0 )
        cout << "The token is too long\n" ;
    if( num == 1 )
        cout << "Illegal character: " << ch << endl;
}

void concatenation(){
    if (ptr_token > 254){
        error(0);
        return;
    }
    token[ptr_token] = ch;
    ptr_token ++;
    token[ptr_token] = '\0';
}

int reserve(){
    int i = 0;
    while( i < keyword_length ){
        if( !strcmp( keyword[i], token) )
            return i + 3; //返回种别编码
        i++;
    }
    return -1; //不是关键字，返回-1
}

int opra(){
    int i = 0;
    while( i < operator_length ){
        if( !strcmp(Opera[i], token) )
            return i + 35;
        i++;
    }
    if( ptr_token <= 4 && ptr_token > 2) // 有可能是两个操作符连在一起，判断不出，将其拆分后重新进行判断。
    {
        if(  pre == '+' || pre == '-' || pre == '*' || pre == '/' || pre == '%' ||
    pre == '=' || pre == '!' || pre == '>' || pre == '<' || pre == '&' || 
    pre == '|' || pre == '^' || pre == '~' ){
            ch = pre; //开始回滚
            pre = token[ptr_token - 2];
            token[ptr_token - 1] = '\0';
            useNext = true; // 回滚，char下次更新时使用next的值，而不直接读取文件
            i = 0;
            while( i < operator_length ){
                if( !strcmp(Opera[i], token) ){
                    return i + 35;
                }
                i++;
            }
        }
    }
    return -1;
}

int bp(){ 
    int i = 0;
    while( i < BS_length ){
        if( !strcmp(BS[i], token) )
            return i + 68;
        i++;
    }
    return -1;
} 

void buildtable(){
    if(isString != 0){
        int k = 0; 
        while( k < ptr_string){
            if( !strcmp( string_table[k], token ) ) {
                break;
            }
            k++;
        }
        if( k == ptr_string ){
            strcpy(string_table[k], token);
            ptr_string ++;
        }
        //isString = 0;
    }
    else{
        char head = token[0]-'0';
        if(!(head >= 0 && head <= 9)){ //token是标识符
            int i = 0;
            while( i < ptr_id ){
                if( !strcmp(id_table[i], token) ){ //两个字符串相等
                    break;
                }
                i++;
            }
            if( i == ptr_id ){
                strcpy(id_table[ptr_id], token);
                ptr_id++; 
            }
        }

        else{ //token是常数
            int j = 0;
            while( j < ptr_num ){
                if( !strcmp(num_table[j], token) ){ //两个字符串相等
                    break;
                }
                j ++;
            }
            if( j == ptr_num ){
                strcpy(num_table[ptr_num], token);
                ptr_num ++;
            }
        }
    }
}

void automata(){
    int state = 0;
    while(state != -1){ //规定-1为退出状态
        if(state == 0){
            if(ch == '"' || ch == '\'' )//不能直接isString == 1就跳到10
                state = 9;
            else if( isString == 1 ){
                state = 10;
            }
            else{
                getbe();
                if(isletter())
                    state = 1;
                else if( isdigit() )
                    state = 2;
                else if( isoperator() )
                    state = 8;
                else if( isbp() )
                    state = 9;
                else{
                    error(1);
                    cout << "191" << endl;
                    iserror = true;
                }
            }
        }
    
        else if( state == 1 ){
            if( isletter() || isdigit() )
                state = 1;
            else   
                state = -1;
        }
        else if( state == 2 ){
            if( isdigit() )
                state = 2;
            else if( ch == '.' )    
                state = 3;
            else if( ch == 'e' )
                state = 5;
            else    
                state = -1;
        }
        else if( state == 3 ){
            if( isdigit() )
                state = 4;
            else{    
                error(1);
                cout << "217" << endl;
                iserror = true;
            }
        }
        else if( state == 4 ){
            if( isdigit() )
                state = 4;
            else if( ch == 'e' )
                state = 5;
            else 
                state = -1;
        }
        else if( state == 5 ){
            if( isdigit() ){
                state = 6;
            }
            else if( ch == '+' || ch == '-' ){
                state = 7;
            }
            else{
                error(1);
                cout << "238" << endl;
                iserror = true;
            }
        }
        else if( state == 6 ){
            if( isdigit() ){
                state = 6;
            }
            else
                state = -1;
        }
        else if( state == 7){
            if( isdigit() )
                state = 6;
            else {
                error(1);
                cout << "254" << endl;
                iserror = true;
            }
        }
        else if( state == 8){
            if( isoperator() )
                state = 8;
            else    
                state = -1;
        }
        else if( state == 9 ){
            state = -1;
        }
        else if( state == 10 ){
            if( ch == '"' || ch == '\'' ){
                state = -1;
            }
        }
        if( iserror ) {
            cout << "error happened in automata" << endl;
            getchar();
            //exit(EXIT_FAILURE);
        }
        if(state != -1){    
            concatenation();
            pre = ch;
            Getchar();
        }
    }
    return;     
}

void Getchar(){
    if (useNext){
        ch = nextCh;
        useNext = false;
    }
    else{
        inFile >> ch;
        nextCh = ch;
    }
}

void LexicalAnalyzer(char* fileName){
    ofstream outFile;
    inFile.open(fileName, ios::in);
    inFile.unsetf(ios_base::skipws);//逐字符读取文件，不跳过空白符
    outFile.open("./LexicalAnalyzerOut.txt",ios::out);
    if( !inFile.is_open() ){
        cout << "Could not find the file!\n";
        getchar();
        //exit(EXIT_FAILURE);
    }
    Getchar();
    outFile.setf(ios::left);
    outFile << setfill(' ')<<setw(66) << "Characters";
    outFile  << "Code" << endl;
    while(!inFile.eof()){
        ptr_token = 0;
        automata();
        int flag = -1;
        if( !( pre == '"' || pre == '\'' ) && isString == 1 ){ //token是字符串
            flag = 2;
            outFile.setf(ios::left);
            outFile << setfill(' ')<<setw(75) << token;
            outFile  << flag << endl;
            //isString = 0;
        }
        else{
            flag = reserve();
            if( flag != -1 ){ //token为关键字
                outFile.setf(ios::left);
                outFile << setfill(' ')<<setw(75) << token ;
                outFile  << flag << endl;
            }
            else{
                flag = opra(); 
                if( flag != -1 ){ // token 为操作符
                    outFile.setf(ios::left);
                    outFile << setfill(' ')<<setw(75) << token;
                    outFile  << flag << endl;
                }
                else{
                    flag = bp();
                    if( flag != -1 ){ //token是界符
                        outFile.setf(ios::left);
                        outFile << setfill(' ')<<setw(75) << token;
                        outFile  << flag << endl;
                        buildtable();
                        if( pre == '"' || pre == '\'' )
                            isString = isString==1 ? 0 : 1;

                    }   
                    else{ //token是标识符或常数
                        if( token[0] >= '0' && token[0] <= '9' ) {//token是常数
                            flag = 0;
                            outFile.setf(ios::left);
                            outFile << setfill(' ')<<setw(75) << token;
                            outFile  << flag << endl;
                            buildtable();
                        }
                        else{//token是标识符
                            flag = 1;
                            outFile.setf(ios::left);
                            outFile << setfill(' ')<<setw(75) << token;
                            outFile  << flag << endl;
                            buildtable();
                        }
                    }
                }
            }
        }
    }
    outFile.close();
    inFile.close();
}

int main(){
    char* fileName = "test.txt";
    LexicalAnalyzer(fileName);
    cout << "Finished\n";
    getchar();
    return 0;
}
