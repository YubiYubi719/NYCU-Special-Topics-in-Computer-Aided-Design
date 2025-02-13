#ifndef STA_H
#define STA_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <array>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include <regex>
#include <cassert>
#include <cfloat>
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
    string removeComment(const string &code);
    double interpolate(
        const double &inputTransition,
        const double &outputLoad,
        const double* const &table, 
        const size_t &col_idx, 
        const size_t &row_idx
    );
    double tableLookUp(const Cell* const &cell, const TableType &tableType);
    void calInputTransitionTime(Cell* const &cell);
    vector<Net*> findPath(Cell* cell);
    void calInputTransitionTime_Simulate(Cell* const &cell);
    void simulate(const vector<short> &pattern);
    void dumpGateInfo(ostringstream &oss, const vector<Cell*> &cellsInGateOrder);

//  *********************************
//  *        Member Variables       *
//  *********************************
    size_t inputNum; // number of input net
    string netlistName;
    string patternName;
    unordered_map<string,Net*> nets;
    vector<Cell*> cells;
    vector<string> patternOrder;
    vector<vector<short>> patterns;
    vector<Cell*> t_sort;
    Library cellLib;
    double maxDelay, minDelay;
    vector<Cell*> outputCell;
    vector<Net*> longestPath;
    vector<Net*> shortestPath;
};

#endif