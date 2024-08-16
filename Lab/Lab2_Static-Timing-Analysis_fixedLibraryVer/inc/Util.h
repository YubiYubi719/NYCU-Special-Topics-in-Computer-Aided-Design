#ifndef UTIL_H
#define UTIL_H
#include <unordered_map>
#include <string>
#include <regex>
#include "Library_fixed.h"
using namespace std;

#define WIRE_DELAY  0.005
#define OUTPUT_LOAD 0.03

const Library cellLib;

const regex Comment_Pattern_1("//(?!.*\\*/).*\n");
const regex Comment_Pattern_2("/\\*[\\s\\S]*?\\*/");
const regex Comment_Pattern_3("//.*\n");

const regex Type_Pattern("(^output)|(^input)|(^wire)|(^INVX1)|(^NANDX1)|(^NOR2X1)");
const regex Word_Pattern("\\w+");
const regex OutputNet_Pattern("ZN\\((.*?)\\)");
const regex INVX1_InputNet_Pattern("I\\((.*?)\\)");
const regex NANDX1_NOR2X1_InputNet_Pattern("(A\\d+)\\((.*?)\\).*(A\\d+)\\((.*?)\\)");

using FunctionPtr = char (*)(pair<char,char>);
char NAND_truthTable(pair<char,char> p);
char NOR_truthTable(pair<char,char> p);

const unordered_map<string, FunctionPtr> truthTable = {
    {"NANDX1", NAND_truthTable},
    {"NOR2X1", NOR_truthTable }
};

#endif