#include "database.h"

Database::Database(){
    
}

void Database::addRelation(Relation inrel){
    //std::cout << "addRelation: adding relation " << inrel.getName() << "to relations" <<  std::endl;
    std::pair<std::string,Relation> pair= std::pair<std::string,Relation>(inrel.getName(),inrel);
    relations.insert(pair);
}

void Database::addTuple(std::string targetrelname,Tuple newtuple){
    //std::cout << "adding tuple to " << targetrelname << std::endl;
    relations.at(targetrelname).addTuple(newtuple);
}

std::string Database::toString(){
    
    std::string dbstring = "";
    for(std::map<std::string,Relation>::iterator it=relations.begin(); it!=relations.end(); it++){
        dbstring += (*it).second.toString();
    }
    return dbstring;
}

int Database::getDatabaseSize(){
    int size=0;
    for(std::map<std::string,Relation>::iterator it=relations.begin(); it!=relations.end(); it++){
        size+=(*it).second.getSize();
    }
    return size;
}

Relation Database::getRelation(std::string targetrelname){
    if(relations.at(targetrelname).getSchemeSize()==0){
        std::string s = "!!!ERROR!!! getRelation - relation named ";
        s += targetrelname;
        s += "'s scheme is empty\n";
        throw s; 
    }
    return relations.at(targetrelname);
}

Relation Database::unionWith(std::string& targetrelname, Relation& unionw){
    relations[targetrelname]=relations[targetrelname].relUnion(unionw);
    return relations[targetrelname];
}
