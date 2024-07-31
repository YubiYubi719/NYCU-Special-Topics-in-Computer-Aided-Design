#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]){
    ifstream input(argv[1]); // my output file
    ofstream output("changeType.txt");
    string curStr;
    while(input >> curStr){
        if(curStr == ".mc") break;
    }
    int num;
    input >> num;
    vector<string> MCs(num);
    for(int i = 0; i < num; i++){
        input >> MCs[i];
    }

    for(string MC:MCs){
        output << '(';
        for(int i = MC.length()-1; i >= 0; i--){
            if(MC[i] == '\'') output << '!';
            else output << MC[i];

            if(i != 0 && MC[i] != '\'') output << " && ";
        }
        output << ')';
        if(MC != *(MCs.rbegin())) output << " || ";
    }
    output << '\n';


    return 0;
}