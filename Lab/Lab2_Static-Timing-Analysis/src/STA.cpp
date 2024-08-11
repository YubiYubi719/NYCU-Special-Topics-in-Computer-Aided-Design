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
    // cout << cleanCode << '\n';
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
            case OP_Type::Cell:{
                // Extract gateName
                regex_search(curLine,match,regex("\\w+"));
                string cellName = match.str();
                curLine = match.suffix().str();

                // Extract output net
                regex_search(curLine,match,regex("ZN\\((.*?)\\)"));
                Net* outputNet = netMap.at(match[1].str());

                Cell* cell = new Cell(cellName,opType,outputNet);
                // Extract input net
                if(opType == "NANDX1"|| opType == "NOR2X1"){
                    cell->inputNet.resize(2);
                    while(regex_search(curLine,match,regex("(A\\d+)\\((.*?)\\)"))){
                        string pin = match[1].str();
                        if(pin == "A1") cell->inputNet[0] = netMap.at(match[2].str());
                        else cell->inputNet[1] = netMap.at(match[2].str());
                        netMap.at(match[2].str())->outputCell.push_back(cell);
                        curLine = match.suffix().str();
                    }
                }
                else{ // opType == "INVX1"
                    regex_search(curLine,match,regex("I\\((.*?)\\)"));
                    cell->inputNet.push_back(netMap.at(match[1].str()));
                    netMap.at(match[1].str())->outputCell.push_back(cell);
                }
                cellMap[cellName] = cell;
                break;
            }
            default: break;
        }
    }
    input.close();
}

void STA::libraryParser(const string &filename){
    ifstream input(filename);
    string curLine;
    string cellType;
    smatch match;
    while(getline(input,curLine)){
        // Extract index_1
        if(regex_search(curLine,regex("index_1"))){
            while(regex_search(curLine,match,regex("\\d+\\.\\d+"))){
                cellLib.index_1.push_back(stod(match.str()));
                curLine = match.suffix().str();
            }
        }

        // Extract index_2
        else if(regex_search(curLine,regex("index_2"))){
            while(regex_search(curLine,match,regex("\\d+\\.\\d+"))){
                cellLib.index_2.push_back(stod(match.str()));
                curLine = match.suffix().str();
            }
        }

        // Extract cell type
        else if(regex_search(curLine,match,regex("cell *\\((\\w+)\\)"))){
            cellType = match[1].str();
            CellInfo* cell_info = new CellInfo;
            if(cellType == "NOR2X1" || cellType == "NANDX1") cell_info->pinCap.resize(2);
            cellLib.cellMap[cellType] = cell_info;
        }

        // Extract input pin capacitance
        else if(regex_search(curLine,match,regex("pin\\(([AI]\\d*)\\)"))){
            string pin = match[1].str();
            // store whole pin-related information into one string
            /*
            Ex:
            pin(A1) {
                direction : input;
                capacitance : 0.0105008;
            }
            --> pin(A1) { direction : input; capacitance : 0.0105008; }
            */
            string pinStr = curLine;
            while(getline(input,curLine)){
                pinStr += curLine;
                if(regex_search(curLine,regex("}"))) break;
            }
            regex_search(pinStr,match,regex("capacitance *: *(\\d+\\.\\d+);"));
            if(pin == "A1")      cellLib.cellMap[cellType]->pinCap[0] = stod(match[1].str());
            else if(pin == "A2") cellLib.cellMap[cellType]->pinCap[1] = stod(match[1].str());
            else /*pin == "I"*/  cellLib.cellMap[cellType]->pinCap.push_back(stod(match[1].str()));
        }

        // extract timing information
        else if(regex_search(curLine,match,regex("(cell_rise)|(cell_fall)|(rise_transition)|(fall_transition)"))){
            string timeType = match.str();
            // store whole time-related information into one string
            /*
            Ex:
            cell_rise(table10){
            values ("0.023104,0.026347,0.032439,0.037211,0.050187,0.075629,0.10893",\
                    "0.027062,0.029858,0.036397,0.042338,0.055823,0.083858,0.11942",\
                    "0.034333,0.037002,0.043649,0.050318,0.064815,0.089926,0.13827",\
                    "0.048838,0.051443,0.053267,0.064115,0.083493,0.11205,0.17069",\
                    "0.076172,0.077668,0.081485,0.08978,0.11263,0.14904,0.20359",\
                    "0.13351,0.13486,0.13847,0.14635,0.16278,0.20974,0.27949",\
                    "0.24777,0.24888,0.25234,0.25971,0.27596,0.30968,0.40393");
                    }
            --> cell_rise(table10){ values ("0.023104,0.026347,..., 0.30968,0.40393"); }
            */
            string timeStr = curLine;
            while(getline(input,curLine)){
                timeStr += curLine;
                if(regex_search(curLine,regex("}"))) break;
            }
            // Store timing table
            while(regex_search(timeStr,match,regex("\\d+\\.\\d+"))){
                cellLib.cellMap[cellType]->tables[timeType].push_back(stod(match.str()));
                timeStr = match.suffix().str();
            }
        }
    }
    input.close();
}

void STA::calOutputLoad(){
    // Traverse all cell 
    for(pair<string,Cell*> p : cellMap){
        // Calculate output load of current cell
        Cell* cell = p.second;
        double outputLoad = 0.0;
        if(cell->outputNet->outputCell.empty()){
            cell->outputLoad = 0.03;
            continue;
        }
        for(Cell* outputCell:cell->outputNet->outputCell){
            for(size_t i = 0; i < outputCell->inputNet.size(); i++){
                Net* net = outputCell->inputNet[i];
                if(net == cell->outputNet){
                    outputLoad += cellLib.cellMap.at(outputCell->type)->pinCap[i];
                    break;
                }
            }
        }
        cell->outputLoad = outputLoad;
    }
}

void STA::dumpOutputLoad(string case_name){
    // ofstream output("312510224_" + case_name + "_load.txt");
    cout << "Step 1:" << '\n';
    for(pair<string,Cell*> p : cellMap){
        Cell* cell = p.second;
        cout << cell->name << ' ' 
             << fixed << setprecision(6) 
             << cell->outputLoad << '\n';
    }
    // output.close();
}