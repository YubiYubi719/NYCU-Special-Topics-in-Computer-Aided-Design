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
        // Use regular expression to extract type of the curLine
        regex_search(curLine,match,regex("(output)|(input)|(wire)|(INVX1)|(NANDX1)|(NOR2X1)|(module)|(endmodule)"));
        opType = match.str();
        curLine = match.suffix().str();
        switch(OP_map.at(opType)){
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

                Gate* gate = new Gate(gateName,opType,outputNet);
                // Extract input of the gate
                if(opType == "NANDX1"|| opType == "NOR2X1"){
                    while(regex_search(curLine,match,regex("\\.(A\\d*)\\((.*?)\\)"))){
                        string pin = match[1].str();
                        if(pin == "A1") gate->A1 = netMap[match[2].str()];
                        if(pin == "A2") gate->A2 = netMap[match[2].str()];
                        curLine = match.suffix().str();
                    }
                }
                else{ // opType == "INVX1"
                    regex_search(curLine,match,regex("\\.I\\d*\\((.*?)\\)"));
                    gate->I = netMap[match[1].str()];
                }
                gateMap[gateName] = gate;
                break;
            }
            default: break;
        }
    }
}

void STA::libraryParser(const string &filename){

}