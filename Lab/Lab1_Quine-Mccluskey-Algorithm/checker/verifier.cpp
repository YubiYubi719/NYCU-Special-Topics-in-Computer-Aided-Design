/* 
This verifier checks following terms:
1. Prime implicants number
2. Minimum number of implicant to cover whole on-set (minimum covering number)
3. Literal count
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// Golden ans
int golden_implicantNum;
int golden_minimumCoveringNum;
vector<string> golden_mc;
string golden_literal;

// Test ans
int test_implicantNum;
int test_minimumCoveringNum;
vector<string> test_mc;
string test_literal;

int calLiteral(const string &imp){
    int litNum = 0;
    for(const char &c:imp){
        if(c == '\'') continue;
        litNum++;
    }
    return litNum;
}

void readfile(string filename, int &impNum, int &mcNum, vector<string> &mcs, string &literal){
    ifstream input(filename);
    string curStr;
    input >> curStr >> impNum;
    while(input >> curStr){ if(curStr == ".mc") break; }
    input >> mcNum;
    mcs.resize(mcNum);
    for(int i = 0; i < mcNum; i++){
        input >> mcs[i];
    }
    input >> literal;
    input.close();
}

void Check(){
    if(golden_implicantNum != test_implicantNum
    || golden_minimumCoveringNum != test_minimumCoveringNum
    || golden_literal != test_literal){
        cout << "wrong answer.\n";
        return;
    }
    cout << "congratulations, correct answer!" << '\n';
}

int main(int argc, char* argv[]){
    // read golden ans
    readfile(argv[1],golden_implicantNum,golden_minimumCoveringNum,golden_mc,golden_literal);
    // read user's ans
    readfile(argv[2],test_implicantNum,test_minimumCoveringNum,test_mc,test_literal);

    Check();

    return 0;
}