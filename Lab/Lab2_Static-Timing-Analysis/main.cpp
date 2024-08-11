#include "STA.h"

int main(int argc, char* argv[]){
    STA sta;
    sta.verilogParser(argv[1]);
    sta.libraryParser(argv[3]);

    return 0;
}