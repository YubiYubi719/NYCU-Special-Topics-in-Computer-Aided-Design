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
    if(cellType == INVX1){
        inputNet.resize(1);
        controllingValue = DONTCARE; // both 0 and 1 are controlling value
        type = INVX1;
    }
    else if(cellType == NANDX1){
        inputNet.resize(2);
        controllingValue = LOW;
        type = NANDX1;
    }
    else /* cellType == NOR2X1 */ {
        inputNet.resize(2);
        controllingValue = HIGH;
        type = NOR2X1;
    }
    number = stoi(cellName.substr(1));
}

Cell::~Cell(){
    ;
}

bool Cell::cmpWithGateOrder(const Cell* const &cell_1, const Cell* const &cell_2){
    return cell_1->number < cell_2->number;
}