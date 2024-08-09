#ifndef NET_H
#define NET_H
#include <list>
#include <string>
using namespace std;

class Gate;

class Net{
public:
    Net();
    ~Net();

    string type;
    Gate* inputGate;
    list<Gate*> outputGate;
};

#endif