#ifndef _LEXER_H
#define _LEXER_H

#include <vector>
#include <string>

enum TokenType {
	UNKNOWN = 0,
	WHITESPACE = 1
};

typedef struct {
	TokenType type;
	std::string lexeme;
} Token;

void tokenize(std::string, std::vector<Token>&);

#endif
