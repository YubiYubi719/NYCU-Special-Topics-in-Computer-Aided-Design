#ifndef STA_H
#define STA_H
#include <bits/stdc++.h>
#include <regex>
using namespace std;

class STA{
public:
    STA();
    ~STA();
    string removeComment(string code);
    void verilogParser(const string &filename);
};

#endif