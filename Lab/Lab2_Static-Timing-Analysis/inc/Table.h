#ifndef TABLE_H
#define TABLE_H
#include <unordered_map>
#include <string>
using namespace std;

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

#endif