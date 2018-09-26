#ifndef RULE_H
#define RULE_H

#include <vector>
#include <string>
#include "predicate.h"

class rule{
private:
	predicate headpredicate;
	std::vector<predicate> predicates;
public:
	rule(predicate head);
	void setHeadPredicate(predicate p);
	void addPredicate(predicate p);
	void appendPredicates(std::vector<predicate> appendees);
	
	std::string toString();
	predicate getHeadPredicate();
    std::vector<predicate> getPredicates();
};
#endif