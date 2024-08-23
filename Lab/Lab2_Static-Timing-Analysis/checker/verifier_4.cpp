#include <bits/stdc++.h>
using namespace std;
#define EPSILON 1

class Cell{
public:
    string name;
    string value;
    int delay;
    int transitionTime;
    int operator==(const Cell &rhs){
        if( name == rhs.name 
        && value == rhs.value
        && (abs(delay - rhs.delay) <= EPSILON)
        && (abs(transitionTime - rhs.transitionTime) <= EPSILON)) return true;
        return false;
    }
};

bool step4(const string DUT_path, const string GOLDEN_path){
    ifstream golden(GOLDEN_path);
    ifstream dut(DUT_path);
    string goldenLine, dutLine;
    int line = 1;
    while(getline(golden, goldenLine)){
        getline(dut,dutLine);
        if(goldenLine.empty() || goldenLine == "\r"){
            line++;
            continue;
        }
        stringstream golden_ss(goldenLine);
        stringstream dut_ss(dutLine);
        vector<string> golden_inf(4), dut_inf(4);
        for(size_t i = 0; i < 4; i++){
            golden_ss >> golden_inf[i];
            dut_ss >> dut_inf[i];
        }
        Cell goldenCell = {golden_inf[0], golden_inf[1], stoi(golden_inf[2].substr(2)), stoi(golden_inf[3].substr(2))};
        Cell dutCell = {dut_inf[0], dut_inf[1], stoi(dut_inf[2].substr(2)), stoi(dut_inf[3].substr(2))};
        if(!(goldenCell == dutCell)){
            cout << "Wrong output at line " << line
                 << " | Golden: " << goldenLine
                 << " | Yours: "   << dutLine << '\n';
            return false;
        }
        line++;
    }
    return true;
}

int main(int argc, char* argv[]){
    // argv[1]: studentID_caseName_gate_info.txt
    // argv[2]: golden_caseName_gate_info.txt
    
    // ****************
    // *    Step 4    *
    // ****************
    if(step4(argv[1],argv[2])) cout << "step4 correct." << '\n';
    else cout << "step4 wrong." << '\n';
    
    return 0;
}