#include "Cell.h"

Cell::Cell():
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    propagationDelay(0.0){
    ;
}

Cell::Cell(
    string &gateName, 
    string &gateType, 
    Net* &oNet
) : name(gateName), 
    type(gateType), 
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    propagationDelay(0.0),
    outputNet(oNet){
    ;
}

Cell::~Cell(){
    ;
}