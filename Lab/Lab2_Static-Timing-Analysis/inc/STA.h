#ifndef STA_H
#define STA_H
#include <bits/stdc++.h>
#include <regex>
#include <omp.h>
#include "Util.h"
#include "Net.h"
#include "Cell.h"
#include "Library.h"
using namespace std;

class STA{
public:
//  *********************************
//  *    Constructor & Destructor   *
//  *********************************
    STA();
    ~STA();

//  *********************************
//  *        Member Functions       *
//  *********************************
    // Parser
    string removeComment(string code);
    void verilogParser(const string &netlistPath);
    void libraryParser(const string &libraryPath);
    void patternParser(const string &patternPath);
    // Step 1
    void calOutputLoad();
    void dumpOutputLoad();
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
    void dumpDelay();
    // Step 3
    vector<Net*> findPath(Cell* cell);
    void pathFinding();
    void dumpPath();
    // Step 4
    void calInputTransitionTime_Synthesis(Cell* cell);
    void simulate(const vector<char> &pattern);
    void assignPattern();
    void dumpGateInfo(ofstream &output, const vector<Cell*> &cells);

//  *********************************
//  *        Member Variables       *
//  *********************************
    size_t inputNum; // number of input net
    string netlistName;
    string patternName;
    unordered_map<string,Net*> netMap;
    unordered_map<string,Cell*> cellMap;
    vector<string> patternOrder;
    vector<vector<char>> patterns;
    vector<Cell*> t_sort;
    Library cellLib;
    double maxDelay, minDelay;
    vector<Cell*> outputCell;
    vector<Net*> longestPath;
    vector<Net*> shortestPath;
};

#endif