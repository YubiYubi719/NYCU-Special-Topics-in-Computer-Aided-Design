#ifndef GATE_H
#define GATE_H
#include <vector>
#include <string>
using namespace std;

class Net;

class Gate{
public:
    Gate();
    ~Gate();

    string type;
    string name;
    double inputCap;
    double riseTime, fallTime;
    double riseTransition, fallTransition;
    double outputLoad;
    vector<Net*> inputNet;
    Net* outputNet;
};

#endif