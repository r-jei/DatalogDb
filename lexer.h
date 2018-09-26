#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <list>
#include <map>
#include <cctype>
#include "token.h"

class finder{
private:
	int linenumber;
public:
	finder();
	int getLineNumber();
	void setLineNumber(int ln);
	token findToken(std::ifstream& input);
	bool charCompare(char a,char b);


};
class machine{
private:

public:
	machine();
	void updateValue(token& t, char c);
	void updateLineNumber(token& t);
};

class comment_Machine:machine{
private:

public:
	token type(std::ifstream& in, token& t, int& ln);
	token block(std::ifstream& in, token& t, int& ln);
	token blockIllegal(std::ifstream& in,token& t, int& ln);
	token blockEnd(std::ifstream& in,token& t,int& ln);
	token line(std::ifstream& in, token& t);
};
class string_Machine:machine{
private:

public:
	string_Machine();
	token findString(std::ifstream& in, token& t, int& ln);
	token endSeq(std::ifstream& in, token& t, int& ln);
};
class id_Machine:machine{
private:

public:
	id_Machine();
	token findID(std::ifstream& in, token& t, int& ln);
	bool keywordCheck(token& t);
};
class symbol_Machine:machine{
private:
	std::map<char,tokentype> symboltypes;
public:
	symbol_Machine();
	token findSymbol(std::ifstream& in, char c, token& t, int& ln);
};
class lexer{
private:
	std::ifstream input;
	std::list<token> tokens;
	int linenumber;

	std::map<tokentype,std::string> typekey;

	finder finder1;

public:
	lexer();
	void setLineNumber(int ln);
	int getLineNumber();
	std::list<token> getTokenList();
	std::list<token> loadList(std::string s);
	void printTokens();
};

#endif
