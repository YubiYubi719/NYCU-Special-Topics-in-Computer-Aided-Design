#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]){
    ifstream input(argv[1]);
    ofstream output(argv[2]);
    string curStr;
    while(input >> curStr){ if(curStr == ".mc") break; }
    output << curStr << " ";
    int num;
    input >> num;
    output << num << '\n';
    vector<string> imps;
    for(int i = 0; i < num; i++){
        string imp;
        input >> imp;
        int cnt = 0;
        for(char c:imp){
            if(c == '-'){
                cnt++;
                continue;
            }
            if(c == '1') output << (char)('A'+cnt);
            else output << (char)('A'+cnt) << '\'';
            ++cnt;
        }
        output << '\n';
    }
    input >> curStr;
    output << curStr;
    input.close();
    output.close();
}