#include "STA.h"

STA::STA(): inputNum(0){
    ;
}

STA::~STA(){
    for(pair<string,Net*> p : netMap)   delete p.second;
    for(pair<string,Cell*> p : cellMap) delete p.second;
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
    netlistName.erase(netlistName.length()-2);
    
    // Read whole commented file into a string
    ifstream input(netlistPath);
    string commentedCode, curLine;
    while(getline(input,curLine)){ commentedCode += curLine + "\n"; }
    string cleanCode = removeComment(commentedCode);
    // add space
    cleanCode = regex_replace(cleanCode,Word_Pattern," $& ");
    // Parse 
    stringstream ss_code(cleanCode), ss;
    string opType;
    smatch match;
    string netName;
    string cellName;
    string pin;
    while(getline(ss_code,curLine)){
        // Use regular expression to extract current type
        ss.clear();
        ss.str(curLine);

        ss >> opType;
        // curLine = match.suffix().str();
        if(opType == "output" || opType == "input" || opType == "wire"){
            // Use regular expression to extract netName
            while(ss >> netName){
                if(isalpha(netName[0])){
                    if(opType == "input") inputNum++;
                    Net* net = new Net(netName,opType);
                    netMap[netName] = net;
                }
            }
        }
        else if(opType == "INVX1" || opType == "NANDX1" || opType == "NOR2X1"){
            // Extract cellName
            ss >> cellName;
            Cell* cell = new Cell(cellName,opType);

            while(ss >> curLine){
                // input net
                if(curLine == "A1"){
                    ss >> netName >> netName; // ignore '('
                    cell->inputNet[0] = netMap.at(netName);
                    netMap.at(netName)->outputCell.push_back(cell);
                }
                else if(curLine == "A2"){
                    ss >> netName >> netName; // ignore '('
                    cell->inputNet[1] = netMap.at(netName);
                    netMap.at(netName)->outputCell.push_back(cell);
                }
                else if(curLine == "I"){
                    ss >> netName >> netName; // ignore '('
                    cell->inputNet[0] = netMap.at(netName);
                    netMap.at(netName)->outputCell.push_back(cell);
                }
                // output net
                else if(curLine == "ZN"){
                    ss >> netName >> netName; // ignore '('
                    Net* outputNet = netMap.at(netName);
                    cell->outputNet = outputNet;
                    outputNet->inputCell = cell;
                    if(outputNet->type == "output") outputCell.push_back(cell);
                }
            }
            cellMap[cellName] = cell;
        }
    }
    input.close();
}

void STA::libraryParser(const string &filename){
    ifstream input(filename);
    string curLine;
    string cellType;
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
            cellType = match[1].str();
            CellInfo* cell_info = new CellInfo;
            if(cellType == "NOR2X1" || cellType == "NANDX1") cell_info->pinCap.resize(2);
            cellLib.cellMap[cellType] = cell_info;
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
            if(pin == "A1")      cellLib.cellMap[cellType]->pinCap[0] = stod(match[1].str());
            else if(pin == "A2") cellLib.cellMap[cellType]->pinCap[1] = stod(match[1].str());
            else /*pin == "I"*/  cellLib.cellMap[cellType]->pinCap.push_back(stod(match[1].str()));
        }

        // extract timing information
        else if(regex_search(curLine,match,Table_Pattern)){
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
            while(getline(ss,curLine)){
                timeStr += curLine;
                if(curLine.back() == '}') break;
            }
            // Store timing table
            while(regex_search(timeStr,match,Float_Pattern)){
                cellLib.cellMap[cellType]->tables[timeType].push_back(stod(match.str()));
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
                    outputLoad += cellLib.cellMap.at(outputCell->type)->pinCap[i];
                    break;
                }
            }
        }
        cell->outputLoad = outputLoad;
    }
}

