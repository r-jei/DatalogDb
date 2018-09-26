#include "predicate.h"

predicate::predicate(){}

void predicate::setName(std::string s){
	name = s;
}

//TODO when do we set the type of the parameters?

void predicate::addParameter(parameter current){
	parameters.push_back(current);
}

void predicate::addStrParameter(parameter current){
	parameters.push_back(current);
}

void predicate::addIdParameter(parameter current){

	parameters.push_back(current);
}

void predicate::addExpParameter(parameter current){
	parameters.push_back(current);	
}

std::string predicate::toString(){

	std::string fillmeup = getName();
	
	fillmeup += "(";
	
	std::vector<parameter>::iterator it = parameters.begin();
	if(parameters.empty()){
        std::cout << "ERROR: vector of parameters was not loaded\n";
        throw "ERROR: vector of parameters was not loaded";
    }
	fillmeup += (*it).toString();
	it++;
	
	
	for(it=it; it<parameters.end(); it++){
		fillmeup += ",";
		fillmeup += (*it).toString();
	}
	
	fillmeup += ")";
	
	return fillmeup;
}

std::vector<parameter> predicate::getParameters(){
	return parameters;
}

std::string predicate::getName(){
	return name;
}

Tuple predicate::getTuple(){
    Tuple t;
    for(unsigned int i=0; i<parameters.size(); i++){
        t.push_back(parameters[i].getValue());
    }
    return t;
}


