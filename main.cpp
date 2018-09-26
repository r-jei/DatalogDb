#include "header.h"
#include "lexer.h"
#include "parser.h"
#include "database.h"
#include "relation.h"
#include "tuple.h"
#include "interpreter.h"
#include "graph.h"
#include "node.h"
#include "predicate.h"
#include <iostream>

std::string typeToString(tokentype t){
    std::map<tokentype,std::string> typekey;
    typekey[COMMA]="COMMA";    typekey[PERIOD]="PERIOD"; typekey[Q_MARK]="Q_MARK";
    typekey[LEFT_PAREN]="LEFT_PAREN"; typekey[RIGHT_PAREN]="RIGHT_PAREN"; 
    typekey[COLON]="COLON";
    typekey[COLON_DASH]="COLON_DASH"; typekey[MULTIPLY]="MULTIPLY"; typekey[ADD]="ADD";
    typekey[SCHEMES]="SCHEMES"; typekey[FACTS]="FACTS"; typekey[RULES]="RULES";
    typekey[QUERIES]="QUERIES"; typekey[ID]="ID"; typekey[STRING]="STRING";
    typekey[COMMENT]="COMMENT"; typekey[WHITESPACE]="WHITESPACE";
    typekey[UNDEFINED]="UNDEFINED";
    typekey[ENDOFFILE]="EOF";
    return typekey[t];
}

using namespace std;

int main(int argc, char *argv[]){

    lexer lexerMain;
    parser parserMain;

    std::list<token> tokenList = lexerMain.loadList(argv[1]);

    parserMain.parse( tokenList );

    Database db;
    Interpreter interpret;
    interpret.loadRelations(parserMain.getSchemes(),db);
    interpret.loadTuples(parserMain.getFacts(),db);
    interpret.interpretRules(parserMain.getRules(),db);

    std::cout << "Query Evaluation\n";
    interpret.askQueries(parserMain.getQueries(),db);

    return 0;
}
