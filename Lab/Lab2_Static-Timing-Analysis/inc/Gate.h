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
    vector<Net*> inputNet;
    Net* outputNet;
};

#endif