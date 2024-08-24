#ifndef UTIL_H
#define UTIL_H
#include <unordered_map>
#include <string>
#include <regex>
using namespace std;

#define WIRE_DELAY  0.005
#define OUTPUT_LOAD 0.03
#define MAX_THREADS 8
#define HIGH '1'
#define LOW  '0'
#define DONTCARE 'X'


enum CellType{
    NANDX1,
    NOR2X1,
    INVX1
};

enum NetType{
    input,
    output,
    wire
};

enum TableType{
    cell_rise,
    cell_fall,
    rise_transition,
    fall_transition
};

inline CellType str2CellType(const string &type){
    if(type[1] == 'N') return INVX1;
    if(type[1] == 'A') return NANDX1;
    return NOR2X1;
}
inline NetType str2NetType(const string &type){
    if(type[0] == 'i') return input;
    if(type[0] == 'o') return output;
    return wire;
}
inline TableType str2TableType(const string &type){
    if(type[5] == 'r') return cell_rise;
    if(type[5] == 'f') return cell_fall;
    if(type[0] == 'r') return rise_transition;
    return fall_transition;
}

const regex Comment_Pattern_1("//(?!.*\\*/).*\n");
const regex Comment_Pattern_2("/\\*[\\s\\S]*?\\*/");
const regex Comment_Pattern_3("//.*\n");

const regex Word_Pattern("\\w+");

const regex Index_1_Pattern("index_1");
const regex Index_2_Pattern("index_2");
const regex Float_Pattern("\\d+\\.\\d+");
const regex Cell_Pattern("cell *\\((\\w+)\\)");
const regex Pin_Pattern("pin\\(([AI]\\d*)\\)");
const regex Capacitance_Pattern("capacitance *: *(\\d+\\.\\d+);");
const regex Table_Pattern("(cell_rise)|(cell_fall)|(rise_transition)|(fall_transition)");

using TablePtr = char (*)(const pair<char,char> &);
char NAND_truthTable(const pair<char,char> &p);
char NOR_truthTable(const pair<char,char> &p);

const vector<TablePtr> truthTable = { NAND_truthTable, NOR_truthTable };

#endif