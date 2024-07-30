#include "QM.h"

int main(int argc, char* argv[]){
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

    return 0;
}