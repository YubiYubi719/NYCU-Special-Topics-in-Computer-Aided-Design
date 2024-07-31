#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]){
    ifstream input(argv[1]); // input tesecase
    ofstream output("onsetCoverChecker.cpp");
    string str;
    int variableNum;
    input >> str >> variableNum;

    input >> str;
    input.ignore(2,'\n');
    getline(input,str);
    stringstream ss(str);
    vector<int> on_set;
    int pos;
    while(ss >> pos){
        on_set.push_back(pos);
    }

    input.close();

    output << "#include <iostream>\n#include <bitset>\n#include <vector>\n#include <functional>\n";
    output << "\nconst int numVariables = " << variableNum << ";\n";
    output << "std::bitset<numVariables> toBitset(int value) {\n";
    output << "return std::bitset<numVariables>(value);\n";
    output << "}\n\n";

    output << "bool booleanFunction(const std::bitset<numVariables>& inputs) {\n";
    for(int i = 0; i < variableNum; i++){
        output << "    bool " << (char)('A'+i) << " = inputs[" << variableNum-1-i << "];\n";
    }

    output << "    return ";
    ifstream input1("changeType.txt");
    getline(input1,str);
    input1.close();
    output << str << ";\n";
    output << "}\n";

    output << "bool coversOnSet(const std::vector<int>& grayOnSet) {\n";
    output << "    for (int gray : grayOnSet) {\n";
    output << "        std::bitset<numVariables> inputs = toBitset(gray);\n";
    output << "        if (!booleanFunction(inputs)) {\n";
    output << "            return false;\n";
    output << "        }\n";
    output << "    }\n";
    output << "    return true;\n";
    output << "}\n\n";

    output << "int main() {\n";
    output << "    std::vector<int> grayOnSet = {";
    for(int i = 0; i < on_set.size(); i++){
        output << on_set[i];
        if(i != on_set.size()-1) output << ", ";
    }
    output << "};\n";
    output << "    if (coversOnSet(grayOnSet)) {\n";
    output << "        std::cout << \"The Boolean function covers the specified on-set.\" << std::endl;\n";
    output << "    } else {\n";
    output << "        std::cout << \"The Boolean function does not cover the specified on-set.\" << std::endl;\n";
    output << "    }\n\n";
    output << "    return 0;\n";
    output << "}";


    output.close();
    return 0;
}