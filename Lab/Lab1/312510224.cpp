#include "QM.h"
// #define TESTMODE

#include <ctime>

int main(int argc, char* argv[]){
    #ifdef TESTMODE
    clock_t start = clock();
    #endif

    QuineMcclusky qm;
    qm.readfile(argv[1]);
    qm.buildImplicationTable();
    while(qm.growImplicant()){ };
    qm.columnCovering();

    // print result
    ofstream output(argv[2]);
    qm.printImplicants(output);
    qm.printMinimumCovering(output);
    output.close();

    #ifdef TESTMODE
    cout << (double)(clock()-start) / CLOCKS_PER_SEC * 1000 << "ms" << '\n';
    #endif

    return 0;
}