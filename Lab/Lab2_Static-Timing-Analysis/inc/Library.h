#ifndef LIBRARY_H
#define LIBRARY_H
#include <bits/stdc++.h>
#include <unordered_map>
#include "Util.h"
#include "CellInfo.h"
using namespace std;

class Library{
public:
    Library();
    ~Library();
    
    vector<double> index_1;
    vector<double> index_2;
    vector<CellInfo*> cellInfos; // index: CellType
                                 // value: CellInfo
};

#endif