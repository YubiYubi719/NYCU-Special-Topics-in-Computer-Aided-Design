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
    stringstream ss_code(cleanCode);
    string opType;
    smatch match;
    while(getline(ss_code,curLine)){
        // Extract type of the curLine
        regex_search(curLine,match,regex("(output)|(input)|(wire)|(INVX1)|(NANDX1)|(NOR2X1)|(module)|(endmodule)"));
        opType = match.str();
        OP_Type op = OP_map.at(opType);
        curLine = match.suffix().str();
        switch(op){
            case OP_Type::Net:{
                string netName;
                // Use regular expression to extract netName
                while(regex_search(curLine,match,regex("\\w+"))){
                    netName = match.str();
                    curLine = match.suffix().str();
                    Net* net = new Net(netName,opType);
                    netMap[netName] = net;
                }
                break;
            }
            case OP_Type::Gate:{
                // Use regular expression to extract gateName
                regex_search(curLine,match,regex("\\w+"));
                string gateName = match.str();
                curLine = match.suffix().str();

                // Extract output net of the gate
                regex_search(curLine,match,regex("\\.ZN\\((.*?)\\)"));
                Net* outputNet = netMap[match[1].str()];

                // Extract input of the gate
                vector<Net*> inputNet;
                while(regex_search(curLine,match,regex("\\.[AI]\\d*\\((.*?)\\)"))){
                    inputNet.push_back(netMap[match[1].str()]);
                    curLine = match.suffix().str();
                }
                Gate* gate = new Gate(gateName,opType,inputNet,outputNet);
                gateMap[gateName] = gate;
                break;
            }
            default: break;
        }
    }
}

void STA::libraryParser(const string &filename){
    
}