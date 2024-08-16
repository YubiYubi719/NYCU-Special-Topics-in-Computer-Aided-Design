#ifndef UTIL_H
#define UTIL_H
#include <unordered_map>
#include <string>
#include <regex>
using namespace std;

#define WIRE_DELAY  0.005
#define OUTPUT_LOAD 0.03

const regex Comment_Pattern_1("//(?!.*\\*/).*\n");
const regex Comment_Pattern_2("/\\*[\\s\\S]*?\\*/");
const regex Comment_Pattern_3("//.*\n");

const regex Type_Pattern("(^output)|(^input)|(^wire)|(^INVX1)|(^NANDX1)|(^NOR2X1)");
const regex Word_Pattern("\\w+");
const regex OutputNet_Pattern("ZN\\((.*?)\\)");
const regex INVX1_InputNet_Pattern("I\\((.*?)\\)");
const regex NANDX1_NOR2X1_InputNet_Pattern("(A\\d+)\\((.*?)\\).*(A\\d+)\\((.*?)\\)");

const regex Index_1_Pattern("index_1");
const regex Index_2_Pattern("index_2");
const regex Float_Pattern("\\d+\\.\\d+");
const regex Cell_Pattern("cell *\\((\\w+)\\)");
const regex Pin_Pattern("pin\\(([AI]\\d*)\\)");
const regex Capacitance_Pattern("capacitance *: *(\\d+\\.\\d+);");
const regex Table_Pattern("(cell_rise)|(cell_fall)|(rise_transition)|(fall_transition)");

using FunctionPtr = char (*)(pair<char,char>);
char NAND_truthTable(pair<char,char> p);
char NOR_truthTable(pair<char,char> p);

const unordered_map<string, FunctionPtr> truthTable = {
    {"NANDX1", NAND_truthTable},
    {"NOR2X1", NOR_truthTable }
};

#endif