#if TESTMODE
void STA::dumpOutputLoad(){
    ofstream output("312510224_" + netlistName + "_load.txt");
    vector<pair<string,Cell*>> cells(cellMap.begin(),cellMap.end());
    sort(cells.begin(),cells.end(), [](const pair<string,Cell*> &p1, const pair<string,Cell*> &p2){
        if(p1.second->outputLoad == p2.second->outputLoad){
            return stoi(p1.second->name.substr(1)) < stoi(p2.second->name.substr(1));
        }
        return p1.second->outputLoad > p2.second->outputLoad;
    });
    for(const pair<string,Cell*> p : cells){
        Cell* cell = p.second;
        output << cell->name << ' ' 
               << fixed << setprecision(6) 
               << cell->outputLoad << '\n';
    }
    output.close();
}
#else
void STA::dumpOutputLoad(){
    ofstream output("312510224_" + netlistName + "_load.txt");
    // vector<pair<string,Cell*>> cells(cellMap.begin(),cellMap.end());
    // sort(cells.begin(),cells.end(), [](const pair<string,Cell*> &p1, const pair<string,Cell*> &p2){
    //     if(p1.second->outputLoad == p2.second->outputLoad){
    //         return stoi(p1.second->name.substr(1)) < stoi(p2.second->name.substr(1));
    //     }
    //     return p1.second->outputLoad > p2.second->outputLoad;
    // });
    for(const pair<string,Cell*> p : cellMap){
        Cell* cell = p.second;
        output << cell->name << ' ' 
               << fixed << setprecision(6) 
               << cell->outputLoad << '\n';
    }
    output.close();
}
#endif


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
    if(index_1_idx == 0) index_1_idx = 1;
    else if(index_1_idx == colSize) index_1_idx = colSize - 1;

    for(index_2_idx = 0; index_2_idx < rowSize; index_2_idx++){
        if(cellLib.index_2[index_2_idx] > cell->inputTransition) break;
    }
    if(index_2_idx == 0) index_2_idx = 1;
    else if(index_2_idx == rowSize) index_2_idx = rowSize - 1;

    const vector<double> &table = cellLib.cellMap.at(cell->type)->tables.at(tableType);
    
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

#if TESTMODE
void STA::dumpDelay(){
    ofstream output("312510224_" + netlistName + "_delay.txt");
    vector<Cell*> cells(t_sort.begin(),t_sort.end());
    sort(cells.begin(),cells.end(),[](Cell* c1, Cell* c2){
        if(c1->delay == c2->delay){
            return c1->number < c2->number;
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
#else 
void STA::dumpDelay(){
    ofstream output("312510224_" + netlistName + "_delay.txt");
    // vector<Cell*> cells(t_sort.begin(),t_sort.end());
    // sort(cells.begin(),cells.end(),[](Cell* c1, Cell* c2){
    //     if(c1->delay == c2->delay){
    //         return stoi(c1->name.substr(1)) < stoi(c2->name.substr(1));
    //     }
    //     return c1->delay > c2->delay;
    // });
    for(Cell* &cell : t_sort){
        output << cell->name << " " 
               << cell->worstCaseValue << " " 
               << fixed << setprecision(6)
               << cell->delay << " " 
               << cell->outputTransition << '\n';
    }
    output.close();
}
#endif

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

void STA::calInputTransitionTime_Simulate(Cell* &cell){
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


void STA::simulate(const vector<char> &pattern){
    for(size_t i = 0; i < pattern.size(); i++){
        netMap.at(patternOrder[i])->value = pattern[i];
    }
    // Traverse netlist in topological order
    for(Cell* &cell : t_sort){
        // Set input transition time
        calInputTransitionTime_Simulate(cell);
        // Calculate intrinsic delay and rise/fall transition time by look-up table
        if(cell->value == '1'){
            cell->delay = tableLookUp(cell,"cell_rise");
            cell->outputTransition = tableLookUp(cell,"rise_transition");
            cell->outputNet->value = '1';
        }
        else /* cell->value == '0' */ {
            cell->delay = tableLookUp(cell,"cell_fall");
            cell->outputTransition = tableLookUp(cell,"fall_transition");
            cell->outputNet->value = '0';
        }
    }
}

void STA::assignPattern(){
    ofstream output("312510224_" + netlistName + "_gate_info.txt");
    vector<Cell*> cellsInGateOrder(t_sort);
    sort(cellsInGateOrder.begin(), cellsInGateOrder.end(), Cell::cmpWithGateOrder);
    for(vector<char> pattern : patterns){
        simulate(pattern);
        dumpGateInfo(output,cellsInGateOrder);
    }
    output.close();
}

void STA::dumpGateInfo(ofstream &output, const vector<Cell*> &cells){
    for(Cell* cell : cells){
        output << cell->name  << " " 
               << cell->value << " "
               << fixed << setprecision(6)
               << cell->delay << " "
               << cell->outputTransition << '\n';
    }
    output << '\n';
}