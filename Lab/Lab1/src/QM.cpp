#include "QM.h"

QM::QM():varNum(0){
    ;
}

QM::~QM(){
    ;
}

void QM::build(string filename){
    ifstream input(filename);
    string curString;
    // variable num
    input >> curString >> varNum; // ignore .i
    // on-set
    input >> curString; // ignore .m
    input.ignore(2,'\n');
    getline(input,curString);
    stringstream ss(curString);
    int onset_pos;
    while(ss >> onset_pos){
        on_set.push_back(onset_pos);
    }
    // don't-care set
    input >> curString; // ignore .d
    input.ignore(2,'\n');
    ss.clear();
    ss.str("");
    getline(input,curString);
    ss.str(curString);
    int dc_pos;
    while(ss >> dc_pos){
        dc_set.push_back(dc_pos);
    }

    input.close();
}