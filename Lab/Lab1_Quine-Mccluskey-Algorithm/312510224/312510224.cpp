#include "QM.h"
#include <ctime>

int main(int argc, char* argv[]){
    #if TESTMODE
    clock_t start = clock();
    #endif

    QuineMccluskey qm;
    qm.readfile(argv[1]);
    qm.buildImplicationTable();
    while(qm.growImplicant()){ };
    qm.columnCovering();

    // print result
    ofstream output(argv[2]);
    qm.printImplicants(output);
    qm.printMinimumCovering(output);
    output.close();

    #if TESTMODE
    cout << (double)(clock()-start) / CLOCKS_PER_SEC * 1000 << "ms" << '\n';
    #endif

    return 0;
}