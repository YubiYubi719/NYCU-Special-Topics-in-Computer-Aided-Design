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
            case OP_Type::input:{
                cout << 1 << '\n';
                break;
            }
            case OP_Type::output:{
                cout << 2 << '\n';
                break;
            }
            case OP_Type::wire:{
                cout << 3 << '\n';

                break;
            }
            case OP_Type::NANDX1:{
                cout << 4 << '\n';

                break;
            }
            case OP_Type::NOR2X1:{
                cout << 5 << '\n';

                break;
            }
            case OP_Type::INVX1:{
                cout << 6 << '\n';

                break;
            }
            default: break;
        }
    }
}