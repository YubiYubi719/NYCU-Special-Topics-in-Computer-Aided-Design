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
#include <climits>
#include "Implicant.h"
using namespace std;

class QuineMccluskey{
public:
    struct ImpWithLit{
        ImpWithLit(string bin, int lit);

        static bool impCmp(const ImpWithLit &imp1, const ImpWithLit &imp2);

        string binary;
        int literal;
    };
    // Constructors/Distructors
    QuineMccluskey();
    ~QuineMccluskey();

    // Member functions
    void readfile(string filename);
    void buildImplicationTable();
    pair<string,int> int2Binary(int num);
    int binary2Int(string binary);
    bool growImplicant();
    int findDiff(const string &s1, const string &s2);
    void removeNonPrimeImplicant(list<Implicant> &curList);
    vector<int> implicant2Pos(string implicant);
    int calLiteral(const string &imp);
    vector<string> coverRemainingOnset(vector<int> remainOnset);
    void columnCovering();
    void printImplicants(ofstream &output);
    void printMinimumCovering(ofstream &output);
    
    // Member variables
    int varNum;
    vector<int> on_set;
    vector<int> dc_set;
    vector<list<Implicant>> implicationTable; // ex: now we have an on-set pos 4
                                              // 4 = 0100 --> has one 1
                                              // it stores in implicationTable[1] as "0100"
    vector<string> primeImplicants;
    vector<string> essPrimeImp;
    vector<string> nonEssPrimeImp;
    
};

#endif