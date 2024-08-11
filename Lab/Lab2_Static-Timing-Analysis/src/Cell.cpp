#include "Cell.h"

Cell::Cell():
    inputCap(0.0),
    riseTime(0.0),
    fallTime(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0){
    ;
}

Cell::Cell(
    string &gateName, 
    string &gateType, 
    Net* &oNet
) : name(gateName), 
    type(gateType), 
    inputCap(0.0),
    riseTime(0.0),
    fallTime(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    outputNet(oNet){
    ;
}

Cell::~Cell(){
    ;
}