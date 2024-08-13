#include "STA.h"

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

    return 0;
}