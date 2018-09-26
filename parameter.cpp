#include "parameter.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////////
parameter::parameter(){
	value = "";
	type = UNDEFINEDPARAM;
}


void parameter::setValue(std::string input){
	value = input;
}

void parameter::appendStringToValue(std::string in){
	value += in;
}

void parameter::setType(paramtype input){
	type = input;
}

std::string parameter::toString(){
	//std::cout << "parameter to string!\n";
	return value;
}

paramtype parameter::getParameterType(){
	return type;
}

std::string parameter::getValue(){
	return value;
}
