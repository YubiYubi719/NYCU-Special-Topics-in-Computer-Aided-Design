#include "Util.h"

char NAND_truthTable(const char &a, const char &b){
    int input = ((a - '0') << 1) + (b - '0');
    return (input == 3) ? '0' : '1'; // NAND: 11 -> 0
}

char NOR_truthTable(const char &a, const char &b){
    int input = ((a - '0') << 1) + (b - '0');
    return (input == 0) ? '1' : '0'; // NOR: 00 -> 1
}