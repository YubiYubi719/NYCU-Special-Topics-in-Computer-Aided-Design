#include <bits/stdc++.h>
using namespace std;

class QM{
public:
    QM();
    ~QM();

    void build(string filename); // read file

    int varNum;
    vector<int> on_set;
    vector<int> dc_set;
};