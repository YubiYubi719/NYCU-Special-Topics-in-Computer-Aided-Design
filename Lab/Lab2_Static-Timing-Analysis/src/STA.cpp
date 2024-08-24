#include "STA.h"

STA::STA(): inputNum(0){
    ;
}

STA::~STA(){
    for(const pair<const string,Net*>  &p : netMap)  delete p.second;
    for(const pair<const size_t,Cell*> &p : cellMap) delete p.second;
}

string STA::removeComment(string &code){
    // Remove //... that does not contain */
    // code = regex_replace(code, Comment_Pattern_1, "");
    
    // Remove /*...*/ cross multi-line
    code = regex_replace(code, Comment_Pattern_2, "");

    // Remove //...
    code = regex_replace(code, Comment_Pattern_3, "");

    string result, curLine;
    stringstream ss(code);
    // Remove redundant space and add newline after ';'
    while(getline(ss,curLine)){
        result += curLine;
        if(curLine.back() == ';') result += "\n";
    }

    return result;
}

void STA::verilogParser(const string &netlistPath){
    // Extract netlist name from its path
    size_t pos = netlistPath.find_last_of("/\\");
    netlistName = (pos == string::npos) ? netlistPath : netlistPath.substr(pos + 1);
    netlistName.erase(netlistName.length()-2);
    
    // Read whole commented file into a string
    ifstream fin(netlistPath);
    string commentedCode, curLine;
    while(getline(fin,curLine,';')){ commentedCode += curLine + ";\n"; }
    string cleanCode = removeComment(commentedCode);
    // add space
    cleanCode = regex_replace(cleanCode,Word_Pattern," $& ");
    // cout << cleanCode << '\n';
    fin.close();
    
    // Parse 
    stringstream ss_code(cleanCode), ss;
    string opType;
    string netName;
    string cellName;
    string pin;
    while(getline(ss_code,curLine)){
        if(curLine.empty()) continue;
        ss.clear();
        ss.str(curLine);

        ss >> opType;
        // if(opType == "output" || opType == "input" || opType == "wire")
        if(opType[0] == 'o' || opType[0] == 'i' || opType[0] == 'w'){
            // Use regular expression to extract netName
            NetType netType = str2NetType(opType);
            while(ss >> netName){
                if(isalpha(netName[0])){
                    if(netType == input) inputNum++;
                    Net* net = new Net(netName,netType);
                    netMap[netName] = net;
                }
            }
        }
        // else if(opType == "INVX1" || opType == "NANDX1" || opType == "NOR2X1")
        else if(opType[1] == 'N' || opType[1] == 'A' || opType[1] == 'O'){
            // Extract cellName
            ss >> cellName;
            Cell* cell = new Cell(cellName,str2CellType(opType));

            while(ss >> curLine){
                // input net
                if(curLine == "A1"){
                    ss >> netName >> netName; // ignore '('
                    Net* &net = netMap.at(netName);
                    cell->inputNet[0] = net;
                    net->outputCell.push_back(cell);
                }
                else if(curLine == "A2"){
                    ss >> netName >> netName; // ignore '('
                    Net* &net = netMap.at(netName);
                    cell->inputNet[1] = net;
                    net->outputCell.push_back(cell);
                }
                else if(curLine == "I"){
                    ss >> netName >> netName; // ignore '('
                    Net* &net = netMap.at(netName);
                    cell->inputNet[0] = net;
                    net->outputCell.push_back(cell);
                }
                // output net
                else if(curLine == "ZN"){
                    ss >> netName >> netName; // ignore '('
                    Net* outputNet = netMap.at(netName);
                    cell->outputNet = outputNet;
                    outputNet->inputCell = cell;
                    if(outputNet->type == output) outputCell.push_back(cell);
                }
            }
            cellMap[cell->number] = cell;
        }
    }
}

