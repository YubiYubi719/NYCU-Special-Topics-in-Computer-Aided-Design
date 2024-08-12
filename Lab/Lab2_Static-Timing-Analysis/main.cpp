#include "STA.h"

int main(int argc, char* argv[]){
    STA sta;
    sta.verilogParser(argv[1]);
    sta.libraryParser(argv[3]);

    // Step 1
    sta.calOutputLoad();
    sta.dumpOutputLoad(argv[1]);

    // Step 2
    sta.calPropagationDelay();
    sta.dumpDelay(argv[1]);

    // Step 3
    

    return 0;
}