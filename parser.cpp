#include "header.h"
#include "parser.h"

/*

*/

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////
void parser::match(tokentype type){
    if(currentToken.getTokenType() == type){
        //std::cout << "matched " << currentToken.getTypeString() << " with " << typeToString(type) << '\n';
        currentToken = getToken();
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
token parser::getToken(){
    it++;
    return (*it);
}

std::vector<predicate> parser::getSchemes(){
    return schemes;
}

std::vector<predicate> parser::getFacts(){
    return facts;
}

std::vector<rule> parser::getRules(){
    return rules;
}

std::vector<predicate> parser::getQueries(){
    return queries;
}

////////////////////////////////////////////////////////////////////////////////////////
/* When this function is called, it adds a new predicate to parser::schemes
*/
void parser::schemeFunc(){
    if( currentToken.getTokenType() == ID ){
        predicate pre;
        parameter currentparameter;
        
        pre.setName(currentToken.getValue()); match(ID); match(LEFT_PAREN);
        currentparameter.setValue(currentToken.getValue()); pre.addIdParameter(currentparameter); match(ID); idList(pre);
        
        match(RIGHT_PAREN);
        
        schemes.push_back(pre);
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
/* This function adds a predicate to parser::facts every time it is called. Any strings encountered are added to parser::domain.
*/
void parser::factFunc(){
    if( currentToken.getTokenType() == ID){
        predicate pre;
        std::set<std::string> stringsfound;
        parameter currentparameter;
        
        pre.setName(currentToken.getValue()); match(ID);
        
        match(LEFT_PAREN);
        
        currentparameter.setValue(currentToken.getValue()); pre.addStrParameter(currentparameter); domain.insert(currentToken.getValue()); match(STRING);
        stringsfound = stringList(stringsfound,pre); 
        
        match(RIGHT_PAREN);
        match(PERIOD);
        
        domain.insert(stringsfound.begin(),stringsfound.end());
        facts.push_back(pre);
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::ruleFunc(){
    if( currentToken.getTokenType() == ID){
        std::vector<predicate> body;
        
        rule foundrule(headPredicate()); match(COLON_DASH);
        
        foundrule.addPredicate(predicateFunc());
        foundrule.appendPredicates(predicateList(body));
        
        match(PERIOD);
        
        rules.push_back(foundrule);
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
/*
If the sequence of tokens matches correct syntax, it returns a predicate loaded with the tokens' data
*/
predicate parser::predicateFunc(){
    if( currentToken.getTokenType() == ID){
        predicate pre;    
        pre.setName(currentToken.getValue()); match(ID); match(LEFT_PAREN);
        pre.addParameter(parameterFunc()); parameterList(pre); match(RIGHT_PAREN);
        return pre;
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

parameter parser::parameterFunc(){
    if( currentToken.getTokenType() == STRING){
        parameter stringpar;
        stringpar.setValue(currentToken.getValue()); match(STRING);
        return stringpar;
        
    }else if(currentToken.getTokenType() == ID){
        parameter idpar;
        idpar.setValue(currentToken.getValue()); match(ID);
        return idpar;
        
    }else if(currentToken.getTokenType() == LEFT_PAREN){
        parameter expressionpar;
        expressionFunc(expressionpar);
        return expressionpar;
    }else{
        throw currentToken;
    }

}

////////////////////////////////////////////////////////////////////////////////////////
std::string parser::operatorFunc(){
    if( currentToken.getTokenType() == ADD){
        match(ADD);
        return "+";
    }else if( currentToken.getTokenType() == MULTIPLY){
        match(MULTIPLY);
        return "*";
    }else{
        throw currentToken;
    }

}

////////////////////////////////////////////////////////////////////////////////////////
void parser::queryFunc(){
    if( currentToken.getTokenType() == ID){
        queries.push_back(predicateFunc()); match(Q_MARK);
        
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
/*

*/
parameter parser::expressionFunc(parameter& inputexpression){
    if( currentToken.getTokenType() == LEFT_PAREN){
        
        inputexpression.appendStringToValue("(");
        //std::cout << inputexpression.getValue() << std::endl;
        match(LEFT_PAREN);
        
        
        inputexpression.appendStringToValue( (parameterFunc()).getValue() );
        //std::cout << inputexpression.getValue() << std::endl;
        inputexpression.appendStringToValue(operatorFunc());
        //std::cout << inputexpression.getValue() << std::endl;
        inputexpression.appendStringToValue( (parameterFunc()).getValue() );
        //std::cout << inputexpression.getValue() << std::endl;
        
        inputexpression.appendStringToValue(")");
        //std::cout << inputexpression.getValue() << std::endl;
        match(RIGHT_PAREN);
        
        //std::cout << inputexpression.getValue() << std::endl;
        return inputexpression;
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
predicate parser::headPredicate(){
    if( currentToken.getTokenType() == ID){
        predicate acceptor;
        parameter currentparameter;
        
        acceptor.setName(currentToken.getValue()); match(ID); 
        
        match(LEFT_PAREN);
        
        currentparameter.setValue(currentToken.getValue());acceptor.addIdParameter(currentparameter); match(ID); idList(acceptor); match(RIGHT_PAREN);
        
        return acceptor;
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::schemeList(){
    if( currentToken.getTokenType() == ID ){
        schemeFunc(); schemeList();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::idList(predicate& acceptor){
    if( currentToken.getTokenType() == COMMA ){
        parameter currentparameter;
        
        match(COMMA);
        
        currentparameter.setValue(currentToken.getValue()); acceptor.addIdParameter(currentparameter); match(ID);
        
        idList(acceptor);
        return;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::factList(){
    if( currentToken.getTokenType() == ID){
        factFunc(); factList();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::ruleList(){
    if( currentToken.getTokenType() == ID){
        ruleFunc(); ruleList();
    }
}

////////////////////////////////////////////////////////////////////////////////////////
std::vector<predicate> parser::predicateList(std::vector<predicate>& in){
    if( currentToken.getTokenType() == COMMA){
        match(COMMA); in.push_back(predicateFunc()); predicateList(in);
    }
    return in;
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::parameterList(predicate& acceptor){
    if( currentToken.getTokenType() == COMMA){
        match(COMMA); acceptor.addParameter(parameterFunc()); parameterList(acceptor);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::queryList(){
    if( currentToken.getTokenType() == ID){
        queryFunc(); queryList();
        }
}

////////////////////////////////////////////////////////////////////////////////////////
std::set<std::string> parser::stringList(std::set<std::string>& stringsfound, predicate& acceptor){
    if( currentToken.getTokenType() == COMMA){
        parameter currentParameter;
        
        match(COMMA); currentParameter.setValue(currentToken.getValue()); acceptor.addStrParameter(currentParameter);
        stringsfound.insert(currentToken.getValue()); match(STRING);
        
        stringList(stringsfound,acceptor);
    }
    return stringsfound;
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::datalogProgram(){
    if( currentToken.getTokenType() == SCHEMES){
        match(SCHEMES); match(COLON); schemeFunc(); schemeList();
        match(FACTS); match(COLON); factList();
        match(RULES); match(COLON); ruleList();
        match(QUERIES); match(COLON); queryFunc(); queryList();
    }else{
        throw currentToken;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
void parser::parse(list<token>& lt){
    it = lt.begin();
    currentToken = (*it);
    try{
        datalogProgram();
        //std::cout << "Success!" << std::endl;
        //print();
    }catch(token temp){
        std::cout << "Parser Failure!\n  (" << temp.getTypeString() << ",\"" << temp.getValue() << "\"," << temp.getLineNumber() << ')' << std::endl;
    }
}

void parser::print(){

    std::cout << "Schemes(" << schemes.size() << "):\n";
    for(std::vector<predicate>::iterator it=schemes.begin(); it<schemes.end(); it++){
        std::cout << "  " << (it)->toString() << "\n";
    }
    
    std::cout << "Facts(" << facts.size() << "):\n";
    for(std::vector<predicate>::iterator it=facts.begin(); it<facts.end(); it++){
        std::cout << "  " << (it)->toString() << ".\n";
    }
    
    std::cout << "Rules(" << rules.size() << "):\n";
    for(std::vector<rule>::iterator it=rules.begin(); it<rules.end(); it++){
        std::cout << "  " << (it)->toString() << "\n";
    }
    
    std::cout << "Queries(" << queries.size() << "):\n";
    for(std::vector<predicate>::iterator it=queries.begin(); it<queries.end(); it++){
        std::cout << "  " << (it)->toString() << "?\n";
    }
    
    std::cout << "Domain(" << domain.size() << "):\n";
    for(std::set<std::string>::iterator it=domain.begin(); it!=domain.end(); it++){
        std::cout << " " << (*it) << "\n";
    }
}
