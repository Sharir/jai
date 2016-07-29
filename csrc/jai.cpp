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
	printf("Token { type: %d\tlexeme: '%s' }\n", t.type, t.lexeme.c_str());
}

string linedString(string s)
{
	int number = 1, len = s.length(), j;
	string ret;
	
	for(int i = 0 ; i < len ; i++)
	{
		ret += to_string(number) + ": ";
		
		j = i;
		while(s[j] != '\n' && j < len)
		{
			ret += s[j];
			j++;
		}
		ret += '\n';
		number++;
		i = j;
	}

	return ret;
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

	string src = readFile(mainFileName); //reading from the source file

	cout << "Source: " << endl << linedString(src) << endl;

	src = processFile(src); //preprocessor - deleting comments, converting multiline strings to one-liners and handling directives

	cout << "Preprocessed: " << endl << linedString(src) << endl;

	vector<Token> tokens;
	tokenize(src, tokens); //lexer - tokenizing the source file into tokens - keywords, identifiers, operators, whitespaces and so on

	for (const Token& t : tokens) {
		printToken(t);
	}

	return 0;
}
