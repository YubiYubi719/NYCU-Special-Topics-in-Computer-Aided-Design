#ifndef CELL_H
#define CELL_H
#include <vector>
#include <string>
using namespace std;

class Net;

class Cell{
public:
    Cell();
    Cell(string &gateName, string &gateType, Net* &oNet);
    ~Cell();

    string name;
    string type;
    double inputTransition, outputTransition;
    double riseTransition, fallTransition;
    double outputLoad;
    double delay; // intrinsic cell delay (i.e. propagation delay)
    double arrivalTime; // sum of the total delay of the path from input til current cell
    vector<Net*> inputNet;
    Net* outputNet;
    int inDegree;
    Cell* prevCell; // record the gate of latest arrived signal
    char worstCaseValue; // used in step 2 for analyzing worst case output delay
    char value; // used in step 4 as real gate value correspond to the input pattern
    char controlledValue;
};

#endif