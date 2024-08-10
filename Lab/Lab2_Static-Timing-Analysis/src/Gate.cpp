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
    Net* &oNet
) : name(gateName), 
    type(gateType), 
    inputCap(0.0),
    riseTime(0.0),
    fallTime(0.0),
    riseTransition(0.0),
    fallTransition(0.0),
    outputLoad(0.0),
    outputNet(oNet),
    A1(nullptr),
    A2(nullptr),
    I(nullptr){
    ;
}

Gate::~Gate(){
    ;
}