#include<iostream>
#include<iomanip>
//#include<string.h>
#include<fstream>
#include<vector>
#include<sstream>
#include<string>
using namespace std;

fstream inFile; //需要读取的源文件
char ch; //当前读取到的字符

struct mydata{
    vector<int> nums;//编码
    vector<int> idx; //如果是常数或字符串或变量，则idx为其在对应表的下标
};

struct ID{
    vector<string> name; //变量名
    vector<int> val;//变量值
};

struct mydata data;
int ID_LEN = 0;//data的下标
int CONS_LEN = 0;
int STR_LEN = 0;
int LEN; // data的长度
int DataIdx = 0;
vector<int> Cons;//常数表
vector<string> Str;//字符串表
struct ID Id;//变量表，只记录变量名字
vector<string> stack; 
vector<string> SEM;
vector<string> QUA;
vector<int> ADDR;//地址栈
vector<int> block;//记录每个基本块的起始位置，用于目标代码生成 
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
        if( i == 0 ){
            Cons.push_back(stod(tmp1));
            data.idx.push_back(CONS_LEN);
            CONS_LEN ++;
        }
        else if( i == 1 ){
            Id.name.push_back(tmp1);
            Id.val.push_back(0);
            data.idx.push_back(ID_LEN);
            ID_LEN++;
        }
        else if( i == 2 ){
            Str.push_back(tmp1);
            data.idx.push_back(STR_LEN);
            STR_LEN ++;
        }
        else{
            data.idx.push_back(-1);
        }
        data.nums.push_back(i); 
    }
    //for(int i = 0; i < nums.size(); i++){
     //   cout << nums[i] << endl;
    //}
    infile.close();
    outfile.close();
    return data;
}

void Getnum(int& num, vector<int>& nums, int& index){
    if( index < nums.size() ){
        num = nums[index++];
    }
}

//判断字符串name是否是变量，若是返回变量在变量表的下标，若不是返回-1
int  isID(string name){
    for( int i = 0; i < Id.name.size(); i++ ){
        if( name == Id.name[i] )
            return i;
    }
    return -1;
}




