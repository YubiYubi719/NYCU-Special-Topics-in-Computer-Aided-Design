#ifndef NET_H
#define NET_H
#include <vector>
#include <string>
#include "Util.h"
using namespace std;

class Cell;

class Net{
public:
    Net();
    Net(const string &netName, const NetType &netType);
    ~Net();

    string name;
    NetType type;
    bool value;
    Cell* inputCell;
    vector<Cell*> outputCell;
};

#endif