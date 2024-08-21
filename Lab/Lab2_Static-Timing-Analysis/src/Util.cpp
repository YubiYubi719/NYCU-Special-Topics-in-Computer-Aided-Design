#include "Util.h"

char NAND_truthTable(const pair<char,char> &p){
    int input = ((p.first - '0') << 1) + (p.second - '0');
    return (input == 3) ? '0' : '1'; // NAND: 11 -> 0
}

char NOR_truthTable(const pair<char,char> &p){
    int input = ((p.first - '0') << 1) + (p.second - '0');
    return (input == 0) ? '1' : '0'; // NOR: 00 -> 1
}