bool LL1(char* fileName){
    int addr = 0;//记录四元式的地址
    int tnum = 0; //记录临时变量的总数
    data = readLexicalAnalyzerOut(fileName);
    LEN = data.nums.size();
    int num;
    Getnum(num, data.nums, DataIdx);
    //stack.push_back("#");
    stack.push_back("S");
    while( stack.size() > 0){
        string s = stack.back();
        /*
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
        cout << "Input: ";
        for( int i = DataIdx; i < data.nums.size(); i++){
            cout << data.nums[i] << " ";
        }
        cout << endl;
        int gg;
        //cin >> gg;
        */
        if( s == "S" ){
            //int
            if( num == 3 ){
                stack.pop_back();
                stack.push_back("GEQ(me)");
                stack.push_back("}");
                stack.push_back(";");
                stack.push_back("0");
                stack.push_back("return");
                stack.push_back("A");
                stack.push_back("{");
                stack.push_back("GEQ(main)");
                stack.push_back(")");
                stack.push_back("(");
                stack.push_back("main");
                stack.push_back("int");
            }
            else{
                cout << "Error in S" << endl;
                getchar();
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "int"){
            if( num == 3){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in int" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "main" ){
            if( num == 1){
                int table_idx = data.idx[DataIdx-1];
                if(Id.name[table_idx] != "main"){
                    cout << "Temp Id is" << Id.name[table_idx] << endl;
                    cout << "Error in main" << endl;
                    exit(EXIT_FAILURE);
                }
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in main" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "(" ){
            if( num == 71 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in (" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == ")" ){
            if( num == 72 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in (" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "{" ){
            if( num == 69 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in {" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "}" ){
            if( num == 70 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in }" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "return" ){
            if( num == 27 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in return" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "0" ){
            if( num == 0 ){
                int table_idx = data.idx[DataIdx-1];
                if(Cons[table_idx] != 0){
                    cout << "Error in 0" << endl;
                    exit(EXIT_FAILURE);
                }
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in 0" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == ";" ){
            if( num == 68 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in ;" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "char"){
            if( num == 8 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in char" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "bool"){
            if( num == 80 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in bool" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "if" ){
            if( num == 17 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in if" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "else" ){
            if( num == 18 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in else" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "while" ){
            if( num == 23){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in while" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "for" ){
            if( num == 24){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in for" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "+" ){
            if( num == 35 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in +" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "-" ){
            if( num == 36 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in -" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "&&" ){
            if( num == 48 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in &&" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "||" ){
            if( num == 49 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in &&" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "*" ){
            if( num == 37 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in *" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "/" ){
            if( num == 38 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in /" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "!" ){
            if( num == 50 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in !" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "id" ){
            if( num == 1 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in id" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "=" ){
            if( num == 51 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in id" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "num" ){
            if( num == 0 ){
                stack.pop_back();
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in id" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "A" ){
            if( num == 3 || num == 8 || num == 80 || num == 1 || num == 17 || num == 23 || num == 24 ){
                stack.pop_back();
                stack.push_back("A");
                stack.push_back("C");
            }
            else if( num == 27 || num == 70){
                stack.pop_back();
            }
            else{
                cout << "num is " << num << endl;
                cout << "Error in A" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "C" ){
            if( num == 3 || num == 8 || num == 80 ){
                stack.pop_back();
                stack.push_back("X");
            }
            else if( num == 17 || num == 23 || num == 24 ){
                stack.pop_back();
                stack.push_back("B");
            }
            else if( num == 1 ){
                stack.pop_back();
                stack.push_back("R");
            }
            else{
                cout << "Error in C" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "X" ){
            if( num == 3 || num == 8 || num == 80 ){
                stack.pop_back();
                stack.push_back(";");
                stack.push_back("Z");
                stack.push_back("Y");
            }
            else{
                cout << "Error in X" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "Y" ){
            if( num == 3 ){
                stack.pop_back();
                stack.push_back("int");
            }
            else if( num == 8 ){
                stack.pop_back();
                stack.push_back("char");
            }
            else if( num == 80 ){
                stack.pop_back();
                stack.push_back("bool");
            }
            else{
                cout << "Error in Y" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "Z" ){
            if( num == 1 ){
                stack.pop_back();
                stack.push_back("Z'");
                stack.push_back("U");
            }
            else{
                cout << "Error in Z" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "Z'" ){
            if( num == 77 ){
                stack.pop_back();
                stack.push_back("Z");
                stack.push_back(",");
            }
            else if( num == 68 || num == 24 || num == 23 || num == 70 || num == 17 || num == 1 || num == 3 || num == 8 || num == 80){
                stack.pop_back();
            }
            else{
                cout << "Error in Z'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "U" ){
            if( num == 1 ){
                stack.pop_back();
                stack.push_back("U'");
                //stack.push_back("PUSH(id)");
                int idxindex = data.idx[DataIdx-1];
                string s = Id.name[idxindex];
                SEM.push_back(s);
                stack.push_back("id");
            }
            else{
                cout << "Error in U" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "U'" ){
            if( num == 77 || num == 68 ){
                stack.pop_back();
            }
            else if( num == 51 ){
                stack.pop_back();
                stack.push_back("GEQ(=)");
                stack.push_back("L");
                stack.push_back("=");
            }
            else{
                cout << "Error in U'" << endl;
                exit(EXIT_FAILURE);
            }
        } 
        else if( s == "R" ){
            if( num == 1 ){
                stack.pop_back();
                stack.push_back(";");
                stack.push_back("GEQ(=)");
                stack.push_back("L");
                stack.push_back("=");
                //stack.push_back("PUSH(id)");
                int idxindex = data.idx[DataIdx-1];
                string s = Id.name[idxindex];
                SEM.push_back(s);
                stack.push_back("id");
            }
            else{
                cout << "Error in R" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "L" ){
            if( num == 0 || num == 1 || num == 71 ){
                stack.pop_back();
                stack.push_back("L'");
                stack.push_back("T");
            }
            else{
                cout << "num is " << num << endl;
                cout << "Error in L" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "L'" ){
            if( num == 35 ){
                stack.pop_back();
                stack.push_back("L'");
                stack.push_back("GEQ(+)");
                stack.push_back("T");
                stack.push_back("+");
            }
            else if( num == 36 ){
                stack.pop_back();
                stack.push_back("L'");
                stack.push_back("GEQ(-)");
                stack.push_back("T");
                stack.push_back("-");
            }
            else if( num == 3 || num == 8 || num == 80 || num == 72 || num == 1 || num == 77 || num == 17 || num == 70 || num == 23 || num == 24 || num == 68 ){
                stack.pop_back();
            }
            else{
                cout << "num is " << num << endl;
                cout << "Error in L'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "T" ){
            if( num == 71 || num == 1 || num == 0 ){
                stack.pop_back();
                stack.push_back("T'");
                stack.push_back("F");
            }
            else{
                cout << "Error in T" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "T'" ){
            if( num == 37 ){
                stack.pop_back();
                stack.push_back("T'");
                stack.push_back("GEQ(*)");
                stack.push_back("F");
                stack.push_back("*");
            }
            else if( num == 38 ){
                stack.pop_back();
                stack.push_back("T'");
                stack.push_back("GEQ(/)");
                stack.push_back("F");
                stack.push_back("/");
            }
            else if( num == 35 || num == 36 || num == 3 || num == 8 || num == 80 || num == 72 || num == 1 || num == 77 || num == 17 || num == 70 || num == 23 || num == 24 || num == 68 ){
                stack.pop_back();
            }
            else {
                cout << "Error in T'" << endl;
                exit(EXIT_FAILURE);
            }

        }
        else if( s == "F" ){
            if( num == 71 ){
                stack.pop_back();
                stack.push_back(")");
                stack.push_back("L");
                stack.push_back("(");
            }
            else if( num == 1 ){
                stack.pop_back();
                //stack.push_back("PUSH(id)");
                int idxindex = data.idx[DataIdx-1];
                string s = Id.name[idxindex];
                //cout << "s:" << s << endl;
                SEM.push_back(s);
                stack.push_back("id");

            }
            else if( num == 0 ){
                stack.pop_back();
                //stack.push_back("PUSH(num)");
                int idxindex = data.idx[DataIdx-1];
                //cout << "idxindex is " << idxindex << endl; 
                int n = Cons[idxindex];
                //cout << "n is " << n << endl;
                string s = to_string(n);
                //cout << "s:" << s << endl;
                SEM.push_back(s);
                stack.push_back("num");
            }
            else {
                cout << "Error in F" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "O" ){
            if( num == 40 ){
                stack.pop_back();
                //stack.push_back("++");
                stack.push_back("GEQ(++)");
                Getnum(num, data.nums, DataIdx);
            }
            else if( num == 41 ){
                stack.pop_back();
                stack.push_back("GEQ(--)");
                //stack.push_back("--");
                Getnum(num, data.nums, DataIdx);
            }
            else if( num == 72 ){
                stack.pop_back();
            }
            else{
                cout << "Error in O'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "Q" ){
            if( num == 72 ){
                stack.pop_back();
            }
            else if( num == 1 ){
                stack.pop_back();
                stack.push_back("O");
                //stack.push_back("PUSH(id)");
                int idxindex = data.idx[DataIdx-1];
                string s = Id.name[idxindex];
                SEM.push_back(s);
                stack.push_back("id");
            }
            else{
                cout << "Error in Q'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "E" ){
            if( num == 72 || num == 1 || num == 0 ){
                stack.pop_back();
                stack.push_back("E'");
                stack.push_back("H");
            }
            else{
                cout << "Error in E" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "E'" ){
            if( num == 48 ){
                stack.pop_back();
                stack.push_back("GEQ(&&)");
                stack.push_back("E");
                stack.push_back("&&");
            }
            else if( num == 72 || num == 68 ){
                stack.pop_back();
            }
            else{
                cout << "Error in E" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "H" ){
            if( num == 71 || num == 1 || num == 0 || num == 50 ){
                stack.pop_back();
                stack.push_back("H'");
                stack.push_back("G");
            }
            else{
                cout << "Error in H" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "H'" ){
            if( num == 49 ){
                stack.pop_back();
                stack.push_back("GEQ(||)");
                stack.push_back("H");
                stack.push_back("||");
            }
            else if( num == 48 || num == 72 || num == 69 ){
                stack.pop_back();
            }
            else{
                cout << "Error in H'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "G" ){
            if( num == 71 || num == 1 || num == 0 ){
                stack.pop_back();
                stack.push_back("GEQ(D)");
                stack.push_back("F");
                stack.push_back("D");
                stack.push_back("F");
            }
            else if( num == 50 ){
                stack.pop_back();
                stack.push_back("E");
                stack.push_back("!");
            }
            else{
                cout << "Error in G'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "D" ){
            if( num == 45 ){ //<
                stack.pop_back();
                //stack.push_back("PUSH(<)");
                SEM.push_back("<");
                Getnum(num, data.nums, DataIdx);
            }
            else if( num == 44 ){ //>
                stack.pop_back();
                //stack.push_back("PUSH(>)");
                SEM.push_back(">");
                Getnum(num, data.nums, DataIdx);
            }
            else if( num == 42 ){ // ==
                stack.pop_back();
                //stack.push_back("{PUSH(==)");
                SEM.push_back("==");
                Getnum(num, data.nums, DataIdx);
            }
            else if( num == 43 ){ // !=
                stack.pop_back();
                //stack.push_back("PUSH(!=)");
                SEM.push_back("!=");
                Getnum(num, data.nums, DataIdx);
            }
            else{
                cout << "Error in D'" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "B" ){
            if( num == 17 ){
                stack.pop_back();
                stack.push_back("IE(ie)");
                stack.push_back("}");
                stack.push_back("A");
                stack.push_back("{");
                stack.push_back("EL(el)");
                stack.push_back("else");
                stack.push_back("}");
                stack.push_back("A");
                stack.push_back("{");
                stack.push_back("IF(if)");
                stack.push_back(")");
                stack.push_back("E");
                stack.push_back("(");
                stack.push_back("if");
            }
            else if( num == 23 ){
                stack.pop_back();
                stack.push_back("WE(we)");
                stack.push_back("}");
                stack.push_back("A");
                stack.push_back("{");
                stack.push_back("DO(do)");
                stack.push_back(")");
                stack.push_back("E");
                stack.push_back("(");
                stack.push_back("WH(wh)");
                stack.push_back("while");
            }
            else if( num == 24 ){
                stack.pop_back();
                stack.push_back("FE(fe)");
                stack.push_back("}");
                stack.push_back("A");
                stack.push_back("{");
                stack.push_back(")");
                stack.push_back("Q");
                stack.push_back("DO(do)");
                stack.push_back(";");
                stack.push_back("G");
                stack.push_back("FOR(for)");
                stack.push_back(";");
                stack.push_back("Z");
                stack.push_back("Y");
                stack.push_back("(");
                stack.push_back("for");
            }
            else {
                cout << "Error in B" << endl;
                exit(EXIT_FAILURE);
            }
        }
        else if( s == "GEQ(main)" ){
            stack.pop_back();
            //ADDR.push_back(int(QUA.size()));
            block.push_back(int(QUA.size()));
            QUA.push_back("main,0,-,-");
            
        }
        else if( s == "GEQ(me)" ){
            stack.pop_back();
            QUA.push_back("end,0,-,-");
            //ADDR.pop_back();
        }
        else if( s == "GEQ(+)" ){
            stack.pop_back();
            string a, b;
            int aidx, bidx;
            int aval, bval;
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            //a不是变量，是常量
            if( aidx == -1 ){
                aval = stoi(a);
            }
            else{
                aval = Id.val[aidx];
            }
            b = SEM.back();
            SEM.pop_back();
            bidx = isID(b);
            if( bidx == -1 ){
                bval = stoi(b);
            }
            else{
                bval = Id.val[bidx];
            }
            string t = "t";
            t = t + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            Id.val.push_back(aval+bval);
            string qua = "+," + b + "," + a + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(-)" ){
            stack.pop_back();
            string a, b;
            int aidx, bidx;
            int aval, bval;
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            //a不是变量，是常量
            if( aidx == -1 ){
                aval = stoi(a);
            }
            else{
                aval = Id.val[aidx];
            }
            b = SEM.back();
            SEM.pop_back();
            bidx = isID(b);
            if( bidx == -1 ){
                bval = stoi(b);
            }
            else{
                bval = Id.val[bidx];
            }
            string t = "t";
            t = t + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            Id.val.push_back(aval-bval);
            string qua = "-," + b + "," + a + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(*)" ){
            stack.pop_back();
            string a, b;
            int aidx, bidx;
            int aval, bval, resval;
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            //a不是变量，是常量
            if( aidx == -1 ){
                aval = stoi(a);
            }
            else{
                aval = Id.val[aidx];
            }
            b = SEM.back();
            SEM.pop_back();
            bidx = isID(b);
            if( bidx == -1 ){
                bval = stoi(b);
            }
            else{
                bval = Id.val[bidx];
            }
            string t = "t";
            t = t + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            Id.val.push_back(aval*bval);
            string qua = "*," + b + "," + a + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(/)" ){
            stack.pop_back();
            string a, b;
            int aidx, bidx;
            int aval, bval, resval;
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            //a不是变量，是常量
            if( aidx == -1 ){
                aval = stoi(a);
            }
            else{
                aval = Id.val[aidx];
            }
            if(aval == 0){
                cout << "Error in GEQ(/)" << endl;
                exit(EXIT_FAILURE);
            }
            b = SEM.back();
            SEM.pop_back();
            bidx = isID(b);
            if( bidx == -1 ){
                bval = stoi(b);
            }
            else{
                bval = Id.val[bidx];
            }
            string t = "t";
            t = t + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            Id.val.push_back(bval/aval);
            string qua = "/," + b + "," + a + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(=)" ){
            stack.pop_back();
            string a, res;
            a = SEM.back();
            SEM.pop_back();
            res = SEM.back();
            SEM.pop_back();
            int residx = isID(res);
            int aidx = isID(a);
            if( residx == -1 ){
                cout << residx << " " << aidx << endl;
                cout << "Error in GEQ(=)" << endl;
                exit(EXIT_FAILURE);
            }
            if( aidx != -1 )
                Id.val[residx] = Id.val[aidx];
            else Id.val[residx] = stoi(a);
            string qua = "=,"+ a + ",-," + res;
            QUA.push_back(qua);  
        }
        else if( s == "GEQ(++)" ){
            stack.pop_back();
            string a;
            a = SEM.back();
            int aidx = isID(a);
            if(aidx == -1){
                cout << "Error in GEQ(++)" << endl;
                exit(EXIT_FAILURE);
            }
            Id.val[aidx] ++;
            string qua = "+,"+ a + ",1," + a;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(--)" ){
            stack.pop_back();
            string a;
            a = SEM.back();
            int aidx = isID(a);
            if(aidx == -1){
                cout << "Error in GEQ(--)" << endl;
                exit(EXIT_FAILURE);
            }
            Id.val[aidx] ++;
            string qua = "-,"+ a + ",1," + a;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(D)" ){
            stack.pop_back();
            string a, b, symbol, t;
            b = SEM.back();
            SEM.pop_back();
            symbol = SEM.back();
            SEM.pop_back();
            a = SEM.back();
            SEM.pop_back();
            int bidx, aidx;
            bidx = isID(b);
            aidx = isID(a);
            int aval, bval;
            if( aidx == -1 ){
                aval = stoi(a);
            }
            else{
                aval = Id.val[aidx];
            }
            if( bidx == -1 ){
                bval = stoi(b);
            }
            else{
                bval = Id.val[bidx];
            }
            t = "t" + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            //Id.val.push_back(bval/aval);
            if( symbol == "!=" ){
                if( aval != bval )
                    Id.val.push_back(1);
                else
                    Id.val.push_back(0);
            }
            else if( symbol == "==" ){
                if( aval == bval )
                    Id.val.push_back(1);
                else
                    Id.val.push_back(0);
            }
            else if( symbol == ">" ){
                if( aval > bval )
                    Id.val.push_back(1);
                else
                    Id.val.push_back(0);
            }
            else if( symbol == "<" ){
                if( aval < bval )
                    Id.val.push_back(1);
                else
                    Id.val.push_back(0);
            }
            else{
                cout << "Error in GEQ(D)" << endl;
                cout << "The symbol is " << symbol << endl; 
                exit(EXIT_FAILURE);
            }
            string qua  = symbol + "," + a + "," + b + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(&&)" ){
            stack.pop_back();
            string a, b, t, qua;
            int aidx, bidx;
            int aval, bval;
            b = SEM.back();
            SEM.pop_back();
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            bidx = isID(b);
            if( aidx == -1 || bidx == -1 ){
                cout << "Error in GEQ(&&)" << endl;
                exit(EXIT_FAILURE);
            }
            aval = Id.val[aidx];
            bval = Id.val[bidx];
            t = "t" + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            if( aval == 1 && bval == 1 )
                Id.val.push_back(1);
            else Id.val.push_back(0);
            qua = "&&," + a + "," + b + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "GEQ(||)" ){
            stack.pop_back();
            string a, b, t, qua;
            int aidx, bidx;
            int aval, bval;
            b = SEM.back();
            SEM.pop_back();
            a = SEM.back();
            SEM.pop_back();
            aidx = isID(a);
            bidx = isID(b);
            if( aidx == -1 || bidx == -1 ){
                cout << "Error in GEQ(||)" << endl;
                exit(EXIT_FAILURE);
            }
            aval = Id.val[aidx];
            bval = Id.val[bidx];
            t = "t" + to_string(tnum++);
            SEM.push_back(t);
            Id.name.push_back(t);
            if( aval == 1 || bval == 1 )
                Id.val.push_back(1);
            else Id.val.push_back(0);
            qua = "||" + a + "," + b + "," + t;
            QUA.push_back(qua);
        }
        else if( s == "IF(if)" ){
            stack.pop_back();
            string t = SEM.back();
            SEM.pop_back();
            ADDR.push_back(int(QUA.size()));//还要回填地址，所以将该地址压栈
            string qua = "if," + t + ",-," ;
            QUA.push_back(qua);
            block.push_back(int(QUA.size()));
        }
        else if( s == "EL(el)" ){
            stack.pop_back();
            string qua = "el,-,-,";
            int addr1 = ADDR.back();
            ADDR.pop_back();
            QUA[addr1] += to_string(int(QUA.size()));
            ADDR.push_back(int(QUA.size()));//还要回填地址，所以将该地址压栈
            QUA.push_back(qua);
            block.push_back(int(QUA.size()));
        }
        else if( s == "IE(ie)" ){
            stack.pop_back();
            string qua = "ie,-,-,-";
            int addr1; //回填两个四元式
            addr1 = ADDR.back();
            ADDR.pop_back();
            QUA[addr1] += to_string(int(QUA.size()));
            QUA.push_back(qua);
        }
        else if( s == "WH(wh)" ){
            stack.pop_back();
            string qua = "wh,-,-,-";
            QUA.push_back(qua);
            ADDR.push_back(int(QUA.size()));//用于we的地址回填
        }
        else if( s == "DO(do)" ){
            stack.pop_back();
            string a = SEM.back();
            SEM.pop_back();
            int aidx = isID(a);
            if(aidx == -1){
                cout << "Error in WH(wh)" << endl;
                exit(EXIT_FAILURE);
            }
            //int aval = Id.val[aidx];
            string qua = "do," + a + ",-,";
            ADDR.push_back(int(QUA.size()));
            QUA.push_back(qua);
            block.push_back(int(QUA.size()));
        }
        else if( s == "WE(we)" ){
            stack.pop_back();
            string qua = "we,-,-,";
            //回填DO的地址
            int addr1 = ADDR.back();
            ADDR.pop_back();
            //得到WH的下一条地址，回填WE
            int addr2 = ADDR.back();
            ADDR.pop_back();
            qua = qua + to_string(addr2);
            QUA.push_back(qua);
            QUA[addr1] = QUA[addr1] + to_string(int(QUA.size()));
            block.push_back(int(QUA.size()));
        }
        else if( s == "FOR(for)" ){
            stack.pop_back();
            string qua = "for,-,-,-";
            QUA.push_back(qua);
            //cout << "1222: " << sizeof(QUA) << endl; 
            ADDR.push_back(int(QUA.size()));
        }
        else if( s == "FE(fe)" ){
            cout << "ADDR is ";
            for(int i = 0; i < ADDR.size(); i++)
                cout << ADDR[i] << endl;
            stack.pop_back();
            int addr1 = ADDR.back();
            ADDR.pop_back();
            int addr2 = ADDR.back();
            ADDR.pop_back();
            string qua = "fe,-,-," + to_string(addr2);
            QUA.push_back(qua);
            QUA[addr1] = QUA[addr1] + to_string(int(QUA.size()));
            block.push_back(int(QUA.size()));
        }



    }
    return true;

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
    for(int i = 0; i < QUA.size(); i++){
        cout << i <<": (" << QUA[i] << ")" << endl;
    }
    //for(int i = 0; i < block.size(); i++){
    //    cout << block[i] << endl;
   // }
    //readLexicalAnalyzerOut("LexicalAnalyzerOut.txt");
    getchar();
    int i;
    cin >> i;
    return 0;
}