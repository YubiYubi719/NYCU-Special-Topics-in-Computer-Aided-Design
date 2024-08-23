#ifndef CELL_H
#define CELL_H
#include <vector>
#include <string>
#include "Util.h"
using namespace std;

class Net;

class Cell{
public:
    Cell();
    Cell(string &cellName, CellType cellType);
    ~Cell();

    static bool cmpWithGateOrder(const Cell* const &cell_1, const Cell* const &cell_2);

    string name;
    CellType type;
    double inputTransition, outputTransition;
    double outputLoad;
    double delay; // intrinsic cell delay (i.e. propagation delay)
    double arrivalTime; // sum of the total delay of the path from input til current cell input
    vector<Net*> inputNet;
    Net* outputNet;
    int inDegree;
    Cell* prevCell; // record the gate of latest arrived signal
    char worstCaseValue; // used in step 2 for analyzing worst case output delay
    char value;  // used in step 4 as real gate value correspond to the input pattern
    char controllingValue;
    size_t number;
};

#endif