#ifndef LIBRARY_H
#define LIBRARY_H
#include <array>
#include "Util.h"
#include "CellInfo.h"
using namespace std;

class Library{
public:
    Library();
    ~Library();
    
    array<double,7> index_1;
    array<double,7> index_2;
    array<CellInfo*,3> cellInfos; // index: CellType
                                  // value: CellInfo
};

#endif