#include "Net.h"

Net::Net(){
    ;
}

Net::Net(
    const string &netName, 
    const string &netType
) : name(netName), 
    type(netType),
    inputCell(nullptr){
    ;
}

Net::~Net(){
    ;
}