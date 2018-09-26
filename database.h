#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <string>
#include <map>
#include "relation.h"

class Database{
private:
    std::map<std::string, Relation> relations;
public:
    Database();
    void addRelation(Relation inrel);
    void addTuple(std::string targetrelname,Tuple newtuple);
    
    std::string toString();
    Relation getRelation(std::string targetrelname);
    int getDatabaseSize();
    
    Relation idSelect(std::string targetrelname,int atta,int attb);
    Relation valueSelect(std::string targetrelname,int att,std::string value);
    Relation project(std::string targetrelname,std::vector<int> attributes);
    Relation rename(std::string targetrelname,std::vector<std::string> schema);
    Relation unionWith(std::string& targetrelname,Relation& unionw);
};
#endif
