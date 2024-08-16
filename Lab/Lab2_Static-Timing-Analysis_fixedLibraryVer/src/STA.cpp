#include "STA.h"

STA::STA(): inputNum(0){
    ;
}

STA::~STA(){
    ;
}

string STA::removeComment(string code){
    // Remove //... that does not contain */
    code = regex_replace(code, Comment_Pattern_1, "");
    
    // Remove /*...*/ cross multi-line
    code = regex_replace(code, Comment_Pattern_2, "");

    // Remove //...
    code = regex_replace(code, Comment_Pattern_3, "");

    string result, curLine;
    stringstream ss(code);
    while(ss >> curLine){
        // Remove redundant space and add newline after ';'
        result += (curLine.back() == ';')? curLine+"\n" : curLine+" ";
    }

    return result;
}

void STA::verilogParser(const string &netlistPath){
    // Extract netlist name from its path
    size_t pos = netlistPath.find_last_of("/\\");
    netlistName = (pos == std::string::npos) ? netlistPath : netlistPath.substr(pos + 1);
    
    // Read whole commented file into a string
    ifstream input(netlistPath);
    string commentedCode, curLine;
    while(getline(input,curLine)){ commentedCode += curLine + "\n"; }
    string cleanCode = removeComment(commentedCode);
    // cout << cleanCode << '\n';
    // Parse 
    stringstream ss_code(cleanCode);
    string opType;
    smatch match;
    string netName;
    string cellName;
    while(getline(ss_code,curLine)){
        // Use regular expression to extract current type
        regex_search(curLine,match,Type_Pattern);
        opType = match.str();
        curLine = match.suffix().str();
        if(opType == "output" || opType == "input" || opType == "wire"){
            // Use regular expression to extract netName
            while(regex_search(curLine,match,Word_Pattern)){
                if(opType == "input") inputNum++;
                netName = match.str();
                curLine = match.suffix().str();
                Net* net = new Net(netName,opType);
                netMap[netName] = net;
            }
        }
        else if(opType == "INVX1" || opType == "NANDX1" || opType == "NOR2X1"){
            // Extract cellName
            regex_search(curLine,match,Word_Pattern);
            cellName = match.str();
            curLine = match.suffix().str();
            Cell* cell = new Cell(cellName,opType);

            // Extract output net
            regex_search(curLine,match,OutputNet_Pattern);
            Net* outputNet = netMap.at(match[1].str());
            cell->outputNet = outputNet;
            outputNet->inputCell = cell;
            if(outputNet->type == "output") outputCell.push_back(cell);

            // Extract input net
            if(opType == "INVX1"){
                regex_search(curLine,match,INVX1_InputNet_Pattern);
                cell->inputNet.push_back(netMap.at(match[1].str()));
                netMap.at(match[1].str())->outputCell.push_back(cell);
            }
            else /* opType == "NANDX1" || opType == "NOR2X1" */ {
                cell->inputNet.resize(2);
                regex_search(curLine,match,NANDX1_NOR2X1_InputNet_Pattern);

                string pin_1 = match[1].str();
                if(pin_1 == "A1"){
                    cell->inputNet[0] = netMap.at(match[2].str());
                }
                else /* pin_1 == "A2" */ {
                    cell->inputNet[1] = netMap.at(match[2].str());
                }
                netMap.at(match[2].str())->outputCell.push_back(cell);

                string pin_2 = match[3].str();
                if(pin_2 == "A1"){
                    cell->inputNet[0] = netMap.at(match[4].str());
                }
                else /* pin_2 == "A2" */ {
                    cell->inputNet[1] = netMap.at(match[4].str());
                }
                netMap.at(match[4].str())->outputCell.push_back(cell);
            }
            cellMap[cellName] = cell;
        }
    }
    input.close();
}

