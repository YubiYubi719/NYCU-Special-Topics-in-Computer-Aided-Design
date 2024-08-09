#ifndef TABLE_H
#define TABLE_H
#include <unordered_map>
#include <string>
using namespace std;

enum class OP_Type{
    Net,
    Gate,
    Others
};

const unordered_map<string, OP_Type> OP_map = {
    {"input"     , OP_Type::Net    },
    {"output"    , OP_Type::Net    },
    {"wire"      , OP_Type::Net    },
    {"NANDX1"    , OP_Type::Gate   },
    {"NOR2X1"    , OP_Type::Gate   },
    {"INVX1"     , OP_Type::Gate   },
    {"module"    , OP_Type::Others },
    {"endmodule" , OP_Type::Others }
};

#endif