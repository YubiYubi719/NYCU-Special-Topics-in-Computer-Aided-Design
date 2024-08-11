#ifndef NET_H
#define NET_H
#include <list>
#include <string>
using namespace std;

class Cell;

class Net{
public:
    Net();
    Net(const string &netName, const string &netType);
    ~Net();

    string name;
    string type;
    Cell* inputCell;
    list<Cell*> outputGate;
};

#endif