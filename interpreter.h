#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include "predicate.h"
#include "database.h"
#include "rule.h"
#include "graph.h"

class Interpreter{
private:
    Graph dependencies;
    Graph revdep; //reverse graph
public:
    void loadRelations(std::vector<predicate> schemes, Database& db);
    void addTuple();
    void loadTuples(std::vector<predicate> facts, Database& db);
    int buildGraph(std::vector<rule> rules);
    void findsccs();
    
    void askQueries(std::vector<predicate> queries, Database& db);
    Relation selectByTuple(Tuple& t,Relation& targetrel,std::map<std::string,std::vector<int>>& activids,std::vector<std::string>& idsequence);
    std::vector<Relation> bodypreds_toRelations(std::vector<predicate>& bodypredicates,Database& db);
    void onepass(std::vector<rule>& rules, Database& db, std::set<int>& scc);
    int interpretscc(std::vector<rule>& rules, Database& db,std::set<int> scc);
    int interpretRules(std::vector<rule> rules,Database& db);
    Relation evaluatePredicate(Relation& targetrel,Tuple& variables);
    
    Graph getDep();
    Graph getRev();
};


#endif
