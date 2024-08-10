#ifndef GATE_H
#define GATE_H
#include <vector>
#include <string>
using namespace std;

class Net;

class Gate{
public:
    Gate();
    Gate(string &gateName, string &gateType, Net* &oNet);
    ~Gate();

    string name;
    string type;
    double inputCap;
    double riseTime, fallTime;
    double riseTransition, fallTransition;
    double outputLoad;
    Net *A1, *A2, *I; // input
    Net* outputNet;
};

#endif