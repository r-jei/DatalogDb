#ifndef PREDICATE_H
#define PREDICATE_H

#include <string>
#include <vector>
#include <iostream>
#include "parameter.h"
#include "tuple.h"

class predicate{
private:
    std::string name;
    std::vector<parameter> parameters;
    
public:
    predicate();
    void setName(std::string s);
    void addParameter(parameter current);
    void addStrParameter(parameter current);
    void addIdParameter(parameter current);
    void addExpParameter(parameter current);
    
    std::string toString();
    std::vector<parameter> getParameters();
    std::string getName();
    Tuple getTuple();
};

#endif
