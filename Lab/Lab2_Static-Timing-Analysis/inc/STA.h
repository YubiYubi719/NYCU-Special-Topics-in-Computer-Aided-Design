#ifndef STA_H
#define STA_H
#include <bits/stdc++.h>
#include <regex>
#include "Table.h"
#include "Net.h"
#include "Cell.h"
#include "Library.h"
using namespace std;

class STA{
public:
    // Constructor & Destructor
    STA();
    ~STA();

    // Member functions
    string removeComment(string code);
    // Parser
    void verilogParser(const string &filename);
    void libraryParser(const string &filename);
    // Step 1
    void calOutputLoad();
    void dumpOutputLoad(string case_name);
    // Step 2 & 3
    double tableLookUp();
    void topologicalSort();
    void traverseNetList();


    // Member variables
    unordered_map<string,Net*> netMap;
    unordered_map<string,Cell*> cellMap;
    vector<Cell*> Tsort_cells;
    Library cellLib;
};

#endif