void STA::libraryParser(const string &filename){
    ifstream input(filename);
    string curLine;
    CellType cellType;
    string libStr;
    smatch match;
    while(getline(input,curLine)) libStr += curLine + '\n';
    input.close();
    // Add \n after every "}"
    regex_replace(libStr,regex("}"),"}\n");

    stringstream ss(libStr);
    while(getline(ss,curLine)){
        // Extract index_1
        if(regex_search(curLine,Index_1_Pattern)){
            while(regex_search(curLine,match,Float_Pattern)){
                cellLib.index_1.push_back(stod(match.str()));
                curLine = match.suffix().str();
            }
        }

        // Extract index_2
        else if(regex_search(curLine,Index_2_Pattern)){
            while(regex_search(curLine,match,Float_Pattern)){
                cellLib.index_2.push_back(stod(match.str()));
                curLine = match.suffix().str();
            }
        }

        // Extract cell type
        else if(regex_search(curLine,match,Cell_Pattern)){
            cellType = str2CellType(match[1].str());
            CellInfo* cell_info = new CellInfo;
            if(cellType == NOR2X1 || cellType == NANDX1) cell_info->pinCap.resize(2);
            cellLib.cellInfos[cellType] = cell_info;
        }

        // Extract input pin capacitance
        else if(regex_search(curLine,match,Pin_Pattern)){
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
            while(getline(ss,curLine)){
                pinStr += curLine;
                if(curLine.back() == '}') break;
            }
            regex_search(pinStr,match,Capacitance_Pattern);
            if(pin == "A1")      cellLib.cellInfos[cellType]->pinCap[0] = stod(match[1].str());
            else if(pin == "A2") cellLib.cellInfos[cellType]->pinCap[1] = stod(match[1].str());
            else /*pin == "I"*/  cellLib.cellInfos[cellType]->pinCap.emplace_back(stod(match[1].str()));
        }

        // extract timing information
        else if(regex_search(curLine,match,Table_Pattern)){
            TableType timeType = str2TableType(match.str());
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
            while(getline(ss,curLine)){
                timeStr += curLine;
                if(curLine.back() == '}') break;
            }
            // Store timing table
            while(regex_search(timeStr,match,Float_Pattern)){
                cellLib.cellInfos[cellType]->tables[timeType].emplace_back(stod(match.str()));
                timeStr = match.suffix().str();
            }
        }
    }
}

void STA::patternParser(const string &patternPath){
    ifstream input(patternPath);
    string curLine;

    // Read input order
    patternOrder.resize(inputNum);
    input >> curLine; // ignore "input"
    string str;
    for(size_t i = 0; i < inputNum; i++){
        input >> str;
        if(str.back() == ',') str.erase(str.size()-1,1);
        patternOrder[i] = str;
    }
    input.ignore(2,'\n');

    // Read pattern
    stringstream ss;
    while(getline(input,curLine)){
        vector<char> pattern(inputNum);
        if(curLine == ".end") break;
        ss.clear();
        ss.str(curLine);
        for(size_t i = 0; i < inputNum; i++) ss >> pattern[i];
        patterns.push_back(pattern);
    }
}

/*
* @brief 
* Sort the cells by their indegree and store the result into vector<Cell*> t_sort
*/
void STA::topologicalSort(){
    queue<Cell*> q;
    for(const pair<const size_t,Cell*> &p : cellMap){
        Cell* cell = p.second;
        for(Net* &net : cell->inputNet){
            if(net->type != input) cell->inDegree++;
        }
        if(cell->inDegree == 0){
            q.push(cell);
            t_sort.push_back(cell);
        }
    }
    while(!q.empty()){
        Cell* topCell = q.front();
        q.pop();
        for(Cell* &adjCell:topCell->outputNet->outputCell){
            adjCell->inDegree--;
            if(adjCell->inDegree == 0){
                q.push(adjCell);
                t_sort.push_back(adjCell);
            }
        }
    }
    assert(t_sort.size() == cellMap.size());
}

void STA::calOutputLoad(){
    // Traverse all cell
    for(Cell* const &cell: t_sort){
        // Calculate output load of current cell
        double outputLoad = 0.0;
        if(cell->outputNet->type == output) outputLoad += OUTPUT_LOAD;
        for(Cell* outputCell:cell->outputNet->outputCell){
            for(size_t i = 0; i < outputCell->inputNet.size(); i++){
                Net* net = outputCell->inputNet[i];
                if(net == cell->outputNet){
                    outputLoad += cellLib.cellInfos[outputCell->type]->pinCap[i];
                    break;
                }
            }
        }
        cell->outputLoad = outputLoad;
    }
}

