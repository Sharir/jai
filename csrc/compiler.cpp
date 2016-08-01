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

// struct CompiledFile {
// 	string name;
// 	// This will be cached in gFiles below, and will be used if multiple files import it. This struct should
// 	// basically contain the symbols. Maybe also ride on this for actual compiled tree or generated code
// };

bool gFatalError = false;
stack<string> gFilesBeingCompiled;
// vector<CompiledFile> gFiles;

void compileFile(string name) {
	gFilesBeingCompiled.push(name);

	char* file = readFile(name); // Reading from the source file
	if (!file) {
		logCompiler(ERROR, ("couldn't read file " + name).c_str());
		exit(1);
	}

	string src = file;

	vector<Token> tokens;
	tokenize(src, tokens); // Lexer - converting the source file into tokens - keywords, identifiers, operators etc.
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
		printf("%s:%d %s: %s\n", gFilesBeingCompiled.top().c_str(), token->line, levelString, msg);
		// Print line segment from source file that contains token
		// Print '^' beneath the start of the token in the previous line
	} else {
		printf("Jai: %s: %s\n", levelString, msg);
	}
}

void printTokens(vector<Token>& tokens) {
	for (const Token& t : tokens) {
		printf("Token { type: %d\t\tline: %d\t\tcol: %d\t\tlexeme: '%s' }\n", t.type, t.line, t.col, t.lexeme.c_str());
	}
}
