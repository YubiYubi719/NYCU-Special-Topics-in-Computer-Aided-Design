#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
using namespace std;

int main(int argc, char* argv[]){
    ifstream input1(argv[1]);
    string curStr;
    while(input1 >> curStr){ if(curStr == ".mc") break; }
    int num;
    input1 >> num;
    unordered_map<string,int> mp;
    for(int i = 0; i < num; i++){
        input1 >> curStr;
        mp[curStr] = 1;
    }
    string literal_1;
    input1 >> literal_1;
    input1.close();

    ifstream input2(argv[2]);
    while(input2 >> curStr){ if(curStr == ".mc") break; }
    int num_test;
    input2 >> num_test;
    // check minimum covering count
    if(num_test != num){
        cout << "wrong answer." << '\n';
        return 0;
    }

    // check implicants
    vector<string> imps(num_test);
    for(int i = 0; i < num_test; i++){
        input2 >> imps[i];
    }
    for(string imp:imps){
        if(!mp.count(imp)){
            cout << "wrong answer." << '\n';
            return 0;
        }
        else{
            mp[imp]--;
        }
    }
    
    for(pair<string,int> p:mp){
        if(p.second != 0){
            cout << "wrong answer." << '\n';
            return 0;
        }
    }


    string literal_2;
    input2 >> literal_2;
    if(literal_1 != literal_2){
        cout << "wrong answer." << '\n';
        return 0;
    }
    input2.close();
    cout << "congratulations, correct answer!" << '\n';

    return 0;
}