void STA::dumpOutputLoad(){
    ofstream fout("312510224_" + netlistName + "_load.txt");
    fout << fixed << setprecision(6);
    // Traverse all cell
    for(Cell* const &cell: t_sort){
        fout << cell->name << ' ' 
             << cell->outputLoad << '\n';
    }
    fout.close();
}

double STA::interpolate(
    const double &inputTransition,
    const double &outputLoad,
    const vector<double> &table, 
    const double &col_idx, 
    const double &row_idx
){
    size_t colSize = cellLib.index_1.size();
    double col_1 = cellLib.index_1[col_idx-1];
    double col_2 = cellLib.index_1[col_idx];
    double row_1 = cellLib.index_2[row_idx-1];
    double row_2 = cellLib.index_2[row_idx];

    double col_1_row_1_val = table[(col_idx-1) + (row_idx-1) * colSize];
    double col_2_row_1_val = table[(col_idx)   + (row_idx-1) * colSize];
    double col_1_row_2_val = table[(col_idx-1) + (row_idx)   * colSize];
    double col_2_row_2_val = table[(col_idx)   + (row_idx)   * colSize];

    double A = col_1_row_1_val + (col_2_row_1_val-col_1_row_1_val) / (col_2-col_1) * (outputLoad-col_1);
    double B = col_1_row_2_val + (col_2_row_2_val-col_1_row_2_val) / (col_2-col_1) * (outputLoad-col_1);

    return A + (B-A) / (row_2-row_1) * (inputTransition-row_1);
}

double STA::tableLookUp(const Cell* const &cell, const TableType &tableType){
    size_t index_1_idx, index_2_idx;
    //If larger, use binary search
    size_t colSize = cellLib.index_1.size();
    size_t rowSize = cellLib.index_2.size();
    for(index_1_idx = 0; index_1_idx < colSize; index_1_idx++){
        if(cellLib.index_1[index_1_idx] > cell->outputLoad) break;
    }
    if(index_1_idx == 0) index_1_idx = 1;
    else if(index_1_idx == colSize) index_1_idx = colSize - 1;

    for(index_2_idx = 0; index_2_idx < rowSize; index_2_idx++){
        if(cellLib.index_2[index_2_idx] > cell->inputTransition) break;
    }
    if(index_2_idx == 0) index_2_idx = 1;
    else if(index_2_idx == rowSize) index_2_idx = rowSize - 1;

    const vector<double> &table = cellLib.cellInfos[cell->type]->tables[tableType];
    
    return interpolate(cell->inputTransition,cell->outputLoad,table,index_1_idx,index_2_idx);
}

