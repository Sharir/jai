#ifndef _PARSER_H
#define _PARSER_H

#include <vector>
#include "token.h"

typedef struct TokenNode {
	Token data;
	std::vector<TokenNode*> children;
} TokenNode;

TokenNode* parseSyntax(std::vector<Token>&);

#endif
