#include "QM.h"

int main(int argc, char* argv[]){
    QM qm;
    qm.readfile(argv[1]);
    qm.buildImplicationTable();
    int cnt = 1;
    while(qm.growImplicant()){
        // cout << "Ieration:" << cnt << '\n';
        // ++cnt;
    };
    qm.columnCovering();
    qm.printImplicants();

    return 0;
}