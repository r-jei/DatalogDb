#include "token.h"

token::token(){
	token_type = UNDEFINED;
	line_number = 0;
	value = "";
}
tokentype token::getTokenType(){
	return token_type;
}
std::string token::getTypeString(){
	return typeToString(token_type);
}
std::string token::getValue(){
	return value;
}
int token::getLineNumber(){
	return line_number;
}
void token::setTokenType(tokentype t){
	token_type = t;
}
void token::setValue(std::string v){
	value = v;
}
void token::setLineNumber(int ln){
	line_number = ln;
}
