#include "STA.h"
#include <chrono>

string netlistPath;
string libraryPath;
string patternPath;

void extractPath(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string cur_arg(argv[i]);
        if (cur_arg.back() == 'v') netlistPath = cur_arg;
        else if  (cur_arg == "-l") libraryPath = argv[++i];
        else if  (cur_arg == "-i") patternPath = argv[++i];
    }
}

int main(int argc, char* argv[]) {

    STA sta;
    extractPath(argc, argv);
    
    #if PRINTTIME
    auto start = std::chrono::high_resolution_clock::now();
    #endif
    sta.verilogParser(netlistPath);
    #if PRINTTIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    cout << duration.count() << " ms" << std::endl;
    #endif
    sta.libraryParser(libraryPath);
    sta.patternParser(patternPath);
    sta.topologicalSort();

    // Step 1
    sta.calOutputLoad();

    // Step 2
    sta.calPropagationDelay();

    // Step 3
    sta.pathFinding();
    sta.dumpPath();

    // Step 4
    sta.assignPattern();
    

    return 0;
}