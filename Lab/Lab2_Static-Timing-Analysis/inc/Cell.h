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
    double inputCap;
    double riseTime, fallTime;
    double riseTransition, fallTransition;
    double outputLoad;
    vector<Net*> inputNet;
    Net* outputNet;
};

#endif