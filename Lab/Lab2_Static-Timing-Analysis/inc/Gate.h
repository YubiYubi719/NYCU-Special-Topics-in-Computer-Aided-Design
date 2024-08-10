#ifndef GATE_H
#define GATE_H
#include <vector>
#include <string>
using namespace std;

class Net;

class Gate{
public:
    Gate();
    Gate(string &gateName, string &gateType, vector<Net*> &iNet, Net* &oNet);
    ~Gate();

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