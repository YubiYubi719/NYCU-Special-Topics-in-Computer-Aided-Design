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
    worstCaseValue(' '){
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
    worstCaseValue(' '){
    ;
}

Cell::~Cell(){
    ;
}