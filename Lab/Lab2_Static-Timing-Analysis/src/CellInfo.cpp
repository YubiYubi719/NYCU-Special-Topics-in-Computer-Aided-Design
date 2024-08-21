#include "CellInfo.h"

CellInfo::CellInfo(){
    tables.resize(4);
    for(vector<double> &table : tables) table.reserve(49);
}

CellInfo::~CellInfo(){
    ;
}