#include <vector>
#include "compiler.h"
#include "parser.h"

using namespace std;

bool next();

TokenNode* dNoContext();

/*
Remember that the lexer naively passes:

ANGLE_BRACKET_LEFT - OP_BIN_LESS
ANGLE_BRACKET_RIGHT - OP_BIN_GREATER

OP_BIN_MUL - OP_DEREFERENCE
OP_BIN_BITWISE_AND - OP_ADDRESS

OP_DECLARATION - OP_TERNARY_COLON
DOUBLE_COLON - (OP_FUNC_DECLARATION | OP_STRUCT_DECLARATION)
*/

vector<Token> tokens;
int index;
int length;
Token t;

TokenNode* parseSyntax(vector<Token>& data) {
	tokens = data;
	index = -1;
	length = tokens.size();

	return dNoContext();
}

bool next() {
	if (++index < length) {
		t = tokens[index];
		return true;
	}
	return false;
}

TokenNode* dNoContext() {
	TokenNode* root = new TokenNode;

	if (!next()) {
		logCompiler(WARN, "empty file");
	} else if (t.type == KW_NAMESPACE) {

	}

	return root;
}
