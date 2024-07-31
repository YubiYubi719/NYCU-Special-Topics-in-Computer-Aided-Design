#include "QM.h"
#include <ctime>

int main(int argc, char* argv[]){
    clock_t start = clock();
    QuineMcclusky qm;
    qm.readfile(argv[1]);
    qm.buildImplicationTable();
    // int cnt = 1;
    while(qm.growImplicant()){
        // cout << "Ieration:" << cnt << '\n';
        // ++cnt;
    };
    qm.columnCovering();

    // print result
    ofstream output(argv[2]);
    qm.printImplicants(output);
    qm.printMinimumCovering(output);
    output.close();

    cout << (double)(clock()-start) / CLOCKS_PER_SEC * 1000 << "ms" << '\n';

    return 0;
}