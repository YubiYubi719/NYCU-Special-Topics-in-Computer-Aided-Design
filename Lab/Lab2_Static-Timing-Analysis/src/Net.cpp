#include "Net.h"

Net::Net() : inputCell(nullptr){
    ;
}

Net::Net(
    const string &netName, 
    const NetType &netType
) : name(netName), 
    type(netType),
    value(DONTCARE),
    inputCell(nullptr){
    ;
}

Net::~Net(){
    ;
}