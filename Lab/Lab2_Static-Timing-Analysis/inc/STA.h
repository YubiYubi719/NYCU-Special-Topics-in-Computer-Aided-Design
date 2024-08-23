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
//  *********************************
//  *    Constructor & Destructor   *
//  *********************************
public:
    STA();
    ~STA();

//  *********************************
//  *        Member Functions       *
//  *********************************
    // Parser
    void verilogParser(const string &netlistPath);
    void libraryParser(const string &libraryPath);
    void patternParser(const string &patternPath);
    void topologicalSort();
    // Step 1
    void calOutputLoad();
    void dumpOutputLoad();
    // Step 2
    void calPropagationDelay();
    void dumpPropagationDelay();
    // Step 3
    void pathFinding();
    void dumpPath();
    // Step 4
    void assignPattern();

private:
//  *********************************
//  *        Member Functions       *
//  *********************************
    string removeComment(string &code);
    double interpolate(
        const double &inputTransition,
        const double &outputLoad,
        const vector<double> &table, 
        const double &col_idx, 
        const double &row_idx
    );
    double tableLookUp(const Cell* const &cell, const TableType &tableType);
    void calInputTransitionTime(Cell* const &cell);
    vector<Net*> findPath(Cell* cell);
    void calInputTransitionTime_Simulate(Cell* const &cell);
    void simulate(const vector<char> &pattern);
    void dumpGateInfo(ofstream &output, const vector<Cell*> &cells);

//  *********************************
//  *        Member Variables       *
//  *********************************
    size_t inputNum; // number of input net
    string netlistName;
    string patternName;
    unordered_map<string,Net*>  netMap;
    unordered_map<size_t,Cell*> cellMap;
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