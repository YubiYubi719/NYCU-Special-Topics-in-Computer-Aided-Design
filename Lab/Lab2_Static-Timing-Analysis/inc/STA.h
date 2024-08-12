#ifndef STA_H
#define STA_H
#include <bits/stdc++.h>
#include <regex>
#include "Util.h"
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
    // Step 2
    double interpolate(
        const double &inputTransition,
        const double &outputLoad,
        const vector<double> &table, 
        const double &col_idx, 
        const double &row_idx
    );
    double tableLookUp(Cell* cell, string tableType);
    void topologicalSort();
    void calInputTransitionTime(Cell* cell);
    void calPropagationDelay();
    void dumpDelay(string case_name);
    // Step 3
    void findLongestPath();
    void findShortestPath();


    // Member variables
    unordered_map<string,Net*> netMap;
    unordered_map<string,Cell*> cellMap;
    vector<Cell*> t_sort;
    Library cellLib;
};

#endif