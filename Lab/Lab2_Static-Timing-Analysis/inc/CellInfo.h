#ifndef CELLINFO_H
#define CELLINFO_H
#include <bits/stdc++.h>
#include <unordered_map>
#include "Util.h"
using namespace std;

class CellInfo{
public:
    CellInfo();
    ~CellInfo();
    
    vector<double> pinCap;
    vector<vector<double>> tables; // first: TableType (cell_rise, cell_fall, rise_transition, fall_transition)
                                   // second: look-up table
};

#endif