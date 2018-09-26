#include "header.h"
#include "lexer.h"

//!!! the comment-finder's functionality has been disabled. When a comment is found, its tokentype is set to WHITESPACE, which is never pushed on to the list of tokens.

machine::machine(){

}

////////////////////////////////////////////////////////////////////////////////////////

void machine::updateValue(token& t, char c){
	std::string s = t.getValue() + c;
	t.setValue(s);
}

////////////////////////////////////////////////////////////////////////////////////////

void machine::updateLineNumber(token& t){
	int i = t.getLineNumber() + 1;
	t.setLineNumber(i);
}

////////////////////////////////////////////////////////////////////////////////////////

token comment_Machine::type(std::ifstream& in, token& t, int& ln){
	char c = in.get();
	if(c == '|'){
		updateValue(t,c);
		return block(in,t,ln);

	}else{
		//updateValue(t,c);
		in.unget();
		return line(in,t);

	}
}

////////////////////////////////////////////////////////////////////////////////////////

token comment_Machine::block(std::ifstream& in, token& t, int& ln){
	//std::cout << "enter comment_Machine.block()\n";
	char c = in.get();
	//std::cout << "comment_Machine.block got " << c << endl;
	if(c == '|'){
		updateValue(t,c);
		return blockEnd(in,t,ln);

	}else if(c == EOF){
		in.clear();
		t.setTokenType(UNDEFINED);
		return t;

	}else{
		if(c == '\n'){
			ln++;
		}
		updateValue(t,c);
		return block(in,t,ln);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

token comment_Machine::blockIllegal(std::ifstream& in,token& t, int& ln){
	//output ENDOFFILE token
	//std::cout << "ENDOFFILE";
	return t;
}

////////////////////////////////////////////////////////////////////////////////////////

token comment_Machine::blockEnd(std::ifstream& in,token& t, int& ln){
	//std::cout << "enter comment_Machine.blockEnd()\n";
	char c = in.get();
	//std::cout << "comment_Machine.blockEnd got " << c << endl;
	if(c == '#'){
		//write COMMENT token
		updateValue(t,c);
		t.setTokenType(WHITESPACE);
		//std::cout << "blockEnd - t.setTokenType(COMMENT)\n";
		return t;
	}else{
		if(c == '\n'){
			ln++;
		}
		updateValue(t,c);
		return block(in,t,ln);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

token comment_Machine::line(std::ifstream& in, token& t){
	//std::cout << "enter comment_Machine.line()\n";
	char c = in.get();
	//std::cout << "comment_Machine.line got " << c << endl;

	if(c == '\n'){
		//write COMMENT token
		in.unget();
		in.clear();
		//std::cout << "comment_Machine.line unget " << c << endl;
		t.setTokenType(WHITESPACE);
		//std::cout << "line - t.setTokenType(COMMENT)\n";
		return t;

	}else if(c==EOF){
		in.unget();
		in.clear();
		t.setTokenType(WHITESPACE);
		return t;
	}else{
		std::string s = t.getValue();
		s.push_back(c);
		t.setValue( s );
		return line(in,t);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

string_Machine::string_Machine(){

}

////////////////////////////////////////////////////////////////////////////////////////

token string_Machine::findString(std::ifstream& in, token& t, int& ln){
	char c = in.get();
	//std::cout << "string_Machine.findString got " << c << endl;

	if(c == '\''){
		updateValue(t,c);
		return endSeq(in,t,ln);

	}else if(c == EOF){
		in.clear();
		t.setTokenType(UNDEFINED);
		return t;

	}else if(c == '\n'){
		updateValue(t,c);
		ln++;
		return findString(in,t,ln);

	}else{
		updateValue(t,c);
		return findString(in,t,ln);
	}
}

////////////////////////////////////////////////////////////////////////////////////////

token string_Machine::endSeq(std::ifstream& in, token& t, int& ln){
	char c = in.get();
	//if c=='\'', then we've come across an empty string
	if( c=='\'' ){
		updateValue(t,c);
		return findString(in,t,ln);
	}else{
		in.unget();
		in.clear();
		t.setTokenType(STRING);
		return t;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

id_Machine::id_Machine(){

}

////////////////////////////////////////////////////////////////////////////////////////

token id_Machine::findID(std::ifstream& in, token& t, int& ln){
	char c = in.get();
	//std::cout << "id_Machine.findID got " << c << endl;
	if( isalnum(c) ){
		updateValue(t,c);
		return findID(in,t,ln);
	}else{
		in.unget();
		in.clear();
		if( keywordCheck(t) ){
			return t;
		}else{
			t.setTokenType(ID);
			return t;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////

bool id_Machine::keywordCheck(token& t){
	if( t.getValue() == "Schemes" ){
		t.setTokenType(SCHEMES);
		return true;
	}else if(t.getValue() == "Rules"){
		t.setTokenType(RULES);
		return true;
	}else if(t.getValue() == "Queries"){
		t.setTokenType(QUERIES);
		return true;
	}else if(t.getValue() == "Facts"){
		t.setTokenType(FACTS);
		return true;
	}else{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

symbol_Machine::symbol_Machine(){
	symboltypes[','] = COMMA; symboltypes['.'] = PERIOD;symboltypes['?'] = Q_MARK;
	symboltypes['('] = LEFT_PAREN; symboltypes[')'] = RIGHT_PAREN; symboltypes[':'] = COLON;
	symboltypes['*'] = MULTIPLY; symboltypes['+'] = ADD;
	symboltypes['\n'] = WHITESPACE;
	symboltypes['\t'] = WHITESPACE;
	symboltypes[' '] = WHITESPACE;
	symboltypes['\v'] = WHITESPACE;
	symboltypes['\f'] = WHITESPACE;
	symboltypes['\r'] = WHITESPACE;
	symboltypes[EOF] = ENDOFFILE;

}

////////////////////////////////////////////////////////////////////////////////////////

token symbol_Machine::findSymbol(std::ifstream& in, char c, token& t, int& ln){
	std::map<char,tokentype>::iterator it;
	//std::cout << "symbol_Machine.findSymbol with " << c << endl;
	it = symboltypes.find(c);

	if( it != symboltypes.end() ){
		if( symboltypes[c] == WHITESPACE){
			if(c=='\n'){ln++;}
			t.setTokenType(WHITESPACE);
			return t;

		}else if(c==':'){
			if( in.get()=='-'){
				t.setValue(":-");
				t.setTokenType(COLON_DASH);
				return t;
			}else{
				in.unget();
				in.clear();
				t.setTokenType(COLON);
				return t;
			}

		}else{
			std::string s = "";
			s.push_back(c);
			t.setValue(s);
			t.setTokenType( (symboltypes[c]) );
			///std::cout << t.getTypeString() << t.getValue() << t.getLineNumber << endl;
			return t;
		}

	}else{
		t.setTokenType(UNDEFINED);
		return t;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

finder::finder(){
	linenumber = 0;
}

////////////////////////////////////////////////////////////////////////////////////////

int finder::getLineNumber(){
	return linenumber;
}

////////////////////////////////////////////////////////////////////////////////////////

void finder::setLineNumber(int ln){
	linenumber = ln;
}

////////////////////////////////////////////////////////////////////////////////////////

token finder::findToken(std::ifstream& input){
	//std::cout << "enter finder.findToken\n";
	char c = input.get();
	//std::cout << "findToken got " << c << endl;
	std::string s;
	token t;

	comment_Machine coM;
	string_Machine stM;
	id_Machine idM;
	symbol_Machine syM;

	t.setLineNumber(linenumber);
	t.setValue(s);

	if(c == '#'){
		s = c;
		t.setValue(s);
		t=coM.type(input,t,linenumber);
		return t;
	}else if(c == '\''){
		s.push_back(c);
		t.setValue(s);
		t=stM.findString(input,t,linenumber);
		return t;
	}else if( isalpha(c) ){
		s.push_back(c);
		t.setValue(s);
		t=idM.findID(input,t,linenumber);
		return t;
	}else if( c==EOF){
		t.setValue("");
		t.setTokenType(ENDOFFILE);
		//std::cout << "return endoffile token" << endl;
		return t;
	}else{
		s.push_back(c);
		t.setValue(s);
		t=syM.findSymbol(input,c,t,linenumber);
		return t;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

bool finder::charCompare(char a,char b){
	if(a == b){
		return true;
	}else{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////

lexer::lexer(){
	linenumber=0;
}

////////////////////////////////////////////////////////////////////////////////////////

void lexer::setLineNumber(int ln){
	linenumber = ln;
}

////////////////////////////////////////////////////////////////////////////////////////

int lexer::getLineNumber(){
	return linenumber;
}

////////////////////////////////////////////////////////////////////////////////////////

/*void analyze(string s){
	//openInput(s);
	loadList(s,tokens);
	//closeInput();
	printTokens(tokens);
	return;
}

//openInput attempts to open a file named s, then returns 'false' if input.failbit is set, and true if it's not.
bool openInput(string s){
	input.open(s);
	if(input.good()){
		//std::cout << "openInput() - input.good() == TRUE" << endl;
		setLineNumber(1);
		return true;
	}
	//std::cout << "openInput() - input.good() == FALSE" << endl;
	return false;
}

void closeInput(){
	input.close();
}*/
std::list<token> lexer::getTokenList(){
	return tokens;
}

////////////////////////////////////////////////////////////////////////////////////////

std::list<token> lexer::loadList(std::string s){
	input.open(s);
	if ( input.is_open() ){
		setLineNumber(1);
		do{
			//std::cout << "enter do loop @ loadList\n";
			finder1.setLineNumber( getLineNumber() );

			token t = finder1.findToken(input);
			if(t.getTokenType() != WHITESPACE ){
				//t.setTypeString( typekey[ t.getTokenType() ] );
				tokens.push_back( t );
			}

			setLineNumber( finder1.getLineNumber() );
		}while(!input.eof());
	}
	input.close();
	return tokens;
}

////////////////////////////////////////////////////////////////////////////////////////

void lexer::printTokens(){
	for(std::list<token>::iterator it=tokens.begin(); it != tokens.end(); it++){
		std::cout << '(' << (*it).getTypeString() << ",\"" << (*it).getValue() << "\"," << (*it).getLineNumber() << ")\n";
	}
	std::cout << "Total Tokens = " << tokens.size() << std::endl;
}
