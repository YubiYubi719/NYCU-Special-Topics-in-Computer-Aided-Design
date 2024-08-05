#include "Implicant.h"

Implicant::Implicant():combinable(false){
    ;
}

Implicant::Implicant(string bin):binary(bin), combinable(false){
    ;
}

Implicant::~Implicant(){
    ;
}

bool Implicant::operator==(const Implicant &rhs) const{
    return binary == rhs.binary;
}

size_t Implicant::Hash::operator()(const Implicant &imp) const{
    size_t h1 = hash<string>()(imp.binary);
    size_t h2 = hash<bool>()(imp.combinable);
    return h1 ^ (h2 << 1);
}