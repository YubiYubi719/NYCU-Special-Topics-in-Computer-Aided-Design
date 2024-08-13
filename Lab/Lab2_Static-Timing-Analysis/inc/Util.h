#ifndef UTIL_H
#define UTIL_H
#include <unordered_map>
#include <string>
using namespace std;

#define WIRE_DELAY  0.005
#define OUTPUT_LOAD 0.03

using FunctionPtr = char (*)(pair<char,char>);
char NAND_truthTable(pair<char,char> p);
char NOR_truthTable(pair<char,char> p);

enum class OP_Type{
    Net,
    Cell,
    Others
};

const unordered_map<string, OP_Type> OP_map = {
    {"input"     , OP_Type::Net    },
    {"output"    , OP_Type::Net    },
    {"wire"      , OP_Type::Net    },
    {"NANDX1"    , OP_Type::Cell   },
    {"NOR2X1"    , OP_Type::Cell   },
    {"INVX1"     , OP_Type::Cell   },
    {"module"    , OP_Type::Others },
    {"endmodule" , OP_Type::Others }
};

const unordered_map<string, FunctionPtr> truthTable = {
    {"NANDX1", NAND_truthTable},
    {"NOR2X1", NOR_truthTable }
};

#endif