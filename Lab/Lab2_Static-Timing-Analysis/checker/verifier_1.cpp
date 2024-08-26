#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define EPSILON 1

enum Status{
    uncovered,
    covered,
    err
};

bool step1(const string DUT_path, const string GOLDEN_path){
    unordered_map<string,pair<int,Status>> mp; // key: cellName, value.first: goldenLoad, value.second: status
    vector<int> wrongLoads;
    string curLine;
    // Read golden ans
    ifstream golden(GOLDEN_path);
    while(getline(golden,curLine)){
        if(curLine.empty()) continue;
        stringstream ss(curLine);
        string cellName, outputLoad;
        ss >> cellName >> outputLoad;
        mp[cellName] = {stoi(outputLoad.substr(2)) , uncovered};
    }
    golden.close();

    // Read user ans
    ifstream dut(DUT_path);
    while(getline(dut,curLine)){
        if(curLine.empty()) continue;
        stringstream ss(curLine);
        string cellName, load;
        ss >> cellName >> load;
        int userLoad = stoi(load.substr(2));
        if(mp.count(cellName)){
            int goldenLoad = mp[cellName].first;
            if(abs(userLoad - goldenLoad) <= EPSILON){
                mp[cellName].second = covered;
            }
            else{
                mp[cellName].second = err;
                wrongLoads.push_back(userLoad);
            }
        }
    }

    size_t i = 0;
    bool isCorrect = true;
    for(const auto &p : mp){
        const Status &status = p.second.second;
        if(status == err){
            string goldenLoad = to_string(p.second.first);
            while(goldenLoad.length() < 6) goldenLoad = "0" + goldenLoad;
            string wrongLoad = to_string(wrongLoads[i]);
            while(wrongLoad.length() < 6) wrongLoad = "0" + wrongLoad;
            cout << "Wrong outputLoad at " << p.first 
                 << " | Golden load: 0." << goldenLoad
                 << " | Your load: 0."   << wrongLoad << '\n';
            i++;
            isCorrect = false;
        }
        else if(status == uncovered) isCorrect = false;
    }
    return isCorrect;
}

int main(int argc, char* argv[]){
    // argv[1]: studentID_caseName_load.txt
    // argv[2]: golden_caseName_load.txt
    
    // ****************
    // *    Step 1    *
    // ****************
    if(step1(argv[1],argv[2])) cout << "step1 correct." << '\n';
    else cout << "step1 wrong." << '\n';

    return 0;
}