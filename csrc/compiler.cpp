#include <cstdlib>
#include <stack>
#include <string>
#include <vector>
#include "compiler.h"
#include "file.h"
#include "lexer.h"
#include "parser.h"
#include "preprocessor.h"
#include "token.h"

using namespace std;

void printTokens(vector<Token>&);
const char* tokenTypeStr(TokenType);
void printTree(const TokenNode*, int = 0);

void printSrcLineSegment(string&, int, int);

struct CompiledFile {
	string name;
	string src;
	// This will be cached in gFiles below, and will be used if multiple files import it. This struct should
	// basically contain the symbols. Maybe also ride on this for actual compiled tree or generated code
};

bool gFatalError = false;
stack<string> gFilesBeingCompiled;
vector<CompiledFile*> gFiles;

void compileFile(string name) {
	for (const CompiledFile* otherCf : gFiles) {
		// Requires proper path based comparison
		if (otherCf->name == name) {
			return;
		}
	}

	gFilesBeingCompiled.push(name);

	char* file = readFile(name); // Reading from the source file
	if (!file) {
		logCompiler(ERROR, "couldn't read file");
		exit(1);
	}

	CompiledFile* cf = new CompiledFile;
	cf->name = name;
	cf->src = file;
	gFiles.push_back(cf);

	vector<Token> tokens;
	tokenize(cf->src, tokens); // Lexer - converting the source file into tokens - keywords, identifiers, operators etc.
	if (gFatalError) return;

	// printTokens(tokens);

	TokenNode* parsed = parseSyntax(tokens);
	if (gFatalError) return;

	printTree(parsed);

	gFilesBeingCompiled.pop();

	// How I imagine the compiler pipeline right now:

	// Read file

	// Collect symbols - lex, parse, extract symbols from tree [ functions, structs, globals ]

	// Preprocess - imports, other directives
	// Last part of preprocessor - #run - may add new source - consider if feasible and preferrable to add directly
	// to fully lexed and parsed tree, instead of to source, and then invoking lexer and parser again
	// If (probably preferrable) adding directly to tree, bear in mind to make some flag that a token is a product
	// of #run, for compiler logging purposes. Line and col should probably be those of the #run directive itself

	// Compile - lex, parse, ?optimize?, generate
	// Might be able to avoid lexing and parsing a second time
}

void logCompiler(LogLevel level, const char* msg, Token* token) {
	const char* levelString;

	switch (level) {
		case DEBUG:
#ifdef COMPILER_DEBUG
			levelString = "DEBUG";
			break;
#else
			return;
#endif

		case INFO:
			levelString = "info";
			break;
		case WARN:
			levelString = "warning";
			break;
		case ERROR:
			gFatalError = true;
			levelString = "error";
			break;
	}

	string filename = gFilesBeingCompiled.top();
	if (filename.empty()) {
		filename = "jai";
	}

	if (token) {
		printf("%s:%d:%d %s: %s\n", filename.c_str(), token->line, token->col, levelString, msg);
		printSrcLineSegment(filename, token->line, token->col);
	} else {
		printf("%s: %s: %s\n", filename.c_str(), levelString, msg);
	}
}

#define MAX_COL_MARGIN 50
void printSrcLineSegment(string& name, int line, int col) {
	// Print line segment from source file that contains token
	// Print '^' beneath the start of the token in the previous line

	string src;
	for (const CompiledFile* cf : gFiles) {
		if (cf->name == name) {
			src = cf->src;
		}
	}

	if (src.empty()) return;

	int newlineCount = 1; // Line index starts from 1 for humans
	int index = -1;
	int length = src.length();
	while (++index < length && newlineCount < line) {
		if (src[index] == '\n') { // Only \n, because \r\n should count as one newline
			++newlineCount;
		}
	}

	if (index >= length) return;

	// length will temporarily act as the index of the end of the line
	length = src.find('\n', index + 1);
	if (length == (int)string::npos) length = src.length();
	if (src[length - 1] == '\r') --length;

	// Neglect indentation at start of line for error reporting
	while (src[index] == '\t' || src[index] == ' ') {
		++index;
		--col;
	}

	// Now length is the actual length of the line segment
	length = length - index;
	if (length <= 0 || col <= 0 || col > length) return;

	// Ensure no more than MAX_COL_MARGIN characters before and after the col position, for long lines
	if (col > MAX_COL_MARGIN) {
		index += col - MAX_COL_MARGIN;
		length -= col - MAX_COL_MARGIN;
		col = MAX_COL_MARGIN;
	}

	if (length - col > MAX_COL_MARGIN) {
		length = col + MAX_COL_MARGIN;
	}

	src = src.substr(index, length); // Only the relevant line segment
	
	printf("\t%s\n\t", src.c_str());
	while (--col) {
		printf(" ");
	}
	printf("^\n");
}

