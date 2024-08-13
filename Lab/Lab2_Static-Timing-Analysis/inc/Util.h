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

enum OP_Type{
    NetType,
    CellType,
    Others
};

const unordered_map<string, OP_Type> OP_map = {
    {"input"     , NetType  },
    {"output"    , NetType  },
    {"wire"      , NetType  },
    {"NANDX1"    , CellType },
    {"NOR2X1"    , CellType },
    {"INVX1"     , CellType },
    {"module"    , Others   },
    {"endmodule" , Others   }
};

const unordered_map<string, FunctionPtr> truthTable = {
    {"NANDX1", NAND_truthTable},
    {"NOR2X1", NOR_truthTable }
};

#endif