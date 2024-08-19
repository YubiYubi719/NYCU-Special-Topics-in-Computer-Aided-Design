#ifndef CELLINFO_H
#define CELLINFO_H
#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

class CellInfo{
public:
    CellInfo();
    ~CellInfo();
    
    struct OBJ{
        string tableType;
        double inputTransition;
        double outputLoad;
    };

    vector<double> pinCap;
    unordered_map<string,vector<double>> tables; // first: table type (cell_rise, cell_fall, rise_transition, fall_transition)
                                                 // second: look-up table
};

#endif