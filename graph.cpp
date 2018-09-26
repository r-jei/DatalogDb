#include "graph.h"

void Graph::initialize(std::vector<rule>& rules){
}

int Graph::addNode(int i){
    Node n;
    nodes[i]=n;
    return 0;
}

int Graph::addEdge(int from,int to){
    nodes[from].edges.insert(to);
    return 0;
}

int Graph::visit(int node){
    nodes[node].visited=1;
    return 0;
}

int Graph::assignPostorder(int node,int num){
    nodes[node].postorder=num;
    return 0;
}

std::vector<std::set<int>> Graph::findsccs(std::vector<int> reversepos){
    std::vector<std::set<int>> components;
    clearVisits();
    int start=-1;
    
    for(int i=reversepos.size()-1; i>=0; i--){
        if(!nodes[reversepos[i]].visited){
            std::set<int> visitednodes;
            dfs(reversepos[i],nodes[reversepos[i]],start,visitednodes);
            components.push_back( visitednodes );
        }
    }
    sccs=components;
    return components;
}

void Graph::clearVisits(){
    for(std::map<int,Node>::iterator it=nodes.begin(); it!=nodes.end(); it++){
        (*it).second.visited=0;
    }
    return;
}

//post order traversal numbers are assigned to the whole graph.
//also, po_stack contains the list of rule numbers ascending by post traversal number.
void Graph::dfsForest(){
    int start=-1;
    
    clearVisits();
    
    for(std::map<int,Node>::iterator it=nodes.begin(); it!=nodes.end(); it++){
       if(!((*it).second.visited)){
           std::set<int> visitednodes;
           dfs((*it).first,(*it).second,start,visitednodes);
       }
    }
    return;
}

//potn is the post order traversal number. rulenum is the number of the rule the node represents.
std::set<int> Graph::dfs(int rulenum,Node& n,int& potn,std::set<int>& visitednodes){
    n.visited=1;
    visitednodes.insert(rulenum);
    for(std::set<int>::iterator it=n.edges.begin(); it!=n.edges.end(); it++){
        if(!nodes[(*it)].visited){
            std::set<int> subreachable=dfs((*it),nodes[(*it)],potn,visitednodes);
        }
    }
    potn++;
    n.postorder=potn;
    po_stack.push_back(rulenum);
    return visitednodes;
}

Node Graph::getNode(int n){
    return nodes[n];
}

std::vector<int> Graph::getPOS(){
    return po_stack;
}

std::vector<std::set<int>> Graph::getsccs(){
    return sccs;
}

std::string Graph::toString(){
    std::string str="";
    for(std::map<int,Node>::iterator mit=nodes.begin(); mit!=nodes.end(); mit++){
        int targetrulenum=(*mit).first;
        str+="R";
        str+=std::to_string(targetrulenum);
        str+=":";
        for(std::set<int>::iterator sit=nodes[targetrulenum].edges.begin(); sit!=nodes[targetrulenum].edges.end(); sit++){
            int dependency=(*sit);
            str+="R";
            str+=std::to_string(dependency);
            sit++;
            if((sit)!=nodes[targetrulenum].edges.end()){
                str+=",";
            }
            sit--;
        }
        str+="\n";
    }
    //printPOTN();
    return str;
}

void Graph::printPOTN(){
    for(unsigned int i=0; i<po_stack.size(); i++){
        std::cout << "POTN(R" << std::to_string(po_stack[i]) << ") = " << nodes[po_stack[i]].postorder << std::endl;
    }
}

void Graph::printsccs(){
    for(unsigned int i=0; i<sccs.size(); i++){
        std::cout << "SCC #" << std::to_string(i) << "\n";
        for(std::set<int>::iterator it=sccs[i].begin(); it!=sccs[i].end(); it++){
            std::cout << std::to_string((*it)) << " ";
        }
        std::cout << std::endl;
    }
}
