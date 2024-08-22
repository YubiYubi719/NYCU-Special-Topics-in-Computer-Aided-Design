#include <bits/stdc++.h>
#include <regex>
using namespace std;
#define EPSILON 1

bool step3(const string DUT_path, const string GOLDEN_path){
    smatch match;
    string curLine, golden_paths;
    ifstream golden(GOLDEN_path);
    string golden_longestDelay, golden_shortestDelay;
    while(getline(golden,curLine)) golden_paths += curLine;
    regex_search(golden_paths, match,regex("Longest delay = (\\d+\\.\\d+)"));
    golden_longestDelay = match[1].str();
    regex_search(golden_paths, match,regex("Shortest delay = (\\d+\\.\\d+)"));
    golden_shortestDelay = match[1].str();
    golden.close();

    ifstream dut(DUT_path);
    string dut_paths;
    string dut_longestDelay, dut_shortestDelay;
    while(getline(dut,curLine)) dut_paths += curLine;
    regex_search(dut_paths, match,regex("Longest delay = (\\d+\\.\\d+)"));
    dut_longestDelay = match[1].str();
    regex_search(dut_paths, match,regex("Shortest delay = (\\d+\\.\\d+)"));
    dut_shortestDelay = match[1].str();
    dut.close();

    return (golden_longestDelay == dut_longestDelay) && (golden_shortestDelay == dut_shortestDelay);
}

int main(int argc, char* argv[]){
    // argv[5]: studentID_caseName_path.txt
    // argv[6]: golden_caseName_path.txt

    // ****************
    // *    Step 3    *
    // ****************
    if(step3(argv[1],argv[2])) cout << "step3 correct." << '\n';
    else cout << "step3 wrong." << '\n';
    
    return 0;
}