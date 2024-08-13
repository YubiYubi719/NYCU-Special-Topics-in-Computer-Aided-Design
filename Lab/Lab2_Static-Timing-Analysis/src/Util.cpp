#include "Util.h"

char NAND_truthTable(pair<char,char> p){
    if(p.first == '0' && p.second == '0') return '1';
    if(p.first == '0' && p.second == '1') return '1';
    if(p.first == '1' && p.second == '0') return '1';
    if(p.first == '1' && p.second == '1') return '0';
    return 'X';
}

char NOR_truthTable(pair<char,char> p){
    if(p.first == '0' && p.second == '0') return '1';
    if(p.first == '0' && p.second == '1') return '0';
    if(p.first == '1' && p.second == '0') return '0';
    if(p.first == '1' && p.second == '1') return '0';
    return 'X';
}