void STA::patternParser(const string &patternPath){
    ifstream input(patternPath);
    string curLine;

    // Read input order
    patternOrder.resize(inputNum);
    input >> curLine; // ignore "input"
    for(size_t i = 0; i < inputNum; i++){
        string str;
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

void STA::calOutputLoad(){
    // Traverse all cell 
    for(pair<string,Cell*> p : cellMap){
        // Calculate output load of current cell
        Cell* cell = p.second;
        double outputLoad = 0.0;
        if(cell->outputNet->type == "output") outputLoad += OUTPUT_LOAD;
        for(Cell* outputCell:cell->outputNet->outputCell){
            for(size_t i = 0; i < outputCell->inputNet.size(); i++){
                Net* net = outputCell->inputNet[i];
                if(net == cell->outputNet){
                    if(outputCell->type == "INVX1"){
                        outputLoad += cellLib.INVX1_inf.pinCap[i];
                    }
                    else if(outputCell->type == "NANDX1"){
                        outputLoad += cellLib.NANDX1_inf.pinCap[i];
                    }
                    else /* outputCell->type == "NOR2X1" */ {
                        outputLoad += cellLib.NOR2X1_inf.pinCap[i];
                    }
                    break;
                }
            }
        }
        cell->outputLoad = outputLoad;
    }
}

void STA::dumpOutputLoad(){
    ofstream output("312510224_" + netlistName + "_load.txt");
    vector<pair<string,Cell*>> cells(cellMap.begin(),cellMap.end());
    sort(cells.begin(),cells.end(), [](const pair<string,Cell*> &p1, const pair<string,Cell*> &p2){
        if(p1.second->outputLoad == p2.second->outputLoad){
            return stoi(p1.second->name.substr(1)) < stoi(p2.second->name.substr(1));
        }
        return p1.second->outputLoad > p2.second->outputLoad;
    });
    for(const pair<string,Cell*> &p : cells){
        Cell* cell = p.second;
        output << cell->name << ' ' 
               << fixed << setprecision(6) 
               << cell->outputLoad << '\n';
        if(cell->name == "G3766") cout << cell->outputLoad << '\n';
    }
    output.close();
}

/*
* @brief 
* Sort the cells by their indegree and store the result into vector<Cell*> t_sort
*/
void STA::topologicalSort(){
    queue<Cell*> q;
    for(const pair<string,Cell*> p : cellMap){
        Cell* cell = p.second;
        for(Net* &net : cell->inputNet){
            if(net->type != "input") cell->inDegree++;
        }
        if(cell->inDegree == 0){
            q.push(cell);
            t_sort.push_back(cell);
        }
        // cout << cell->name << " indegree: " << cell->inDegree << '\n';
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

double STA::tableLookUp(Cell* cell, string tableType){
    size_t index_1_idx, index_2_idx;
    //If larger, use binary search
    size_t colSize = cellLib.index_1.size();
    size_t rowSize = cellLib.index_2.size();
    for(index_1_idx = 0; index_1_idx < colSize; index_1_idx++){
        if(cellLib.index_1[index_1_idx] > cell->outputLoad) break;
    }
    for(index_2_idx = 0; index_2_idx < rowSize; index_2_idx++){
        if(cellLib.index_2[index_2_idx] > cell->inputTransition) break;
    }

    vector<double> table;
    
    if(cell->type == "INVX1"){
        table = cellLib.INVX1_inf.tables.at(tableType);
    }
    else if(cell->type == "NANDX1"){
        table = cellLib.NANDX1_inf.tables.at(tableType);
    }
    else /* cell->type == "NOR2X1" */ {
        table = cellLib.NOR2X1_inf.tables.at(tableType);
    }

    if(index_1_idx == 0) index_1_idx = 1;
    else if(index_1_idx == colSize) index_1_idx = colSize - 1;

    if(index_2_idx == 0) index_2_idx = 1;
    else if(index_2_idx == rowSize) index_2_idx = rowSize - 1;

    return interpolate(cell->inputTransition,cell->outputLoad,table,index_1_idx,index_2_idx);
}

void STA::calInputTransitionTime(Cell* cell){
    assert(cell->inputNet.size() == 1 || cell->inputNet.size() == 2);
    // INVX1
    if(cell->inputNet.size() == 1){
        Cell* const &prevCell = cell->inputNet[0]->inputCell;
        if(cell->inputNet[0]->type == "input"){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else /* cell->inputNet[0]->type == "wire" or "output" */ {
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
            cell->prevCell = prevCell;
        }
        return;
    }

    // NANDX1, NOR2X1
    if(cell->inputNet[0]->type == "input" && cell->inputNet[1]->type == "input"){
        cell->inputTransition = 0.0;
        cell->arrivalTime = 0.0;
    }
    else if(cell->inputNet[0]->type != "input" && cell->inputNet[1]->type == "input"){
        Cell* const &prevCell = cell->inputNet[0]->inputCell;
        cell->inputTransition = prevCell->outputTransition;
        cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        cell->prevCell = prevCell;
    }
    else if(cell->inputNet[0]->type == "input" && cell->inputNet[1]->type != "input"){
        Cell* const &prevCell = cell->inputNet[1]->inputCell;
        cell->inputTransition = prevCell->outputTransition;
        cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        cell->prevCell = prevCell;
    }
    else /* cell->inputNet[0]->type != "input" && cell->inputNet[1]->type != "input" */ {
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
    topologicalSort();
    // Traverse netlist in topological order
    for(Cell* &cell : t_sort){
        calInputTransitionTime(cell);
        // Calculate intrinsic delay and rise/fall transition time by look-up table
        double riseDelay = tableLookUp(cell,"cell_rise");
        double fallDelay = tableLookUp(cell,"cell_fall");
        if(riseDelay > fallDelay){
            cell->delay = riseDelay;
            cell->outputTransition = tableLookUp(cell,"rise_transition");
            cell->worstCaseValue = '1';
        }
        else /* riseDelay < fallDelay */ {
            cell->delay = fallDelay;
            cell->outputTransition = tableLookUp(cell,"fall_transition");
            cell->worstCaseValue = '0';
        }
    }
}

void STA::dumpDelay(){
    ofstream output("312510224_" + netlistName + "_delay.txt");
    vector<Cell*> cells(t_sort.begin(),t_sort.end());
    sort(cells.begin(),cells.end(),[](Cell* c1, Cell* c2){
        if(c1->delay == c2->delay){
            return stoi(c1->name.substr(1)) < stoi(c2->name.substr(1));
        }
        return c1->delay > c2->delay;
    });
    for(Cell* &cell : cells){
        output << cell->name << " " 
               << cell->worstCaseValue << " " 
               << fixed << setprecision(6)
               << cell->delay << " " 
               << cell->outputTransition << '\n';
    }
    output.close();
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
    longestPath = findPath(longestPathCell);
    shortestPath = findPath(shortestPathCell);
}

void STA::dumpPath(){
    ofstream output("312510224_" + netlistName + "_path.txt");
    output << "Longest delay = " 
         << fixed << setprecision(6)
         << maxDelay << ", the path is: ";
    for(auto iter = longestPath.rbegin(); iter != longestPath.rend(); iter++){
        output << (*iter)->name << " ";
        if(next(iter) != longestPath.rend()) output << "-> ";
    }
    output << '\n';
    output << "Shortest delay = " 
         << fixed << setprecision(6)
         << minDelay << ", the path is: ";
    for(auto iter = shortestPath.rbegin(); iter != shortestPath.rend(); iter++){
        output << (*iter)->name << " ";
        if(next(iter) != shortestPath.rend()) output << "-> ";
    }
    output.close();
}

void STA::calInputTransitionTime_Synthesis(Cell* cell){
    assert(cell->inputNet.size() == 1 || cell->inputNet.size() == 2);
    // INVX1
    if(cell->inputNet.size() == 1){
        if(cell->inputNet[0]->type == "input"){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else /*cell->inputNet[0]->type == "wire" */ {
            Cell* const &prevCell = cell->inputNet[0]->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
            cell->prevCell = prevCell;
        }
        cell->value = (cell->inputNet[0]->value == '0')? '1' : '0';
        return;
    }

    // NANDX1, NOR2X1
    Net* A1 = cell->inputNet[0];
    Net* A2 = cell->inputNet[1];
    if(A1->type == "input" && cell->inputNet[1]->type == "input"){
        cell->inputTransition = 0.0;
        cell->arrivalTime = 0.0;
    }
    else if(A1->type != "input" && A2->type == "input"){
        if(A2->value == cell->controlledValue){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else if(A1->value == cell->controlledValue){
            Cell* const &prevCell = A1->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else /* A1->value != cell->controlledValue && A2->value != cell->controlledValue */ {
            Cell* const &prevCell = A1->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
    }
    else if(A1->type == "input" && A2->type != "input"){
        if(A1->value == cell->controlledValue){
            cell->inputTransition = 0.0;
            cell->arrivalTime = 0.0;
        }
        else if(A2->value == cell->controlledValue) {
            Cell* const &prevCell = A2->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else /* A1->value != cell->controlledValue && A2->value != cell->controlledValue */ {
            Cell* const &prevCell = A2->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
    }
    else /* A1->type != "input" && A2->type != "input" */ {
        if(A1->value == cell->controlledValue && A2->value == cell->controlledValue){
            Cell* const &prevCell_0 = A1->inputCell;
            Cell* const &prevCell_1 = A2->inputCell;
            // Choose the smaller arrival time input
            double arrival_0 = prevCell_0->arrivalTime + prevCell_0->delay + WIRE_DELAY;
            double arrival_1 = prevCell_1->arrivalTime + prevCell_1->delay + WIRE_DELAY;
            if(arrival_0 < arrival_1){
                cell->inputTransition = prevCell_0->outputTransition;
                cell->arrivalTime = arrival_0;
            }
            else /* arrival_1 < arrival_0 */ {
                cell->inputTransition = prevCell_1->outputTransition;
                cell->arrivalTime = arrival_1;
            }
        }
        else if(A1->value == cell->controlledValue){
            Cell* const &prevCell = A1->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else if(A2->value == cell->controlledValue){
            Cell* const &prevCell = A2->inputCell;
            cell->inputTransition = prevCell->outputTransition;
            cell->arrivalTime = prevCell->arrivalTime + prevCell->delay + WIRE_DELAY;
        }
        else /* A1->value != cell->controlledValue && A2->value != cell->controlledValue */ {
            Cell* const &prevCell_0 = A1->inputCell;
            Cell* const &prevCell_1 = A2->inputCell;
            // Choose the bigger arrival time input
            double arrival_0 = prevCell_0->arrivalTime + prevCell_0->delay + WIRE_DELAY;
            double arrival_1 = prevCell_1->arrivalTime + prevCell_1->delay + WIRE_DELAY;
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
    cell->value = truthTable.at(cell->type)({A1->value,A2->value});
}


void STA::synthesis(const vector<char> &pattern){
    for(size_t i = 0; i < pattern.size(); i++){
        netMap.at(patternOrder[i])->value = pattern[i];
    }
    // Traverse netlist in topological order
    for(Cell* &cell : t_sort){
        // Set input transition time
        calInputTransitionTime_Synthesis(cell);
        // Calculate intrinsic delay and rise/fall transition time by look-up table
        if(cell->value == '1'){
            cell->delay = tableLookUp(cell,"cell_rise");
            cell->outputTransition = tableLookUp(cell,"rise_transition");
            cell->outputNet->value = '1';
        }
        else /* cell->outputNet->value == '0' */ {
            cell->delay = tableLookUp(cell,"cell_fall");
            cell->outputTransition = tableLookUp(cell,"fall_transition");
            cell->outputNet->value = '0';
        }
    }
}

void STA::assignPattern(){
    ofstream output("312510224_" + netlistName + "_gate_info.txt");
    for(const vector<char> &pattern:patterns){
        synthesis(pattern);
        dumpGateInfo(output);
    }
    output.close();
}

void STA::dumpGateInfo(ofstream &output){
    vector<Cell*> cellsInGateOrder(t_sort);
    sort(cellsInGateOrder.begin(), cellsInGateOrder.end(), Cell::cmpWithGateOrder);
    for(Cell* cell : cellsInGateOrder){
        output << cell->name  << " " 
               << cell->value << " "
               << fixed << setprecision(6)
               << cell->delay << " "
               << cell->outputTransition << '\n';
    }
    output << '\n';
}