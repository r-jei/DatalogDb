#include "rule.h"

rule::rule(predicate head){
	headpredicate = head;
}

void rule::setHeadPredicate(predicate p){
	headpredicate = p;
}

void rule::addPredicate(predicate p){
	predicates.push_back(p);
}

void rule::appendPredicates(std::vector<predicate> appendees){
	predicates.insert(predicates.end(),appendees.begin(),appendees.end());
}

std::string rule::toString(){
	std::string fillmeup = headpredicate.toString();
	
	fillmeup += " :- ";
	
	std::vector<predicate>::iterator it = predicates.begin();
	fillmeup += (*it).toString();
	it++;
	
	for(it=it; it<predicates.end(); it++){
		fillmeup += ",";
		fillmeup += (*it).toString();
	}
	
	fillmeup += ".";
	
	return fillmeup;
}

predicate rule::getHeadPredicate(){
	return headpredicate;
}

std::vector<predicate> rule::getPredicates(){
    return predicates;
}
