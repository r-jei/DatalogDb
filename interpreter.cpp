#include "interpreter.h"

/* The basic idea of loadRelations is:
 * Every predicate in schemes defines a new relation,
 * The schema of the new relation is all of the parameters of that scheme
 * The name of the new relation is the name of the current predicate
 */
void Interpreter::loadRelations(std::vector<predicate> schemes, Database& db){
    for(unsigned int i=0; i<schemes.size(); i++){
        Relation r;
        std::vector<std::string> schema;
        std::vector<parameter> attributes = schemes[i].getParameters();
        //std::cout << "Interpreter::loadRelations schemes[i].getName();" << schemes[i].getName() << std::endl;
        r.setName(schemes[i].getName());
        
        for(unsigned int j=0; j<attributes.size(); j++){
            //std::cout << "loadRelations pushin' it back\n";
            schema.push_back(attributes[j].getValue());
        }

        r.setScheme(schema);
        db.addRelation(r);
    }
    return;
}

/* The basic idea of loadTuples is: 
 * Where the name of every fact(predicate) is thdfFe target relation
 * create a tuple(std::vector<std::string>) of all of the current fact's parameters
 * add the tuple to the target relation within the database
 */

void Interpreter::loadTuples(std::vector<predicate> facts, Database& db){
   for(unsigned int i=0; i<facts.size(); i++){
       std::string targetrelation=facts[i].getName();
       Tuple strings=facts[i].getTuple();
       
       db.addTuple(targetrelation,strings);
   }
   return;
}

int Interpreter::buildGraph(std::vector<rule> rules){
    for(unsigned int i=0; i<rules.size(); i++){
        dependencies.addNode(i);
        revdep.addNode(i);
    }
    for(unsigned int homenum=0; homenum<rules.size(); homenum++){
        std::vector<predicate> bodypreds=rules[homenum].getPredicates();
        for(unsigned int bpnum=0; bpnum<bodypreds.size(); bpnum++){
            std::string bpname=bodypreds[bpnum].getName();
            for(unsigned int adjnum=0; adjnum<rules.size(); adjnum++){
                std::string hpname=rules[adjnum].getHeadPredicate().getName();
                if(hpname==bpname){
                    dependencies.addEdge(homenum,adjnum);
                    revdep.addEdge(adjnum,homenum);
                }
            }
        }
    }
    //std::cout << "Dependency Graph\n" << dependencies.toString();
    //std::cout << "Reverse Graph\n" << revdep.toString();
    
    revdep.dfsForest();
    //revdep.printPOTN();
    
    dependencies.findsccs(revdep.getPOS());
    //dependencies.printsccs();
    return 0;
}

/* The basic idea of askQueries is:
 * Every query is a predicate.
 * The name of the predicate is the target relation.
 * Because I do not have to check for arity, I can assume that attributes are defined by their index.
 * The parameters of a query make up a tuple.
 * This function checks if there are any tuples that match the conditions described by the parameters.
 * 
 * for testing
 * 
 * std::cout << "queries[i].getName() " << queries[i].getName() << std::endl;
 * std::cout << "db.getRelation(targetrelanem).getName() " << db.getRelation(targetrelname).getName();
 * std::cout << "db.getRelation(targetrelname).getSchemeSize() " << db.getRelation(targetrelname).getSchemeSize() << std::endl;
 * std::cout << "targetrel.getSchemeSize() " << targetrel.getSchemeSize() << std::endl;
 * 
 */
