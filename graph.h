#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "rule.h"
#include <map>
#include <string>
#include <vector>

class Graph{
private:
    std::map<int,Node> nodes;
    std::vector<int> po_stack;
    std::vector<std::set<int>> sccs;
public:
    void initialize(std::vector<rule>& rules);
    int addNode(int i);
    int addEdge(int from,int to);
    int visit(int node);
    int assignPostorder(int node,int num);
    
    std::vector<std::set<int>> findsccs(std::vector<int> pos);
    void clearVisits();
    void dfsForest();
    std::set<int> dfs(int rulenum,Node& n,int& potn,std::set<int>& visitednodes);
    
    Node getNode(int n);
    std::vector<int> getPOS();
    std::vector<std::set<int>> getsccs();
    
    std::string toString();
    void printPOTN();
    void printsccs();
    std::set<int> dfs(std::vector<rule>& rules,Node n); //depth first search. returns the set of all nodes reachable from input node
};
#endif