void printTokens(vector<Token>& tokens) {
	for (const Token& t : tokens) {
		printf("Token { type: %d\t\tline: %d\t\tcol: %d\t\tlexeme: '%s' }\n", t.type, t.line, t.col, t.lexeme.c_str());
	}
}

const char* tokenTypeStr(TokenType t) {
	switch (t) {
		case UNKNOWN: return "UNKNOWN";
		case WHITESPACE: return "WHITESPACE";
		case LITERAL_INTEGER: return "LITERAL_INTEGER";
		case LITERAL_FLOATING_POINT: return "LITERAL_FLOATING_POINT";
		case LITERAL_CHARACTER: return "LITERAL_CHARACTER";
		case LITERAL_STRING: return "LITERAL_STRING";
		case PAREN_LEFT: return "PAREN_LEFT";
		case PAREN_RIGHT: return "PAREN_RIGHT";
		case BRACKET_LEFT: return "BRACKET_LEFT";
		case BRACKET_RIGHT: return "BRACKET_RIGHT";
		case SQUARE_BRACKET_LEFT: return "SQUARE_BRACKET_LEFT";
		case SQUARE_BRACKET_RIGHT: return "SQUARE_BRACKET_RIGHT";
		case ANGLE_BRACKET_LEFT: return "ANGLE_BRACKET_LEFT";
		case ANGLE_BRACKET_RIGHT: return "ANGLE_BRACKET_RIGHT";
		case COMMA: return "COMMA";
		case SEMICOLON: return "SEMICOLON";
		case DOUBLE_COLON: return "DOUBLE_COLON";
		case IDENTIFIER: return "IDENTIFIER";
		case OP_DECLARATION: return "OP_DECLARATION";
		case OP_INFERENCE: return "OP_INFERENCE";
		case OP_FUNC_DECLARATION: return "OP_FUNC_DECLARATION";
		case OP_STRUCT_DECLARATION: return "OP_STRUCT_DECLARATION";
		case OP_FUNC_RET: return "OP_FUNC_RET";
		case OP_BIN_ADD: return "OP_BIN_ADD";
		case OP_BIN_SUB: return "OP_BIN_SUB";
		case OP_BIN_MUL: return "OP_BIN_MUL";
		case OP_BIN_DIV: return "OP_BIN_DIV";
		case OP_BIN_MOD: return "OP_BIN_MOD";
		case OP_BIN_BITWISE_XOR: return "OP_BIN_BITWISE_XOR";
		case OP_BIN_BITWISE_AND: return "OP_BIN_BITWISE_AND";
		case OP_BIN_BITWISE_OR: return "OP_BIN_BITWISE_OR";
		case OP_BIN_BITWISE_SHIFT_LEFT: return "OP_BIN_BITWISE_SHIFT_LEFT";
		case OP_BIN_BITWISE_SHIFT_RIGHT: return "OP_BIN_BITWISE_SHIFT_RIGHT";
		case OP_BIN_LOGICAL_AND: return "OP_BIN_LOGICAL_AND";
		case OP_BIN_LOGICAL_OR: return "OP_BIN_LOGICAL_OR";
		case OP_BIN_EQUALS: return "OP_BIN_EQUALS";
		case OP_BIN_NOT_EQUALS: return "OP_BIN_NOT_EQUALS";
		case OP_BIN_LESS: return "OP_BIN_LESS";
		case OP_BIN_LESS_EQUAL: return "OP_BIN_LESS_EQUAL";
		case OP_BIN_GREATER: return "OP_BIN_GREATER";
		case OP_BIN_GREATER_EQUAL: return "OP_BIN_GREATER_EQUAL";
		case OP_ASSIGN: return "OP_ASSIGN";
		case OP_ADD_ASSIGN: return "OP_ADD_ASSIGN";
		case OP_SUB_ASSIGN: return "OP_SUB_ASSIGN";
		case OP_MUL_ASSIGN: return "OP_MUL_ASSIGN";
		case OP_DIV_ASSIGN: return "OP_DIV_ASSIGN";
		case OP_MOD_ASSIGN: return "OP_MOD_ASSIGN";
		case OP_XOR_ASSIGN: return "OP_XOR_ASSIGN";
		case OP_AND_ASSIGN: return "OP_AND_ASSIGN";
		case OP_OR_ASSIGN: return "OP_OR_ASSIGN";
		case OP_SHIFT_LEFT_ASSIGN: return "OP_SHIFT_LEFT_ASSIGN";
		case OP_SHIFT_RIGHT_ASSIGN: return "OP_SHIFT_RIGHT_ASSIGN";
		case OP_TERNARY_QUESTION: return "OP_TERNARY_QUESTION";
		case OP_TERNARY_COLON: return "OP_TERNARY_COLON";
		case OP_UNI_INCREMENT: return "OP_UNI_INCREMENT";
		case OP_UNI_DECREMENT: return "OP_UNI_DECREMENT";
		case OP_UNI_NOT: return "OP_UNI_NOT";
		case OP_UNI_BITWISE_NOT: return "OP_UNI_BITWISE_NOT";
		case OP_ADDRESS: return "OP_ADDRESS";
		case OP_PTR: return "OP_PTR";
		case OP_MEMBER_ACCESS: return "OP_MEMBER_ACCESS";
		case KW_BREAK: return "KW_BREAK";
		case KW_CASE: return "KW_CASE";
		case KW_CAST: return "KW_CAST";
		case KW_CONST: return "KW_CONST";
		case KW_CONTINUE: return "KW_CONTINUE";
		case KW_DEFAULT: return "KW_DEFAULT";
		case KW_DELETE: return "KW_DELETE";
		case KW_ELSE: return "KW_ELSE";
		case KW_ENUM: return "KW_ENUM";
		case KW_FALSE: return "KW_FALSE";
		case KW_FOR: return "KW_FOR";
		case KW_IF: return "KW_IF";
		case KW_NEW: return "KW_NEW";
		case KW_NULL: return "KW_NULL";
		case KW_OLD: return "KW_OLD";
		case KW_OPAQUE: return "KW_OPAQUE";
		case KW_RETURN: return "KW_RETURN";
		case KW_SIZEOF: return "KW_SIZEOF";
		case KW_SOA: return "KW_SOA";
		case KW_STRUCT: return "KW_STRUCT";
		case KW_SWITCH: return "KW_SWITCH";
		case KW_TRUE: return "KW_TRUE";
		case KW_WHILE: return "KW_WHILE";
		case BT_INT8: return "BT_INT8";
		case BT_INT16: return "BT_INT16";
		case BT_INT32: return "BT_INT32";
		case BT_INT64: return "BT_INT64";
		case BT_UINT8: return "BT_UINT8";
		case BT_UINT16: return "BT_UINT16";
		case BT_UINT32: return "BT_UINT32";
		case BT_UINT64: return "BT_UINT64";
		case BT_FLOAT: return "BT_FLOAT";
		case BT_DOUBLE: return "BT_DOUBLE";
		case BT_BOOL: return "BT_BOOL";
		case BT_CHAR: return "BT_CHAR";
		case BT_STRING: return "BT_STRING";
		case BT_VOID: return "BT_VOID";
		case DIR_EXPORT: return "DIR_EXPORT";
		case DIR_IMPORT: return "DIR_IMPORT";
		case DIR_RUN: return "DIR_RUN";
		case ROOT: return "ROOT";
		case LIST: return "LIST";
	}

	return NULL;
}

void printTree(const TokenNode* tree, int depth) {
	for (int i = 0; i < depth; ++i) {
		printf("\t");
	}

	printf("%s    '%s'\n", tokenTypeStr(tree->data.type), tree->data.lexeme.c_str());

	for (const TokenNode* child : tree->children) {
		printTree(child, depth + 1);
	}
}
