#include <vector>
#include "compiler.h"
#include "parser.h"
#include "token.h"

using namespace std;

bool next();
void prev();
TokenNode* makeNode();
TokenNode* makeNode(Token);
TokenNode* makeNode(TokenType);

TokenNode* dImports();
TokenNode* dNoContext();
TokenNode* dNoContextItem();
TokenNode* dGenericDecl();
//namespace

TokenNode* dType();
TokenNode* dTypeAllowRef();
TokenNode* dBaseType();
TokenNode* dTypeId();
TokenNode* dTypeMod();
TokenNode* dTypeModRef();

TokenNode* dStructOrFuncDecl();
TokenNode* dStructDecl();
TokenNode* dFuncDecl();
TokenNode* dVarDecl();

/*
Remember that the lexer naively passes:

ANGLE_BRACKET_LEFT - OP_BIN_LESS
ANGLE_BRACKET_RIGHT - OP_BIN_GREATER

OP_BIN_MUL - OP_PTR
OP_BIN_BITWISE_AND - OP_ADDRESS
OP_FUNC_RET - OP_PTR_MEMBER_ACCESS ?

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

	TokenNode* root = makeNode(ROOT);

	TokenNode* node = dImports();
	if (node) root->children.push_back(node);

	node = dNoContext();
	if (node) root->children.push_back(node);

	return root;
}

bool next() {
	if (++index < length) {
		t = tokens[index];
		return true;
	}
	return false;
}

void prev() {
	if (--index >= 0) {
		t = tokens[index];
	}
}

TokenNode* makeNode() {
	return makeNode(t);
}
TokenNode* makeNode(Token token) {
	TokenNode* node = new TokenNode;
	node->data = token;
	return node;
}
TokenNode* makeNode(TokenType type) {
	Token t;
	t.type = type;
	t.lexeme = "";
	t.line = -1;
	t.col = -1;
	return makeNode(t);
}

TokenNode* dImports() {
	if (!next()) {
		logCompiler(WARN, "empty file");
	} else if (t.type == DIR_IMPORT) {
		TokenNode* node = makeNode();

		if (!next() || t.type != LITERAL_STRING) {
			logCompiler(ERROR, "#import expects a string", &node->data);
		} else {
			node->children.push_back(makeNode());
			TokenNode* out = dImports();
			if (out) node->children.push_back(out);
			return node;
		}
	}

	prev();
	return NULL;
}

TokenNode* dNoContext() {
	TokenNode* context = makeNode(NO_CONTEXT);

	TokenNode* item;
	while (true) {
		item = dNoContextItem();
		if (item) {
			context->children.push_back(item);
		} else {
			break;
		}
	}

	return context;
}

TokenNode* dNoContextItem() {
	if (!next()) return NULL;
	TokenNode* node;

	if (t.type == IDENTIFIER) {
		Token name = t;
		node = dGenericDecl();
		if (node) {
			node->children.insert(node->children.begin(), makeNode(name));
			return node;
		} else {
			logCompiler(ERROR, "expected declaration", &name);
		}
	}

	return NULL;
}

TokenNode* dGenericDecl() {
	if (!next()) return NULL;

	if (t.type == DOUBLE_COLON) {
		TokenNode* decl = makeNode();
		TokenNode* out = dStructOrFuncDecl();

		if (out) {
			decl->data.type = out->data.type == KW_STRUCT ? OP_STRUCT_DECLARATION : OP_FUNC_DECLARATION;
			decl->children.push_back(out);
			return decl;
		} else {
			delete decl;
			prev();
			return NULL;
		}
	} else {
		prev();
		return dVarDecl();
	}
}

TokenNode* dType() {
	return NULL;
}

TokenNode* dTypeAllowRef() {
	return NULL;
}

TokenNode* dBaseType() {
	return NULL;
}

TokenNode* dTypeId() {
	return NULL;
}

TokenNode* dTypeMod() {
	if (!next()) return NULL;

	if (t.type == OP_BIN_MUL) {
		t.type = OP_PTR;
		return makeNode();
	} else if (t.type == SQUARE_BRACKET_LEFT) {
		if (!next() || t.type != SQUARE_BRACKET_RIGHT) {
			logCompiler(ERROR, "expected ']'", &t);
			prev();
			return NULL;
		}


	}

	prev();
	return NULL;
}

TokenNode* dTypeModRef() {
	if (!next()) return NULL;

	if (t.type == OP_BIN_BITWISE_AND) {
		t.type = OP_ADDRESS;
		return makeNode();
	}

	prev();
	return NULL;
}

TokenNode* dStructOrFuncDecl() {
	if (!next()) return NULL;

	if (t.type == KW_STRUCT) {
		TokenNode* node = makeNode();
		TokenNode* out = dStructDecl();

		if (out) {
			node->children.push_back(out);
			return node;
		} else {
			delete node;
			prev();
			return NULL;
		}
	} else {
		prev();
		return dFuncDecl();
	}
}

TokenNode* dStructDecl() {
	return makeNode(UNKNOWN);
}

TokenNode* dFuncDecl() {
	return makeNode(UNKNOWN);
}

TokenNode* dVarDecl() {
	return makeNode(UNKNOWN);
}
