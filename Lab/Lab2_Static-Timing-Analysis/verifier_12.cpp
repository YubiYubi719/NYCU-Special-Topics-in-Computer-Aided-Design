#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

void step1(const string DUT_path, const string GOLDEN_path){
    ifstream DUT(DUT_path);
    string curLine;
    unordered_map<string,int> mp;
    while(getline(DUT,curLine)){
        stringstream ss(curLine);
        string cellName, outputLoad;
        ss >> cellName >> outputLoad;
        mp[cellName] = stoi(outputLoad.substr(2));
    }
}

int main(int argc, char* argv[]){
    // argv[1]: studentID_caseName_load.txt
    // argv[2]: golden_caseName_load.txt

    // argv[3]: studentID_caseName_delay.txt
    // argv[4]: golden_caseName_delay.txt

    // argv[5]: studentID_caseName_path.txt
    // argv[6]: golden_caseName_path.txt

    // argv[7]: studentID_caseName_gate_info.txt
    // argv[8]: golden_caseName_gate_info.txt
    
    // ****************
    // *    Step 1    *
    // ****************


    return 0;
}