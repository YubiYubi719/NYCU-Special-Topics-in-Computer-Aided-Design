#include "STA.h"

STA::STA(){
    ;
}

STA::~STA(){
    ;
}

string STA::removeComment(string code){
    // Remove //... that does not contain */
    code = regex_replace(code,regex("//(?!.*\\*/).*\n"), "");
    
    // Remove /*...*/ cross multi-line
    code = regex_replace(code, regex("/\\*[\\s\\S]*?\\*/"), "");

    // Remove //...
    code = regex_replace(code, regex("//.*\n"), "");

    string result, curLine;
    stringstream ss(code);
    while(ss >> curLine){
        // Remove redundant space and add newline after ';'
        result += (curLine.back() == ';')? curLine+"\n" : curLine+" ";
    }

    return result;
}

void STA::verilogParser(const string &filename){
    ifstream input(filename);
    // Read whole commented file into a string
    string commentedCode, curLine;
    while(getline(input,curLine)){ commentedCode += curLine + "\n"; }
    string cleanCode = removeComment(commentedCode);
    cout << cleanCode << '\n';
    // Parse 
    stringstream ss_code(cleanCode), ss;
    string str;
    while(getline(ss_code,curLine)){
        ss.clear();
        ss.str(curLine);
        ss >> str;
        OP_Type op = OP_map.at(str);
        switch(op){
            case OP_Type::Net:{
                string netType = str;
                string netName;
                while(ss >> netName){
                    if(netName == "," || netName == ";") continue;
                    if(netName.back() == ',' || netName.back() == ';') netName.erase(netName.length()-1,1);
                    Net* net = new Net(netName,netType);
                    netMap[netName] = net;
                }
                break;
            }
            case OP_Type::Gate:{
                string gateType = str;
                string gateName;
                //todo: use regular expression to extract gateName and input, output net of the gate
                // ...
                break;
            }
            default: break;
        }
    }
}