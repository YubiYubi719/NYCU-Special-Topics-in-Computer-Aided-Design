#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <cmath>

using namespace std;

void compareFiles(const string& file1, const string& file2) {
    ifstream inputFile1(file1);
    ifstream inputFile2(file2);

    if (!inputFile1.is_open() || !inputFile2.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    struct GateInfo {
        int logicValue;
        double delay;
        double transTime;
    };

    map<string, GateInfo> gateMap1, gateMap2;

    string line;
    while (getline(inputFile1, line)) {
        istringstream iss(line);
        string gateName;
        int logicValue;
        double delay, transTime;
        iss >> gateName >> logicValue >> delay >> transTime;
        gateMap1[gateName] = {logicValue, delay, transTime};
    }

    while (getline(inputFile2, line)) {
        istringstream iss(line);
        string gateName;
        int logicValue;
        double delay, transTime;
        iss >> gateName >> logicValue >> delay >> transTime;
        gateMap2[gateName] = {logicValue, delay, transTime};
    }

    inputFile1.close();
    inputFile2.close();

    bool filesAreEquivalent = true;

    // Compare gateMap1 with gateMap2
    for (const auto& pair : gateMap1) {
        const string& gateName = pair.first;
        const GateInfo& info1 = pair.second;

        if (gateMap2.find(gateName) == gateMap2.end()) {
            cout << "Gate " << gateName << " is missing in " << file2 << endl;
            filesAreEquivalent = false;
        } else {
            const GateInfo& info2 = gateMap2[gateName];
            if (info1.logicValue != info2.logicValue) {
                cout << "Difference in gate " << gateName << " logic value: "
                     << file1 << " has " << info1.logicValue << ", "
                     << file2 << " has " << info2.logicValue << endl;
                filesAreEquivalent = false;
            }
            if (abs(info1.delay - info2.delay) > 1e-6) {
                cout << "Difference in gate " << gateName << " delay: "
                     << file1 << " has " << fixed << setprecision(6) << info1.delay << ", "
                     << file2 << " has " << fixed << setprecision(6) << info2.delay << endl;
                filesAreEquivalent = false;
            }
            if (abs(info1.transTime - info2.transTime) > 1e-6) {
                cout << "Difference in gate " << gateName << " transition time: "
                     << file1 << " has " << fixed << setprecision(6) << info1.transTime << ", "
                     << file2 << " has " << fixed << setprecision(6) << info2.transTime << endl;
                filesAreEquivalent = false;
            }
        }
    }

    // Check for gates that are in gateMap2 but not in gateMap1
    for (const auto& pair : gateMap2) {
        const string& gateName = pair.first;
        if (gateMap1.find(gateName) == gateMap1.end()) {
            cout << "Gate " << gateName << " is missing in " << file1 << endl;
            filesAreEquivalent = false;
        }
    }

    if (filesAreEquivalent) {
        cout << "The files are equivalent." << endl;
    } else {
        cout << "The files are different." << endl;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <file1> <file2>" << endl;
        return 1;
    }

    string file1 = argv[1];
    string file2 = argv[2];

    compareFiles(file1, file2);

    return 0;
}
