#include <iostream>
#include <string>
#include <vector>
#include "file.h"
#include "lexer.h"
#include "os.h"

using namespace std;

void printToken(const Token& t) {
<<<<<<< Updated upstream
	if(t.lexeme == "\n")
	{
		printf("Token { type: %d\tlexeme: '\\n' }\n", t.type, t.lexeme.c_str());
		return;
	}
	printf("Token { type: %d\tlexeme: '%s' }\n", t.type, t.lexeme.c_str());
=======
	printf("Token { type: %d\tlexeme: '", t.type);
	string s(t.lexeme);
	for(std::string::iterator it = s.begin() ; it != s.end() ; it++)
	{
		if(it[0] == '\n')
		{
			putchar('\\');
			putchar('n');
		}
		else if(it[0] == '\t')
		{
			putchar('\\');
			putchar('t');
		}
		else
			putchar(it[0]);
	}
	
	printf("' }\n");
	//	%s' }\n", t.type, t.lexeme.replace(t.lexeme.begin(), t.lexeme.end(), "\n" "\\n").c_str());
>>>>>>> Stashed changes
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

	string data = readFile(mainFileName);
	vector<Token> tokens;
	tokenize(data, tokens);

	for (const Token& t : tokens) {
		printToken(t);
	}
	
	return 0;
}
