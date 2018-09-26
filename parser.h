#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <iostream>
#include <vector>
#include <set>
#include "predicate.h"
#include "rule.h"
#include "token.h"


class parser{
private:
    token currentToken;
    std::list<token>::iterator it;

    std::vector<predicate> schemes;
    std::vector<predicate> facts;
    std::vector<rule> rules;
    std::vector<predicate> queries;
    std::set<std::string> domain;
    
public:

    void error();
    void match(tokentype type);
    token getToken();
    std::vector<predicate> getSchemes();
    std::vector<predicate> getFacts();
    std::vector<rule> getRules();
    std::vector<predicate> getQueries();

    
    void schemeFunc();
    void factFunc();
    void ruleFunc();
    predicate headPredicate();
    predicate predicateFunc();
    parameter parameterFunc();
    void queryFunc();
    parameter expressionFunc(parameter& inputexpression);
    std::string operatorFunc();

    
    void schemeList();
    void idList(predicate& acceptor);
    void factList();
    void ruleList();
    std::vector<predicate> predicateList(std::vector<predicate>& acceptor);
    void parameterList(predicate& acceptor);
    void queryList();
    std::set<std::string> stringList(std::set<std::string>& stringsfound, predicate& acceptor);
    void datalogProgram();
    
    void parse(std::list<token>& lt);
    void print();
};
#endif
