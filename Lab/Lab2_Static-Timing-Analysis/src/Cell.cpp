#include "Cell.h"

Cell::Cell():
    inputTransition(0.0),
    outputTransition(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    delay(0.0),
    arrivalTime(0.0),
    inDegree(0),
    prevCell(nullptr),
    worstCaseValue(' '),
    value(' '){
    ;
}

Cell::Cell(
    string &gateName, 
    string &gateType, 
    Net* &oNet
) : name(gateName), 
    type(gateType), 
    inputTransition(0.0),
    outputTransition(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    delay(0.0),
    arrivalTime(0.0),
    outputNet(oNet),
    inDegree(0),
    prevCell(nullptr),
    worstCaseValue(' '),
    value(' '){
    if(gateType == "INVX1")         controlledValue = 'X'; // both 0 and 1 are controlling value
    else if(gateType == "NANDX1")   controlledValue = '0';
    else /* gateType == "NOR2X1" */ controlledValue = '1';
}

Cell::~Cell(){
    ;
}

bool Cell::cmpWithGateOrder(const Cell* const &cell_1, const Cell* const &cell_2){
    return stoi(cell_1->name.substr(1)) < stoi(cell_2->name.substr(1));
}