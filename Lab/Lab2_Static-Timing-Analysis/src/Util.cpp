#include "Util.h"

char NAND_truthTable(const char &a, const char &b){
    bool A = (a == '1');
    bool B = (b == '1');
    return (A && B) ? '0' : '1';
}

char NOR_truthTable(const char &a, const char &b){
    bool A = (a == '1');
    bool B = (b == '1');
    return (A || B) ? '0' : '1';
}