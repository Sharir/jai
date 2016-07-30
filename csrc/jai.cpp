#include <iostream>
#include <string>
#include <vector>
#include "os.h"
#include "file.h"
#include "preprocessor.h"
#include "lexer.h"
#include "parser.h"

using namespace std;

void printToken(const Token& t) {
	printf("Token { type: %d\t\tline: %d\t\tcol: %d\t\tlexeme: '%s' }\n", t.type, t.line, t.col, t.lexeme.c_str());
}

int main(int argc, char** argv) {
	string* p_mainFileName = NULL;
	
	// parse cli
	for (int i = 1; i < argc; i++) {
		if (!p_mainFileName) {
			p_mainFileName = new string(argv[i]);
		} else {
			printf("Please specify only one main file\n");
			return 1;
		}
	}

	if (!p_mainFileName) {
		printf("Please specify a main file\n");
		return 1;
	}

	string mainFileName = *p_mainFileName;
	delete p_mainFileName;

	// decide output filename
#ifdef OS_WIN
	int index = mainFileName.rfind("\\");
#else
	int index = mainFileName.rfind("/");
#endif

	string outFileName = index < 0 ? mainFileName : mainFileName.substr(index + 1, mainFileName.length() - index - 1);
	index = outFileName.rfind(".");
	outFileName = index < 0 ? outFileName + '_' : outFileName.substr(0, index);

#ifdef OS_WIN
	outFileName += ".exe";
#endif

	char* file = readFile(mainFileName); // Reading from the source file
	if (!file) {
		printf("Error: file '%s' doesn't exist\n", mainFileName.c_str());
		return 1;
	}

	string src = file;

	src = processFile(src); // Preprocessor - handling directives

	vector<Token> tokens;
	tokenize(src, tokens); // Lexer - converting the source file into tokens - keywords, identifiers, operators etc.

	for (const Token& t : tokens) {
		printToken(t);
	}

	return 0;
}