void Interpreter::askQueries(std::vector<predicate> queries, Database& db){
    
    for(unsigned int i=0; i<queries.size(); i++){
        
        std::string targetrelname=queries[i].getName();        
        Tuple t=queries[i].getTuple();      //this tuple contains all the parameters - ids and strings - within the query.
        Relation targetrel;       //the query will be applied to this relation. the resulting relation will have a scheme comprised of the IDs used in the query, and all tuples which matched the conditions described in the query.
        targetrel=db.getRelation(targetrelname);        
        std::map<std::string,std::vector<int>> activeids;       //this is used to detect if an ID has previously been encountered, and at what tuple indexes it occurs
        std::vector<std::string> idsequence;        //this is used to track the order of the first occurence of all IDs listed in the query
        Relation prorel;
        
        
        Relation selrel = selectByTuple(t,targetrel,activeids,idsequence);
        std::cout << queries[i].toString() << "? ";
        int matches=selrel.getSize();
        if(matches){
            std::cout << "Yes(" << matches << ")\n";
        }else{
            std::cout << "No\n";
        }
        
        //project
        std::vector<int> positions; //a vector of the positions of the first occurences of every variable (no constants, i.e. 'e', 'a' etc - only S N A P sort of things)
        for(unsigned int i=0; i<idsequence.size(); i++){
            positions.push_back(activeids[idsequence[i]][0]);
        }
        if(idsequence.size()!=0){
            prorel = selrel.project(positions);

            //rename
            Relation renrel = prorel.rename(idsequence);
            std::cout << renrel.toString();
        }
    }
}



Relation Interpreter::selectByTuple(Tuple& t,Relation& targetrel,std::map<std::string,std::vector<int>>& activeids,std::vector<std::string>& idsequence){
    //selections
    for(unsigned int j=0; j<t.size(); j++){
        if(t[j][0]=='\''){
            targetrel=targetrel.valueSelect(j,t[j]);
        }else{
            if( activeids[t[j]].size()>0 ){     //if we have already seen an ID of this value
                targetrel=targetrel.idSelect(activeids[t[j]][0],j);
                activeids[t[j]].push_back(j);
            }else{      //if this is the first time seeing an ID of this value
                activeids[t[j]].push_back(j);
                idsequence.push_back(t[j]);
            }
        }
    }
    return targetrel;
}

std::vector<Relation> Interpreter::bodypreds_toRelations(std::vector<predicate>& bodypredicates,Database& db){
    std::vector<Relation> intermediates;
    for(unsigned int j=0; j<bodypredicates.size(); j++){
        Relation targetrel=db.getRelation(bodypredicates[j].getName());
        Tuple variables=bodypredicates[j].getTuple();
        intermediates.push_back(evaluatePredicate(targetrel,variables));
    }
    return intermediates;
}

void Interpreter::onepass(std::vector<rule>& rules, Database& db, std::set<int>& scc){
    for(std::set<int>::iterator it=scc.begin(); it!=scc.end(); it++){
        Relation addrulesto;
        std::vector<predicate> bodypredicates;
        
        bodypredicates=rules[(*it)].getPredicates();
        std::string targetname=rules[(*it)].getHeadPredicate().getName();
        predicate headpredicate=rules[(*it)].getHeadPredicate();
        addrulesto=db.getRelation(targetname);        
        
        //1. evaluate every predicate on the right hand side of the rule like a query, returning an intermediate Relation for each predicate
        std::vector<Relation> intermediates=bodypreds_toRelations(bodypredicates,db);

        //2. join intermediates if necessary
        Relation resultofpredicates=intermediates[0];
        for(unsigned int j=1; j<intermediates.size(); j++){
            resultofpredicates=resultofpredicates.join(intermediates[j]);
        }
        //3. on the result of step 2, project only the columns whose attribute names appear in the head predicate
        Tuple headnames=rules[(*it)].getHeadPredicate().getTuple();
        std::vector<std::string> resultnames=resultofpredicates.getScheme();
        std::vector<int> resultcolumns;
        for(unsigned int j=0; j<headnames.size(); j++){
            for(unsigned int k=0; k<resultnames.size(); k++){
                if(headnames[j]==resultnames[k]){
                    resultcolumns.push_back(k);
                }
            }
        }
        
        Relation prorel=resultofpredicates.project(resultcolumns);
        //4. 
        prorel.setScheme(addrulesto.getScheme());
        //5.
        db.unionWith(targetname,prorel);
        
    }
}

