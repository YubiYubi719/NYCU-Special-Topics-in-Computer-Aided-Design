#ifndef TABLE_H
#define TABLE_H
#include <unordered_map>
#include <string>
using namespace std;

enum class OP_Type{
    input,
    output,
    wire,
    NANDX1,
    NOR2X1,
    INVX1,
    module,
    endmodule
};

const unordered_map<string, OP_Type> OP_map = {
    {"input"     , OP_Type::input      },
    {"output"    , OP_Type::output     },
    {"wire"      , OP_Type::wire       },
    {"NANDX1"    , OP_Type::NANDX1     },
    {"NOR2X1"    , OP_Type::NOR2X1     },
    {"INVX1"     , OP_Type::INVX1      },
    {"module"    , OP_Type::module     },
    {"endmodule" , OP_Type::endmodule  }
};

#endif