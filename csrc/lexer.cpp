#include <regex>
#include <string>
#include "lexer.h"
#include "token.h"

using namespace std;

bool isNewline(char);
bool isWhitespace(char);

void tokenize(string data, vector<Token>& tokens) {
	int lexIndex = 0;
	int length = data.length();
	char c;

	int tokenStart = lexIndex;
	TokenType runningType = UNKNOWN;

	for (; lexIndex < length; ++lexIndex) {
		c = data[lexIndex];

		// Whitespaces
		if (runningType != LITERAL_STRING && isWhitespace(c)) {
			runningType = WHITESPACE;
			continue;
		} else if (runningType == WHITESPACE) {
			Token next;
			next.type = WHITESPACE;
			next.lexeme = data.substr(tokenStart, lexIndex - tokenStart);
			tokens.push_back(next);
			tokenStart = lexIndex;
			runningType = UNKNOWN;
		}

		// Strings
		if (c == '"') {
			if (runningType == UNKNOWN) {
				runningType = LITERAL_STRING;
				continue;
			} else {
				Token next;
				next.type = LITERAL_STRING;
				next.lexeme = data.substr(tokenStart, lexIndex - tokenStart + 1);
				tokens.push_back(next);
				tokenStart = lexIndex + 1;
				runningType = UNKNOWN;
				continue;
			}
		} else if (runningType == LITERAL_STRING) {
			if (isNewline(c) || lexIndex + 1 == length) {
				printf("Syntax Error: unterminated string\n");
			}
			continue;
		}

		// If after all the checks the type is still undetermined, I think it's a syntax error
		if (runningType == UNKNOWN) {
			printf("Syntax Error: unexpected symbol '%c'\n", c);
			// Syntax error. Continuing for testing purposes

			runningType = UNKNOWN;
			tokenStart = lexIndex + 1;
		}
	}
}

bool isNewline(char c) {
	return c == '\n' || c =='\r';
}

bool isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
