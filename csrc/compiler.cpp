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
	gFilesBeingCompiled.push(name);

	char* file = readFile(name); // Reading from the source file
	if (!file) {
		logCompiler(ERROR, ("couldn't read file '" + name + "'").c_str());
		exit(1);
	}

	CompiledFile* cf = new CompiledFile;
	cf->name = name;
	cf->src = file;
	gFiles.push_back(cf);

	vector<Token> tokens;
	tokenize(cf->src, tokens); // Lexer - converting the source file into tokens - keywords, identifiers, operators etc.
	if (gFatalError) exit(1);

	printTokens(tokens);

	// How I imagine the compiler pipeline right now:

	// Read file

	// Collect symbols - lex, parse, extract symbols from tree [ namespaces, functions, structs, globals ]

	// Preprocess - imports, ?macros?, other directives
	// Last part of preprocessor - #run - may add new source - consider if feasible and preferrable to add directly
	// to fully lexed and parsed, instead of to source, and then invoking lexer and parser again
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

	if (token) {
		string filename = gFilesBeingCompiled.top();
		printf("%s:%d:%d %s: %s\n", filename.c_str(), token->line, token->col, levelString, msg);
		printSrcLineSegment(filename, token->line, token->col);
	} else {
		printf("jai: %s: %s\n", levelString, msg);
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
	if (length <= 0 || col <= 0 || col >= length) return;

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
