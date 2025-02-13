#include "Cell.h"

Cell::Cell():
    inputTransition(0.0),
    outputTransition(0.0),
    outputLoad(0.0),
    delay(0.0),
    arrivalTime(0.0),
    inDegree(0),
    prevCell(nullptr),
    worstCaseValue(' '),
    value(' '){
    ;
}

Cell::Cell(
    string &cellName, 
    CellType cellType
) : name(cellName),
    type(cellType),
    inputTransition(0.0),
    outputTransition(0.0),
    outputLoad(0.0),
    delay(0.0),
    arrivalTime(0.0),
    inDegree(0),
    prevCell(nullptr),
    worstCaseValue(' '){
        
    if(cellType == NANDX1)   controllingValue = false;
    else /* cellType == NOR2X1 */ controllingValue = true;

    inputNet = {{nullptr , nullptr}};
    number = stoi(cellName.substr(1));
}

Cell::~Cell(){
    ;
}

bool Cell::ascendingGateOrder(const Cell* const &cell_1, const Cell* const &cell_2){
    return cell_1->number < cell_2->number;
}