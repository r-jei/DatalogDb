#include "relation.h"

Relation::Relation(){
    name="";
    std::vector<std::string> s;
    scheme=s;
    std::set<Tuple> t;
    tuples=t;
}

std::string Relation::getName(){
    return name;
}

std::string Relation::toString(){
    std::string relstr = "";
    for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
        relstr += "  ";
        relstr += scheme[0];
        relstr += "=";
        relstr += (*it)[0];
        for(unsigned int i=1; i<(*it).size(); i++){
            relstr += ", ";
            relstr += scheme[i];
            relstr += "=";
            relstr += (*it)[i];
        }
        relstr += '\n';
    }
    return relstr;
}

int Relation::getSize(){
    return tuples.size();
}

int Relation::getSchemeSize(){
    return scheme.size();
}

std::vector<std::string> Relation::getScheme(){
    return scheme;
}

std::set<Tuple> Relation::getTuples(){
    return tuples;
}

void Relation::addTuple(Tuple input){
    tuples.insert(input);
}

void Relation::setScheme(std::vector<std::string> newscheme){
    /*
    std::set<std::string> filterset;
    for(unsigned int i=0; i<newscheme.size(); i++){
        filterset.insert(newscheme[i]);
    }
    std::vector<std::string> midscheme;
    for(std::set<std::string>::iterator it=filterset.begin(); it!=filterset.end(); it++){
        midscheme.push_back((*it));
    }
    */
    scheme = newscheme;
    return;
}

void Relation::setName(std::string s){
    name=s;
}

/* Select
 * TODO - the name of the return relation is….?
 */

Relation Relation::idSelect(int attribposa, int attribposb){
    Relation subrelation;
    //std::cout << "idSelect" << scheme.size() << std::endl;
    subrelation.setScheme(scheme);
    for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
        if((*it)[attribposa]==(*it)[attribposb]){
            subrelation.addTuple((*it));
        }        
    }
    return subrelation;
}


Relation Relation::valueSelect(int attribpos, std::string value){
    Relation subrelation;
    //std::cout << "valueSelect" << scheme.size();
    subrelation.setScheme(scheme);
    //std::cout << "valueSelectsubrelationsize " << subrelation.getSchemeSize() << std::endl;
    for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
        if((*it)[attribpos]==value){
            subrelation.addTuple((*it));
        }
    }
    return subrelation;
}

/* Project
 * the name of the return relation is the parent relation's name
 * 
 */

Relation Relation::project(std::vector<int> attributes){
    if(scheme.size()==0){
        std::string s = "!!!ERROR!!! Relation::project - tried to project on relation ";
        s += name;
        s += ", whose scheme is empty.\n";
        throw s;
    }
    Relation prjrel;
    prjrel.setName(name);
    std::vector<std::string> newscheme;
    for(unsigned int i=0; i<attributes.size(); i++){
        //std::cout << attributes[i] << std::endl;
        //std::cout << scheme.size() << std::endl;
        newscheme.push_back(scheme[attributes[i]]);
    }
    prjrel.setScheme(newscheme);
    for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
        Tuple newtuple;
        for(unsigned int i=0; i<attributes.size(); i++){
            newtuple.push_back((*it)[attributes[i]]);
        }
        prjrel.addTuple(newtuple);
    }
    return prjrel;
}

/*PSEUDOCODE

Relation Relation::Rename(newscheme){
    Relation rnmrel;
    rnmrel.scheme = newscheme
    rnmrel.tuples = currenttuples
    return rnmrel
}*/
//TODO you could just initialize rnmrel as "this relation" and avoid having to do that for loop.
Relation Relation::rename(std::vector<std::string> newscheme){
    Relation rnmrel=(*this);
    rnmrel.setScheme(newscheme);
    /*for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
        rnmrel.addTuple((*it));
    }*/
    return rnmrel;
}

//
std::vector<std::string> Relation::combineSchemes(std::vector<std::string> scheme1,std::vector<std::string> scheme2){
    std::vector<std::string> newscheme = scheme1;
    for(unsigned int i=0; i<scheme2.size(); i++){
        bool commonname=0;
        for(unsigned int j=0; j<newscheme.size(); j++){
            if(newscheme[j]==scheme2[i]){
                //this name exists in both scheme1 and scheme2. We should not add it to the new scheme.
                commonname=1;
                break;
            }
        }
        //if the name located at scheme2[i] does not exist in scheme1, add it to the new scheme.
        if(!commonname){
            newscheme.push_back(scheme2[i]);
        }
    }
    return newscheme;
}

//
bool Relation::isJoinable(std::vector<std::string>& scheme1,Tuple& t1,std::vector<std::string>& scheme2,Tuple& t2){
    for(unsigned int i=0; i<scheme1.size(); i++){
        for(unsigned int j=0; j<scheme2.size(); j++){
            if(scheme1[i]==scheme2[j]){
                if(t1[i]!=t2[j]){
                    return 0;
                }
            }
        }
    }
    return 1;
}

//TODO
Tuple Relation::joinTuples(std::vector<std::string>& scheme1,Tuple& t1,std::vector<std::string>& scheme2,Tuple& t2){
    Tuple newtuple=t1;
    std::vector<std::string> newscheme = scheme1;
    for(unsigned int i=0; i<scheme2.size(); i++){
        bool commonname=0;
        for(unsigned int j=0; j<newscheme.size(); j++){
            if(newscheme[j]==scheme2[i]){
                //this name exists in both scheme1 and scheme2. We should not add its value to the tuple.
                commonname=1;
                break;
            }
        }
        //if the name located at scheme2[i] does not exist in scheme1, add it to the new scheme.
        if(!commonname){
            newscheme.push_back(scheme2[i]);
            newtuple.push_back(t2[i]);
        }
    }
    return newtuple;
}

Relation Relation::join(Relation& joinee){
    std::vector<std::string> joineesch=joinee.getScheme();
    std::vector<std::string> newscheme = combineSchemes(scheme,joineesch);
    Relation newrel;
    newrel.setScheme(newscheme);
    std::set<Tuple> jtuples = joinee.getTuples();
    
    for(std::set<Tuple>::iterator it1=tuples.begin(); it1!=tuples.end(); it1++){
        Tuple t1=(*it1);
        for(std::set<Tuple>::iterator it2=jtuples.begin(); it2!=jtuples.end(); it2++){
            Tuple t2=(*it2);
            bool joinable=isJoinable(scheme,t1,joineesch,t2);
            if(joinable){
                //join (*it1) and (*it2) to make tuple t
                Tuple newtuple = joinTuples(scheme,t1,joineesch,t2);
                //add tuple t to newrel
                newrel.addTuple(newtuple);
            }
        }
    }    
    return newrel;
}

Relation Relation::relUnion(Relation& r2){
    Relation r3;
    if(scheme!=r2.getScheme()){
        std::cout << "ERROR: Relations " << name << " and " << r2.getName() << " are not union-compatible (schemes are different!)\n";
        throw "Relations not union-compatible!\n";
    }else{

        r3.setScheme(scheme);
        r3.setName(name);
        
        for(std::set<Tuple>::iterator it=tuples.begin(); it!=tuples.end(); it++){
            r3.addTuple((*it));
        }
        
        std::set<Tuple> r2tuples = r2.getTuples();
        for(std::set<Tuple>::iterator it=r2tuples.begin(); it!=r2tuples.end(); it++){
            r3.addTuple((*it));
        }
    }
    return r3;
}
