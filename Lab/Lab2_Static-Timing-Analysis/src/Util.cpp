#include "Util.h"

char NAND_truthTable(const bool &a, const bool &b){
    return (a && b) ? '0' : '1';
}

char NOR_truthTable(const bool &a, const bool &b){
    return (a || b) ? '0' : '1';
}