void STA::calInputTransitionTime(Cell* const &cell){
    assert(cell->inputNet.size() == 1 || cell->inputNet.size() == 2);
    // INVX1
    if(cell->type == INVX1){
        Cell* const &prevCell = cell->inputNet[0]->inputCell;
        if(cell->inputNet[0]->type == input){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else /* cell->inputNet[0]->type == wire or output */ {
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
            cell->prevCell = prevCell;
        }
        return;
    }

    // NANDX1, NOR2X1
    if(cell->inputNet[0]->type == input && cell->inputNet[1]->type == input){
        cell->inputTransition = 0.0;
        cell->arrivalTime = 0.0;
    }
    else if(cell->inputNet[0]->type != input && cell->inputNet[1]->type == input){
        Cell* const &prevCell = cell->inputNet[0]->inputCell;
        cell->inputTransition = prevCell->outputTransition;
        cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        cell->prevCell = prevCell;
    }
    else if(cell->inputNet[0]->type == input && cell->inputNet[1]->type != input){
        Cell* const &prevCell = cell->inputNet[1]->inputCell;
        cell->inputTransition = prevCell->outputTransition;
        cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        cell->prevCell = prevCell;
    }
    else /* cell->inputNet[0]->type != input && cell->inputNet[1]->type != input */ {
        Cell* const &prevCell_0 = cell->inputNet[0]->inputCell;
        Cell* const &prevCell_1 = cell->inputNet[1]->inputCell;
        // Choose the slower signal
        double arrival_0 = prevCell_0->arrivalTime + prevCell_0->delay + WIRE_DELAY;
        double arrival_1 = prevCell_1->arrivalTime + prevCell_1->delay + WIRE_DELAY;
        if(arrival_0 > arrival_1){
            cell->inputTransition = prevCell_0->outputTransition;
            cell->arrivalTime = arrival_0;
            cell->prevCell = prevCell_0;
        }
        else /* arrival_1 > arrival_0 */ {
            cell->inputTransition = prevCell_1->outputTransition;
            cell->arrivalTime = arrival_1;
            cell->prevCell = prevCell_1;
        }
    }
}

void STA::calPropagationDelay(){
    // Traverse netlist in topological order
    for(Cell* const &cell : t_sort){
        calInputTransitionTime(cell);
        // Calculate intrinsic delay and rise/fall transition time by look-up table
        double riseDelay = tableLookUp(cell,cell_rise);
        double fallDelay = tableLookUp(cell,cell_fall);
        if(riseDelay > fallDelay){
            cell->delay = riseDelay;
            cell->outputTransition = tableLookUp(cell,rise_transition);
            cell->worstCaseValue = HIGH;
        }
        else /* riseDelay < fallDelay */ {
            cell->delay = fallDelay;
            cell->outputTransition = tableLookUp(cell,fall_transition);
            cell->worstCaseValue = LOW;
        }
    }
}

void STA::dumpPropagationDelay(){
    ofstream fout("312510224_" + netlistName + "_delay.txt");
    fout << fixed << setprecision(6);
    for(Cell* const &cell : t_sort){
        fout << cell->name << " " 
             << cell->worstCaseValue << " " 
             << cell->delay << " " 
             << cell->outputTransition << '\n';
    }
    fout.close();
}

vector<Net*> STA::findPath(Cell* cell){
    vector<Net*> path;
    while(cell != nullptr){
        path.push_back(cell->outputNet);
        if(cell->prevCell == nullptr) path.push_back(cell->inputNet[0]);
        cell = cell->prevCell;
    }
    return path;
}

void STA::pathFinding(){
    maxDelay = DBL_MIN;
    minDelay = DBL_MAX;
    Cell *longestPathCell = nullptr, *shortestPathCell = nullptr;
    for(Cell* const &cell : outputCell){
        double outputDelay = cell->arrivalTime + cell->delay;
        if(outputDelay > maxDelay){
            maxDelay = outputDelay;
            longestPathCell = cell;
        }
        if(outputDelay < minDelay){
            minDelay = outputDelay;
            shortestPathCell = cell;
        }
    }
    assert(longestPathCell != nullptr && shortestPathCell != nullptr);
    longestPath  = findPath(longestPathCell);
    shortestPath = findPath(shortestPathCell);
}

void STA::dumpPath(){
    ofstream output("312510224_" + netlistName + "_path.txt");
    output << fixed << setprecision(6);
    output << "Longest delay = " 
           << maxDelay << ", the path is: ";
    for(auto iter = longestPath.rbegin(); iter != longestPath.rend(); iter++){
        output << (*iter)->name << " ";
        if(next(iter) != longestPath.rend()) output << "-> ";
    }
    output << '\n';
    output << "Shortest delay = " 
           << minDelay << ", the path is: ";
    for(auto iter = shortestPath.rbegin(); iter != shortestPath.rend(); iter++){
        output << (*iter)->name << " ";
        if(next(iter) != shortestPath.rend()) output << "-> ";
    }
    output.close();
}

void STA::calInputTransitionTime_Simulate(Cell* const &cell){
    assert(cell->inputNet.size() == 1 || cell->inputNet.size() == 2);
    // INVX1
    if(cell->type == INVX1){
        if(cell->inputNet[0]->type == input){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else /*cell->inputNet[0]->type == wire */ {
            Cell* const &prevCell = cell->inputNet[0]->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        cell->value = (cell->inputNet[0]->value == LOW)? HIGH : LOW;
        return;
    }

    // NANDX1, NOR2X1
    const Net* const &A1 = cell->inputNet[0];
    const Net* const &A2 = cell->inputNet[1];
    if(A1->type == input && A2->type == input){
        cell->inputTransition = 0.0;
        cell->arrivalTime = 0.0;
    }
    else if(A1->type != input && A2->type == input){
        if(A2->value == cell->controllingValue){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else{
            const Cell* const &prevCell = A1->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
    }
    else if(A1->type == input && A2->type != input){
        if(A1->value == cell->controllingValue){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else{
            const Cell* const &prevCell = A2->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
    }
    else /* A1->type != input && A2->type != input */ {
        if(A1->value == cell->controllingValue && A2->value == cell->controllingValue){
            const Cell* const &prevCell_0 = A1->inputCell;
            const Cell* const &prevCell_1 = A2->inputCell;
            // Choose the smaller arrival time input
            const double arrival_0 = prevCell_0->arrivalTime + prevCell_0->delay + WIRE_DELAY;
            const double arrival_1 = prevCell_1->arrivalTime + prevCell_1->delay + WIRE_DELAY;
            if(arrival_0 < arrival_1){
                cell->inputTransition = prevCell_0->outputTransition;
                cell->arrivalTime = arrival_0;
            }
            else /* arrival_1 < arrival_0 */ {
                cell->inputTransition = prevCell_1->outputTransition;
                cell->arrivalTime = arrival_1;
            }
        }
        else if(A1->value == cell->controllingValue){
            const Cell* const &prevCell = A1->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else if(A2->value == cell->controllingValue){
            const Cell* const &prevCell = A2->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else /* A1->value != cell->controllingValue && A2->value != cell->controllingValue */ {
            const Cell* const &prevCell_0 = A1->inputCell;
            const Cell* const &prevCell_1 = A2->inputCell;
            // Choose the bigger arrival time input
            const double arrival_0 = prevCell_0->arrivalTime + prevCell_0->delay + WIRE_DELAY;
            const double arrival_1 = prevCell_1->arrivalTime + prevCell_1->delay + WIRE_DELAY;
            if(arrival_0 > arrival_1){
                cell->inputTransition = prevCell_0->outputTransition;
                cell->arrivalTime = arrival_0;
            }
            else /*arrival_1 > arrival_0*/ {
                cell->inputTransition = prevCell_1->outputTransition;
                cell->arrivalTime = arrival_1;
            }
        }
    }
    cell->value = truthTable[cell->type]({A1->value,A2->value});
}

void STA::simulate(const vector<char> &pattern){
    size_t patternSize = pattern.size();
    for(size_t i = 0; i < patternSize; i++){
        netMap.at(patternOrder[i])->value = pattern[i];
    }
    // Traverse netlist in topological order
    for(Cell* const &cell : t_sort){
        // Set input transition time
        calInputTransitionTime_Simulate(cell);
        // Calculate intrinsic delay and rise/fall transition time by look-up table
        if(cell->value == HIGH){
            cell->delay = tableLookUp(cell,cell_rise);
            cell->outputTransition = tableLookUp(cell,rise_transition);
            cell->outputNet->value = HIGH;
        }
        else /* cell->value == LOW */ {
            cell->delay = tableLookUp(cell,cell_fall);
            cell->outputTransition = tableLookUp(cell,fall_transition);
            cell->outputNet->value = LOW;
        }
    }
}

void STA::assignPattern(){
    vector<Cell*> cellsInGateOrder(t_sort);
    string result;
    result.reserve(patterns.size()*t_sort.size()*30);
    ostringstream oss;
    oss << fixed << setprecision(6);
    sort(cellsInGateOrder.begin(), cellsInGateOrder.end(), Cell::cmpWithGateOrder);
    for(vector<char> pattern : patterns){
        simulate(pattern);
        dumpGateInfo(oss,cellsInGateOrder);
    }

    ofstream output("312510224_" + netlistName + "_gate_info.txt");
    output << oss.str();

    output.close();
}

void STA::dumpGateInfo(ostringstream &output, const vector<Cell*> &cells){
    for(const Cell* const &cell : cells){
        output << cell->name  << " " 
               << cell->value << " "
               << cell->delay << " "
               << cell->outputTransition << '\n';
    }
    output << '\n';
}