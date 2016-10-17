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
TokenNode* dNoContext(bool = false);
TokenNode* dNoContextItem();
TokenNode* dGenericDecl();

TokenNode* genericArgumentList(bool = false);
TokenNode* dStructOrFuncDecl();

TokenNode* dStructDecl();
TokenNode* dStructContext();
TokenNode* dStructContextItem();
TokenNode* dStructContextDecl();
TokenNode* dStructOperatorDecl();

TokenNode* dFuncDecl();
TokenNode* dVarDecl();

TokenNode* dType();
TokenNode* dBaseType();
TokenNode* dTypeId();
TokenNode* dTypeMod();

TokenNode* dSymbol();

TokenNode* dExpression(bool = false);
TokenNode* dExprAssign();

/*
Remember that the lexer naively passes:

ANGLE_BRACKET_LEFT - OP_BIN_LESS
ANGLE_BRACKET_RIGHT - OP_BIN_GREATER

OP_BIN_MUL - OP_PTR
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

	TokenNode* root = makeNode(ROOT);

	root->children.push_back(dImports());

	TokenNode* context = dNoContext();
	root->children.push_back(context);

	if (context->children.empty()) {
		logCompiler(WARN, "empty file");
	}

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
	if (--index >= 0 && index < length) {
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
	TokenNode* imports = makeNode(DIR_IMPORT);
	Token dir;

	while (next()) {
		if (t.type == DIR_IMPORT) {
			dir = t;
			if (!next() || t.type != LITERAL_STRING) {
				logCompiler(ERROR, "#import expects a string", &dir);
				prev();
				break;
			}

			imports->children.push_back(makeNode());
		} else {
			prev();
			break;
		}
	}

	return imports;
}

TokenNode* dNoContext(bool end) {
	TokenNode* context = makeNode(LIST);

	TokenNode* item;
	while (true) {
		item = dNoContextItem();
		if (item) {
			context->children.push_back(item);
		} else if (!next()) {
			break;
		} else if (!end || t.type != BRACKET_RIGHT) { // Couldn't match a declaration, there are more tokens, and they're not the end token
			logCompiler(ERROR, "expected declaration", &t);
		} else {
			break;
		}
	}

	prev();
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
	TokenNode* out;

	if (t.type == DOUBLE_COLON) {
		TokenNode* decl = makeNode();
		out = dStructOrFuncDecl();

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
		out = dVarDecl();
		if (out && (!next() || t.type != SEMICOLON)) {
			prev();
			logCompiler(ERROR, "expected semicolon", &t);
		}

		return out;
	}
}

TokenNode* genericArgumentList() {
	TokenNode* list = makeNode(LIST);
	TokenNode* arg;
	TokenNode* type;
	Token name;

	while (true) {
		if (!next() || t.type != IDENTIFIER) {
			prev();
			break;
		}

		name = t;
		if (!next() || t.type != OP_DECLARATION) {
			logCompiler(ERROR, "expected declaration in argument list", &name);
			prev();
			break;
		}

		arg = makeNode();
		arg->children.push_back(makeNode(name));
		type = dType();

		if (!type) {
			logCompiler(ERROR, "expected type in argument list", &name);
			delete arg;
			// Not backtracking (prev) because we don't want a different area to report this same error again under
			// some unexpected symbol or something. Just continue as it's fairly obvious at this point that they
			// wanted an argument declaration and made a mistake
			break;
		}

		arg->children.push_back(type);
		list->children.push_back(arg);

		if (!next() || t.type != COMMA) {
			prev();
			break;
		}
	}

	if (list->children.empty()) {
		delete list;
		return NULL;
	}

	return list;
}

TokenNode* dStructOrFuncDecl() {
	if (!next()) return NULL;
	TokenNode* node;
	TokenNode* temp;

	if (t.type == KW_OPAQUE || t.type == KW_STRUCT) {
		if (t.type == KW_OPAQUE) {
			temp = makeNode();

			if (!next() || t.type != KW_STRUCT) {
				logCompiler(ERROR, "expected 'struct' after 'opaque'", &t);
				delete temp;
				return NULL;
			}

			node = makeNode();
			node->children.push_back(temp);
		} else {
			node = makeNode();
		}

		temp = dStructDecl();

		if (temp) {
			node->children.push_back(temp);
			return node;
		}

		delete node;
		prev();
		return NULL;
	} else {
		prev();
		return dFuncDecl();
	}
}

TokenNode* dStructDecl() {
	if (!next() || t.type != BRACKET_LEFT) {
		logCompiler(ERROR, "expected struct declaration", &t);
		return NULL;
	}

	TokenNode* node = dStructContext();
	if (node->children.empty()) {
		logCompiler(ERROR, "empty struct declaration", &t);
	}
	
	if (!next() || t.type != BRACKET_RIGHT) {
		logCompiler(ERROR, "expected '}' to close struct context", &t);
	}

	return node;
}

TokenNode* dStructContext() {
	TokenNode* context = makeNode(LIST);

	TokenNode* item;
	while (true) {
		item = dStructContextItem();
		if (item) {
			context->children.push_back(item);
		} else if (!next()) {
			break;
		} else if (t.type != BRACKET_RIGHT) { // Couldn't match a declaration, there are more tokens, and they're not the end token
			logCompiler(ERROR, "expected declaration", &t);
		} else {
			break;
		}
	}

	prev();
	return context;
}

TokenNode* dStructContextItem() {
	if (!next()) return NULL;
	TokenNode* node;

	if (t.type == IDENTIFIER) {
		Token name = t;
		node = dStructContextDecl();
		if (node) {
			node->children.insert(node->children.begin(), makeNode(name));
			return node;
		} else {
			logCompiler(ERROR, "expected declaration", &name);
		}
	} else {

	}

	return NULL;
}

TokenNode* dStructContextDecl() {
	return NULL;
}

TokenNode* dStructOperatorDecl() {
	return NULL;
}

TokenNode* dFuncDecl() {
	return makeNode(UNKNOWN);
}

TokenNode* dVarDecl() {
	return makeNode(UNKNOWN);
}

TokenNode* dType() {
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
		Token left = t;
		if (!next() || t.type != SQUARE_BRACKET_RIGHT) {
			logCompiler(ERROR, "expected ']'", &t);
			prev();
			return NULL;
		}

		return makeNode(left);
	}

	prev();
	return NULL;
}

TokenNode* dSymbol() {
	return NULL;
}

TokenNode* dExpression(bool notNull) {
	TokenNode* node = dExprAssign();

	if (!node && notNull) {
		logCompiler(ERROR, "expected expression", &t);
	}

	return node;
}

TokenNode* dExprMultiple(bool notNull = true) {
	TokenNode* node = dExpression(notNull);

	if (node) {
		if (!next() || t.type != COMMA) {
			prev();
		} else {
			TokenNode* nextExpr = makeNode();
			nextExpr->children.push_back(dExprMultiple());
			node->children.push_back(nextExpr);
		}
	}

	return node;
}

TokenNode* dExprPrimary() {
	if (!next()) return NULL;

	if (t.type == LITERAL_INTEGER || t.type == LITERAL_FLOATING_POINT || t.type == LITERAL_CHARACTER || t.type == LITERAL_STRING) {
		return makeNode();
	}

	if (t.type == PAREN_LEFT) {
		TokenNode* node = dExpression();

		// Notice how even if no expression is found, it will still complain about parentheses.
		// For the input "(", both "expected expression" and "expected a closing brace" will be printed
		if (!next() || t.type != PAREN_RIGHT) {
			prev();
			logCompiler(ERROR, "expected ')'", &t);
		}

		return node;
	}

	prev();
	return dSymbol(); // Could be null and that's fine
}

TokenNode* dPostfix() {
	if (!next()) return NULL;

	TokenNode* node = makeNode();

	if (t.type == SQUARE_BRACKET_LEFT) {
		node->children.push_back(dExpression());

		// Same deal as in dExprPrimary
		if (!next() || t.type != SQUARE_BRACKET_RIGHT) {
			prev();
			logCompiler(ERROR, "expected ']'");
		}
	} else if (t.type == PAREN_LEFT) {
		TokenNode* exprs = dExprMultiple(false); // Could be null. Don't error
		if (exprs) node->children.push_back(exprs);

		if (!next() || t.type != PAREN_RIGHT) {
			prev();
			logCompiler(ERROR, "expected ')'", &t);
		}
	} else if (t.type == OP_MEMBER_ACCESS) {
		if (!next() || t.type != IDENTIFIER) {
			prev();
			logCompiler(ERROR, "expected identifier", &t);
		} else {
			node->children.push_back(makeNode());
		}
	} else if (t.type != OP_UNI_INCREMENT && t.type != OP_UNI_DECREMENT) {
		prev();
		delete node;
		return NULL;
	}

	return node;
}

TokenNode* dExprPostfix() {
	TokenNode* node = dExprPrimary();

	// Not technically what the grammar defines, but much faster, safe (can't get into endless loop ever) and achieves the same thing
	if (node) {
		TokenNode* post;

		while (true) {
			post = dPostfix();
			if (!post) break;

			node->children.push_back(post);
		}
	}

	return node;
}

TokenNode* dOpUnary() {
	if (!next()) return NULL;

	if (t.type == OP_ADDRESS || t.type == OP_PTR ||
			t.type == OP_UNI_INCREMENT || t.type == OP_UNI_DECREMENT || t.type == OP_UNI_NOT || t.type == OP_UNI_BITWISE_NOT ||
			t.type == OP_BIN_ADD || t.type == OP_BIN_SUB) { // Basically "- <Expression>" (and "+" should also be fine)
		return makeNode();
	}

	prev();
	return NULL;
}

TokenNode* dExprUnary() {
	TokenNode* node;

	if (next() && t.type == KW_SIZEOF) {
		node = makeNode();

		if (!next() || t.type != PAREN_LEFT) {
			prev();
			logCompiler(ERROR, "expected '('", &t);
		} else {
			TokenNode* type = dType();
			if (!type) {
				logCompiler(ERROR, "expected type to sizeof", &t);
			} else {
				node->children.push_back(type);

				if (!next() || t.type != PAREN_RIGHT) {
					prev();
					logCompiler(ERROR, "expected ')'", &t);
				}
			}
		}

		return node;
	}

	prev();

	TokenNode* op = dOpUnary();
	node = dExprPostfix();

	// If we consumed an OpUnary here, it couldn't have been part of a different branch, and we must error.
	// If we wouldn't error here then it would be possible to have an OpUnary in source code and it wouldn't
	// be consumed by anything, in situations where Expression (or ExprMultiple) gets notNull=false.
	// For example, consider this: "my_func(-)". This is incorrect syntax, but unless we report it here,
	// it would pass as normal. In this case we would get an "expected expression" pointed at the ')'.
	// In a case where we didn't consume an OpUnary (op == null), we can let the call hierarchy decide if it
	// expected an expression or not
	if (op) {
		if (!node) {
			logCompiler(ERROR, "expected expression", &t);
		} else {
			op->children.push_back(node);
		}

		return op;
	}

	return node;
}

TokenNode* dExprCast() {
	TokenNode* node;

	if (next() && t.type == KW_CAST) {
		node = makeNode();

		if (!next() || t.type != PAREN_LEFT) {
			prev();
			logCompiler(ERROR, "expected '('", &t);
		} else {
			TokenNode* type = dType();
			if (!type) {
				logCompiler(ERROR, "expected type for cast", &t);
			} else {
				node->children.push_back(type);

				if (!next() || t.type != PAREN_RIGHT) {
					prev();
					logCompiler(ERROR, "expected ')'", &t);
				} else {
					TokenNode* toCast = dExprCast();
					if (!toCast) {
						prev();
						logCompiler(ERROR, "expected expression to cast", &t);
					} else {
						type->children.push_back(toCast);
					}
				}
			}
		}

		return node;
	}

	prev();
	return dExprUnary();
}

TokenNode* dExprArith1() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprCast();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_MUL && t.type != OP_BIN_DIV && t.type != OP_BIN_MOD)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprCast();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprArith2() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprArith1();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_ADD && t.type != OP_BIN_SUB)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprArith1();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprShift() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprArith2();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_BITWISE_SHIFT_LEFT && t.type != OP_BIN_BITWISE_SHIFT_RIGHT)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprArith2();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprAnd() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprShift();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_BITWISE_AND)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprShift();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprXor() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprAnd();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_BITWISE_XOR)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprAnd();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprOr() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprXor();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_BITWISE_OR)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprXor();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprComparison() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprOr();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_LESS && t.type != OP_BIN_GREATER &&
				t.type != OP_BIN_LESS_EQUAL && t.type != OP_BIN_GREATER_EQUAL &&
				t.type != OP_BIN_EQUALS && t.type != OP_BIN_NOT_EQUALS)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprOr();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprLogicAnd() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprComparison();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_LOGICAL_AND)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprComparison();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprLogicOr() {
	TokenNode* top;
	TokenNode* temp;

	top = dExprLogicAnd();
	if (!top) return NULL;

	while (true) {
		if (!next() || (t.type != OP_BIN_LOGICAL_OR)) {
			prev();
			break;
		}

		temp = makeNode();
		temp->children.push_back(top);
		top = temp;

		temp = dExprLogicAnd();
		if (temp) {
			top->children.push_back(temp);
		} else {
			logCompiler(ERROR, "expected expression operand", &t);
		}
	}

	return top;
}

TokenNode* dExprTernary() {
	TokenNode* node = dExprLogicOr();
	if (!node) return NULL;

	if (next() && t.type == OP_TERNARY_QUESTION) {
		TokenNode* ternary = makeNode();
		ternary->children.push_back(node);
		ternary->children.push_back(dExprTernary());

		if (!next() || t.type != OP_DECLARATION) {
			prev();
			logCompiler(ERROR, "expected ':' for ternary expression", &t);
		} else {
			ternary->children.push_back(dExprTernary());
		}

		return ternary;
	}

	prev();
	return node;
}

TokenNode* dExprAssign() {
	TokenNode* node = dExprTernary();
	if (!node) return NULL;

	if (next() && (t.type == OP_ASSIGN || t.type == OP_ADD_ASSIGN || t.type == OP_SUB_ASSIGN ||
			t.type == OP_MUL_ASSIGN || t.type == OP_DIV_ASSIGN || t.type == OP_MOD_ASSIGN ||
			t.type == OP_XOR_ASSIGN || t.type == OP_AND_ASSIGN || t.type == OP_OR_ASSIGN ||
			t.type == OP_SHIFT_LEFT_ASSIGN || t.type == OP_SHIFT_RIGHT_ASSIGN)) {
		TokenNode* opAssign = makeNode();

		opAssign->children.push_back(node);
		opAssign->children.push_back(dExprAssign());

		return opAssign;
	}

	prev();
	return node;
}
