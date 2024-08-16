#ifndef LIBRARY_FIXED_H
#define LIBRARY_FIXED_H
#include <bits/stdc++.h>
#include <unordered_map>
#include "CellInfo_fixed.h"
using namespace std;

class Library{
public:
    Library();
    ~Library();
    
    const vector<double> index_1 = {0.001400,0.003000,0.006200,0.012500,0.025100,0.050400,0.101000};
    const vector<double> index_2 = {0.0208,0.0336,0.06,0.1112,0.2136,0.4192,0.8304};
    
    const NOR2X1_INF NOR2X1_inf;
    const NANDX1_INF NANDX1_inf;
    const INVX1_INF  INVX1_inf;
};

#endif