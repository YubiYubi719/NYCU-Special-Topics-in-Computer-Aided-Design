#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;
#define EPSILON 1

enum Status{
    uncovered,
    covered,
    err
};

class Cell{
public:
    string value;
    int delay;
    int transitionTime;
    int operator==(const Cell &rhs){
        if( value == rhs.value
        && (abs(delay - rhs.delay) <= EPSILON)
        && (abs(transitionTime - rhs.transitionTime) <= EPSILON)) return true;
        return false;
    }
};

ostream& operator<<(ostream &output, const Cell& cell){
    string delay = to_string(cell.delay);
    while(delay.length() < 6) delay = "0" + delay;
    string transTime = to_string(cell.transitionTime);
    while(transTime.length() < 6) transTime = "0" + transTime;
    output << "Value: "    << cell.value << ", "
            << "Delay: 0." << delay      << ", "
            << "Transition time: 0." << transTime << " ";
    return output;
}

bool step1(const string DUT_path, const string GOLDEN_path){
    unordered_map<string,pair<Cell,Status>> mp; // key: cellName, value.first: goldenLoad, value.second: status
    vector<Cell> wrongCells;
    string curLine;
    // Read golden ans
    ifstream golden(GOLDEN_path);
    while(getline(golden,curLine)){
        if(curLine.empty()) continue;
        stringstream ss(curLine);
        string cellName, v, d, t;
        ss >> cellName >> v >> d >> t;
        mp[cellName] = {{v,stoi(d.substr(2)),stoi(t.substr(2))}, uncovered};
    }
    golden.close();

    // Read user ans
    ifstream dut(DUT_path);
    while(getline(dut,curLine)){
        if(curLine.empty()) continue;
        stringstream ss(curLine);
        string cellName, v, d, t;
        ss >> cellName >> v >> d >> t;
        Cell userCell = {v,stoi(d.substr(2)),stoi(t.substr(2))};
        if(mp.count(cellName)){
            const Cell &goldenCell = mp[cellName].first;
            if(userCell == goldenCell){
                mp[cellName].second = covered;
            }
            else{
                mp[cellName].second = err;
                wrongCells.push_back(userCell);
            }
        }
    }

    size_t i = 0;
    bool isCorrect = true;
    for(const auto &p : mp){
        const Status &status = p.second.second;
        if(status == err){
            cout << "Wrong output at " << p.first 
                 << " | Golden: " << p.second.first
                 << " | Yours: "   << wrongCells[i] << '\n';
            i++;
            isCorrect = false;
        }
        else if(status == uncovered) isCorrect = false;
    }
    return isCorrect;
}

int main(int argc, char* argv[]){
    // argv[1]: studentID_caseName_delay.txt
    // argv[2]: golden_caseName_delay.txt
    
    // ****************
    // *    Step 2    *
    // ****************
    if(step1(argv[1],argv[2])) cout << "step2 correct." << '\n';
    else cout << "step2 wrong." << '\n';

    return 0;
}