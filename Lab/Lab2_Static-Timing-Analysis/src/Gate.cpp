#include "Gate.h"

Gate::Gate():
    inputCap(0.0),
    riseTime(0.0),
    fallTime(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0){
    ;
}

Gate::Gate(
    string &gateName, 
    string &gateType, 
    vector<Net*> &iNet, 
    Net* &oNet
) : name(gateName), 
    type(gateType), 
    inputNet(iNet), 
    outputNet(oNet),
    inputCap(0.0),
    riseTime(0.0),
    fallTime(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0){
    ;
}

Gate::~Gate(){
    ;
}