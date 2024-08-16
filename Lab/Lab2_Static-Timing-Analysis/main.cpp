#include "STA.h"
#include <ctime>

string netlistPath;
string libraryPath;
string patternPath;

void extractPath(int argc, char* argv[]){
    for(int i = 1; i < argc; i++){
        string cur_arg(argv[i]);
        if(cur_arg.back() == 'v') netlistPath = cur_arg;
        else if(cur_arg == "-l")  libraryPath = argv[++i];
        else if(cur_arg == "-i")  patternPath = argv[++i];
    }
}

int main(int argc, char* argv[]){
    #if PRINTTIME
    clock_t start = clock();
    #endif
    STA sta;
    extractPath(argc,argv);
    sta.verilogParser(netlistPath);
    sta.libraryParser(libraryPath);
    sta.patternParser(patternPath);

    // Step 1
    sta.calOutputLoad();
    sta.dumpOutputLoad();

    // Step 2
    sta.calPropagationDelay();
    sta.dumpDelay();

    // Step 3
    sta.pathFinding();
    sta.dumpPath();

    // Step 4
    sta.assignPattern();

    #if PRINTTIME
    cout << (double)(clock() - start) / CLOCKS_PER_SEC * 1000 << "ms" << '\n';
    #endif

    return 0;
}