//TODO this function's not complete!! work on it
int Interpreter::interpretscc(std::vector<rule>& rules, Database& db, std::set<int> scc){
    int passes=0;
    int before=0;
    int after=0;
    
    //trivial cases: scc only contains one node, and has no self-loop.
    if(scc.size()==1){
        //dependencies.
        Node targetnode=dependencies.getNode(*(scc.begin()));
        bool selfloop=0;
        
        for(std::set<int>::iterator it=targetnode.edges.begin(); it!=targetnode.edges.end(); it++){
            if((*it)==(*scc.begin())){
                selfloop=1;
            }
        }
        
        if(!selfloop){
            onepass(rules,db,scc);
            std::cout << "1 passes: R" << std::to_string(*(scc.begin())) << std::endl;
            return 0;
        }
    }    
    do{
        before=db.getDatabaseSize();
        //one pass through the scc
        onepass(rules,db,scc);
        after=db.getDatabaseSize();
        passes++;
        
    }while(before!=after);
    std::cout << std::to_string(passes) << " passes: ";
    for(std::set<int>::iterator it=scc.begin(); it!=scc.end(); it++){
        std::cout << "R" << std::to_string((*it));
        it++;
        if(it!=scc.end()){
            std::cout << ",";
        }
        it--;
    }
    std::cout << std::endl;
    return 0;
}

/*
1.
    For every predicate on the right hand side of a rule, evaluate that predicate in the same way queries are evaluated in the previous project. The result of the evaluation should be a relation. If there are n predicates on the right hand side of a rule, then there should be n intermediate relations from each predicate.
2.
    Next, if there is a single predicate on the right hand side of the rule, use the single intermediate result from Step 1 as the result for Step 2. If there are two or more predicates on the right-hand side of a rule, join all the intermediate results to form the single result for Step 2. Thus, if p1, p2, and p3 are the intermediate results from step one; you should construct a relation: p1 |x| p2 |x| p3.
3.
    It is possible for the predicate list in the original rule to have free variables (variables not defined in the head of the rule). Perform a project operation on the result from Step 2 to remove columns corresponding to free variables. That is, project on the result from Step 2 keeping only those columns whose attribute names appear in the head predicate of the rule.
4.
    Before taking the union in Step 5, we must first make the result of Step 3 be union compatible with the appropriate relation in the database. To make it union compatible: Select the relation r from the database whose name matches the name of the head predicate of the rule. For every variable v that appears in column i of the head predicate of the rule and column j of the result from Step 3, rename the attribute for column j of the result to the name of the attribute in column i of the schema of r.
5.
    Union the results of Step 4 with the relation in the database whose name is equal to the name of the head of the rule. In Step 4 we called this relation in the database r. Add tuples to relation r from the result of Step 4.

*/
int Interpreter::interpretRules(std::vector<rule> rules,Database& db){
    /*project 5 goes here!*/
    buildGraph(rules);
    std::cout << "Dependency Graph\n" << dependencies.toString() << "\n";

    std::cout << "Rule Evaluation\n";
    std::vector<std::set<int>> sccs=dependencies.getsccs();
    for(unsigned int i=0; i<sccs.size(); i++){
        interpretscc(rules,db,sccs[i]);
    }
    std::cout << std::endl;
    
    return 0;
}   

Relation Interpreter::evaluatePredicate(Relation& targetrel,Tuple& variables){
    std::map<std::string,std::vector<int>> activeids;       //this is used to detect if an ID has previously been encountered, and at what tuple indexes it occurs
    std::vector<std::string> idsequence;        //this is used to track the order of the first occurence of all IDs listed in the query
    
    Relation selrel=selectByTuple(variables,targetrel,activeids,idsequence);
    //project
    std::vector<int> positions; //a vector of the positions of the first occurences of every variable (no constants, i.e. 'e', 'a' etc - only S N A P sort of things)
    for(unsigned int i=0; i<idsequence.size(); i++){
        positions.push_back(activeids[idsequence[i]][0]);
    }
    if(idsequence.size()!=0){
        Relation prorel = selrel.project(positions);
        //rename
        Relation renrel = prorel.rename(idsequence);
        return renrel;
    }
    return selrel;
}

Graph Interpreter::getDep(){
    return dependencies;
}

Graph Interpreter::getRev(){
    return revdep;
}
