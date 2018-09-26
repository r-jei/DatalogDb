#ifndef RELATION_H
#define RELATION_H

#include "tuple.h"
#include <set>
#include <string>
#include <vector>
#include <iostream>

class Relation{
private:
    std::string name;
    std::vector<std::string> scheme;
    std::set<Tuple> tuples;
public:
    Relation();
    std::string getName();
    std::string toString();
    int getSize();
    int getSchemeSize();
    std::vector<std::string> getScheme();
    std::set<Tuple> getTuples();
        
    void addTuple(Tuple input);
    void setScheme(std::vector<std::string> newschema);
    void setName(std::string n);
    
    Relation valueSelect(int attribpos, std::string value);
    Relation idSelect(int attribposa, int attribposb);
    Relation project(std::vector<int> attributes);
    Relation rename(std::vector<std::string> newscheme);
    std::vector<std::string> combineSchemes(std::vector<std::string> scheme1,std::vector<std::string> scheme2);
    bool isJoinable(std::vector<std::string>& scheme1,Tuple& t1,std::vector<std::string>& scheme2,Tuple& t2);
    Tuple joinTuples(std::vector<std::string>& scheme1,Tuple& t1,std::vector<std::string>& scheme2,Tuple& t2);
    Relation join(Relation& joinee);
    Relation relUnion(Relation& r2);
};

#endif
