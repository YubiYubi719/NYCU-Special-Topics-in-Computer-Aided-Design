#ifndef IMPLICANT_H
#define IMPLICANT_H
#include <string>
using namespace std;

class Implicant{
public:
    // Constructors/Distructors
    Implicant();
    Implicant(string bin);
    ~Implicant();

    // Member functions
    bool operator==(const Implicant &rhs) const;

    // Member variables
    string binary; // its position in binary form
    bool combinable;  // whether the implicant can grow bigger

    // For unordered_set
    struct Hash {
        size_t operator()(const Implicant &imp) const;
    };
};


#endif