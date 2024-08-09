#ifndef STA_H
#define STA_H
#include <bits/stdc++.h>
#include <regex>
#include "Table.h"
#include "Net.h"
#include "Gate.h"
using namespace std;

class STA{
public:
    STA();
    ~STA();
    string removeComment(string code);
    void verilogParser(const string &filename);
};

#endif