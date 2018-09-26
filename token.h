#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "header.h"

class token{
private:
	tokentype token_type;
	int line_number;
	std::string value;
	//std::map<tokentype,std::string> typekey;

public:
	token();
	tokentype getTokenType();
	std::string getTypeString();
	std::string getValue();
	int getLineNumber();
	void setTokenType(tokentype t);
	void setValue(std::string v);
	void setLineNumber(int ln);

};



#endif
