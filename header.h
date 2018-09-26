#ifndef HEADER_H
#define HEADER_H

#include <string>
#include <map>

enum tokentype    {
        COMMA, PERIOD, Q_MARK,
        LEFT_PAREN, RIGHT_PAREN, COLON,
        COLON_DASH, MULTIPLY, ADD,
        SCHEMES, FACTS, RULES,
    	QUERIES, ID, STRING,
		COMMENT, WHITESPACE, UNDEFINED, ENDOFFILE
};

std::string typeToString(tokentype t);


#endif
