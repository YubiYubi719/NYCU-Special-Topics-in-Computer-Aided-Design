#ifndef CELLINFO_H
#define CELLINFO_H
#include <array>
#include "Util.h"
using namespace std;

class CellInfo{
public:
    CellInfo();
    ~CellInfo();
    
    array<double,2> pinCap;
    array<array<double,49>,4> tables; // first: TableType (cell_rise, cell_fall, rise_transition, fall_transition)
                                      // second: look-up table
};

#endif