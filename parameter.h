#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>

enum paramtype{
    STRINGPARAM, IDPARAM, EXPRESSIONPARAM, UNDEFINEDPARAM
};

////////////////////////////////////////////////////////////////////////////////////////
class parameter{
private:
    std::string value;
    paramtype type;
    
public:
    parameter();
    void setValue(std::string input);
    void setType(paramtype input);
    void appendStringToValue(std::string in);
        
    std::string toString();
    paramtype getParameterType();
    std::string getValue();
};

#endif
