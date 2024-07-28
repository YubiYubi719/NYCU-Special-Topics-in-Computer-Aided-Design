#ifndef QM_H
#define QM_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include "Implicant.h"
using namespace std;

class QM{
public:
    struct ImpWithLit{
        ImpWithLit(string bin, int lit);
        string binary;
        int literal;
    };
    // Constructors/Distructors
    QM();
    ~QM();

    // Member functions
    void readfile(string filename);
    void buildImplicationTable();
    string int2Binary(int num);
    pair<string,int> int2Binary(int num, int &maxLen); // also used to calculate max length of binaries
    int binary2Int(string binary);
    bool growImplicant();
    int findDiff(string s1, string s2);
    void removeNonPrimeImplicant(list<Implicant> &curList);
    vector<int> implicant2Pos(string implicant);
    void columnCovering();
    void printImplicants();
    static bool impCmp(const ImpWithLit &imp1, const ImpWithLit &imp2);

    // Member variables
    int varNum;
    vector<int> on_set;
    vector<int> dc_set;
    int maxLen; // record maximum length of binaries
    vector<list<Implicant>> implicationTable; // ex: now we have an on-set pos 4
                                              // 4 = 0100 --> has one 1
                                              // it stores in implicationTable[1] as "0100"
    list<string> primeImplicants;
    vector<ImpWithLit> primeImp_with_literal;
};

#endif