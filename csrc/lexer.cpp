#include <string>
#include <vector>
#include "token.h"
#include "utils.h"

using namespace std;

/*

TODO:
- Add binary, octal and hex integer literals (consider floats)
- Add token line and col indices

- Tell off Tzah for:
	- Including stuff in the header file instead of the source file (algorithm in preproc)
	- Putting "private" function declarations in the header file (count_occurrences, prepare - in preproc)
	- Explicitly using cpp iterators - much better to use builtin syntax or managed functions

*/

string empty("");

Token makeToken(TokenType, string, int = -1, int = -1);
bool safeGetNext(string&, int, int&, char&);
bool allowedFirstIdChar(char);
bool allowedIdChar(char);

TokenType getDirective(string);
TokenType getKwBtId(string);

int gLine;
int gCol;

void tokenize(string src, vector<Token>& tokens) {
	int lexIndex = -1;
	int tokenStart;
	int length = src.length();
	char c;

	bool tempb;
	string temps;
	TokenType tempt;

	while (safeGetNext(src, length, lexIndex, c)) {
		tokenStart = lexIndex;

		// Whitespaces
		if (isWhitespace(c)) {
			while (safeGetNext(src, length, lexIndex, c) && isWhitespace(c));
			--lexIndex;
		}

		// Both types of comments
		else if (c == '/' && lexIndex + 1 < length && (src[lexIndex + 1] == '/' || src[lexIndex + 1] == '*')) {
			safeGetNext(src, length, lexIndex, c); // We already explicitly checked above and know it's safe
			if (c == '/') {
				while (safeGetNext(src, length, lexIndex, c) && !isNewline(c));
			} else { // We can only enter this if the next char was '/' or '*', so we know for certain this else means "/*"
				tokenStart = 1; // Using tokenStart as a temp integer
				tempb = false;
				while (safeGetNext(src, length, lexIndex, c)) {
					if (c == '/' && safeGetNext(src, length, lexIndex, c) && c == '*') {
						++tokenStart;
					} else if (c == '*' && safeGetNext(src, length, lexIndex, c) && c == '/') {
						if (--tokenStart <= 0) break;
					}
				}
			}
		}

		// Numbers
		else if (isNumeric(c)) {
			tempb = false;
			while (true) {
				if (!safeGetNext(src, length, lexIndex, c)) return;
				if (isNewline(c)) return;

				if (c == '.') {
					if (tempb) {
						printf("Syntax Error: too many decimal points in number\n");
						break;
					}

					tempb = true;
				} else if (!isNumeric(c)) {
					if (!isAlphabetical(c)) --lexIndex; // Allows literal suffixes like f for floats
					tokens.push_back(makeToken(tempb ? LITERAL_FLOATING_POINT : LITERAL_INTEGER, src.substr(tokenStart, lexIndex - tokenStart + 1)));
					break;
				}
			}
		}

		// Chars
		else if (c == '\'') {
			tempb = false;
			while (true) {
				if (!safeGetNext(src, length, lexIndex, c)) {
					printf("Syntax Error: unterminated character literal\n");
					return;
				}

				if (isNewline(c)) {
					printf("Syntax Error: unterminated character literal\n");
					break;
				}

				if (c == '\\' && !tempb) {
					tempb = true;
				} else if (c == '\'' && !tempb) {
					tokens.push_back(makeToken(LITERAL_CHARACTER, src.substr(tokenStart + 1, lexIndex - tokenStart - 1)));
					break;
				} else if (tempb) {
					tempb = false;
				}
			}
		}

		// Strings
		else if (c == '"') {
			tempb = false;
			temps = "";
			while (true) {
				if (!safeGetNext(src, length, lexIndex, c)) {
					printf("Syntax Error: unterminated string\n");
					return;
				}

				if (isNewline(c)) {
					// Once we hit a newline char (\n or \r), we negate any whitespace that follows to allow arbitrary indentation, including space indents (not just tab indents)
					while (safeGetNext(src, length, lexIndex, c) && isWhitespace(c));
				}

				if (c == '\\' && !tempb) {
					tempb = true;
				} else if (c == '"' && !tempb) {
					tokens.push_back(makeToken(LITERAL_STRING, temps));
					break;
				} else if (tempb) {
					tempb = false;
				}

				temps += c;
			}
		}

		else if (c == '(') {
			tokens.push_back(makeToken(PAREN_LEFT, empty));
		} else if (c == ')') {
			tokens.push_back(makeToken(PAREN_RIGHT, empty));
		} else if (c == '{') {
			tokens.push_back(makeToken(BRACKET_LEFT, empty));
		} else if (c == '}') {
			tokens.push_back(makeToken(BRACKET_RIGHT, empty));
		} else if (c == '[') {
			tokens.push_back(makeToken(SQUARE_BRACKET_LEFT, empty));
		} else if (c == ']') {
			tokens.push_back(makeToken(SQUARE_BRACKET_RIGHT, empty));
		}

		else if (c == '<') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '<' && c != '=')) {
				tokens.push_back(makeToken(ANGLE_BRACKET_LEFT, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_BIN_LESS_EQUAL, empty));
			} else if (c == '<') {
				if (safeGetNext(src, length, lexIndex, c) && c == '=') {
					tokens.push_back(makeToken(OP_SHIFT_LEFT_ASSIGN, empty));
				} else {
					tokens.push_back(makeToken(OP_BIN_BITWISE_SHIFT_LEFT, empty));
					--lexIndex;
				}
			}
		} else if (c == '>') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '>' && c != '=')) {
				tokens.push_back(makeToken(ANGLE_BRACKET_RIGHT, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_BIN_GREATER_EQUAL, empty));
			} else if (c == '>') {
				if (safeGetNext(src, length, lexIndex, c) && c == '=') {
					tokens.push_back(makeToken(OP_SHIFT_RIGHT_ASSIGN, empty));
				} else {
					tokens.push_back(makeToken(OP_BIN_BITWISE_SHIFT_RIGHT, empty));
					--lexIndex;
				}
			}
		}

		else if (c == ',') {
			tokens.push_back(makeToken(COMMA, empty));
		} else if (c == ';') {
			tokens.push_back(makeToken(SEMICOLON, empty));
		}

		else if (c == ':') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != ':' && c != '=')) {
				tokens.push_back(makeToken(OP_DECLARATION, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_INFERENCE, empty));
			} else if (c == ':') {
				tokens.push_back(makeToken(DOUBLE_COLON, empty));
			}
		}

		else if (c == '+') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '+' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_ADD, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_ADD_ASSIGN, empty));
			} else if (c == '+') {
				tokens.push_back(makeToken(OP_UNI_INCREMENT, empty));
			}
		} else if (c == '-') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '-' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_SUB, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_SUB_ASSIGN, empty));
			} else if (c == '-') {
				tokens.push_back(makeToken(OP_UNI_DECREMENT, empty));
			}
		}

		else if (c == '*') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_MUL, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_MUL_ASSIGN, empty));
			}
		} else if (c == '/') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_DIV, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_DIV_ASSIGN, empty));
			}
		} else if (c == '%') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_MOD, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_MOD_ASSIGN, empty));
			}
		} else if (c == '^') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_BITWISE_XOR, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_XOR_ASSIGN, empty));
			}
		}

		else if (c == '&') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '&' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_BITWISE_AND, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_AND_ASSIGN, empty));
			} else if (c == '&') {
				tokens.push_back(makeToken(OP_BIN_LOGICAL_AND, empty));
			}
		} else if (c == '|') {
			if (!safeGetNext(src, length, lexIndex, c) || (c != '|' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_BITWISE_OR, empty));
				--lexIndex;
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_OR_ASSIGN, empty));
			} else if (c == '|') {
				tokens.push_back(makeToken(OP_BIN_LOGICAL_OR, empty));
			}
		}

		else if (c == '=') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_ASSIGN, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_BIN_EQUALS, empty));
			}
		} else if (c == '!') {
			if (!safeGetNext(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_UNI_NOT, empty));
				--lexIndex;
			} else {
				tokens.push_back(makeToken(OP_BIN_NOT_EQUALS, empty));
			}
		}

		else if (c == '?') {
			tokens.push_back(makeToken(OP_TERNARY_QUESTION, empty));
		} else if (c == '~') {
			tokens.push_back(makeToken(OP_UNI_BITWISE_NOT, empty));
		} else if (c == '.') {
			tokens.push_back(makeToken(OP_MEMBER_ACCESS, empty));
		}

		// Directives
		else if (c == '#') {
			while (safeGetNext(src, length, lexIndex, c) && allowedIdChar(c));
			--lexIndex;
			if (lexIndex == tokenStart) {
				printf("Syntax Error: empty directive not allowed\n");
			} else {
				temps = src.substr(tokenStart + 1, lexIndex - tokenStart);
				tempt = getDirective(temps);
				if (tempt == UNKNOWN) {
					printf("Syntax Error: unknown directive '#%s'\n", temps.c_str());
				} else {
					tokens.push_back(makeToken(tempt, empty));
				}
			}
		}

		// Keywords, builtin types, identifiers
		else if (allowedFirstIdChar(c)) {
			while (safeGetNext(src, length, lexIndex, c) && allowedIdChar(c));
			temps = src.substr(tokenStart, lexIndex - tokenStart);
			--lexIndex;
			tempt = getKwBtId(temps);
			if (tempt == IDENTIFIER) {
				tokens.push_back(makeToken(tempt, temps));
			} else {
				tokens.push_back(makeToken(tempt, empty));
			}
		}

		else {
			printf("Syntax Error: unexpected symbol '%c'\n", c);
		}
	}
}

Token makeToken(TokenType t, string s, int line, int col) {
	Token token;
	token.type = t;
	token.lexeme = s;
	token.line = line < 0 ? gLine : line;
	token.col = col < 0 ? gCol : col;
	return token;
}

bool safeGetNext(string& data, int length, int& index, char& c) {
	if (++index < length) {
		c = data[index];
		return true;
	}
	return false;
}

bool allowedFirstIdChar(char c) {
	return isAlphabetical(c) || c == '_';
}

bool allowedIdChar(char c) {
	return allowedFirstIdChar(c) || isNumeric(c);
}

TokenType getDirective(string s) {
	if (s == "import") return DIR_IMPORT;
	if (s == "run") return DIR_RUN;
	return UNKNOWN;
}

TokenType getKwBtId(string s) {
	if (s == "i8" || s == "byte") return BT_INT8;
	if (s == "i16" || s == "short") return BT_INT16;
	if (s == "i32" || s == "int") return BT_INT32;
	if (s == "i64" || s == "long") return BT_INT64;

	if (s == "u8") return BT_UINT8;
	if (s == "u16") return BT_UINT16;
	if (s == "u32") return BT_UINT32;
	if (s == "u64") return BT_UINT64;

	if (s == "float") return BT_FLOAT;
	if (s == "double") return BT_DOUBLE;

	if (s == "bool") return BT_BOOL;
	if (s == "char") return BT_CHAR;
	if (s == "string") return BT_STRING;
	if (s == "void") return BT_VOID;
	// if (s == "any") return BY_ANY;

	if (s == "break") return KW_BREAK;
	if (s == "case") return KW_CASE;
	if (s == "cast") return KW_CAST;
	if (s == "const") return KW_CONST;
	if (s == "continue") return KW_CONTINUE;
	if (s == "default") return KW_DEFAULT;
	if (s == "delete") return KW_DELETE;
	if (s == "else") return KW_ELSE;
	if (s == "enum") return KW_ENUM;
	if (s == "false") return KW_FALSE;
	if (s == "for") return KW_FOR;
	if (s == "if") return KW_IF;
	if (s == "namespace") return KW_NAMESPACE;
	if (s == "new") return KW_NEW;
	if (s == "null") return KW_NULL;
	if (s == "return") return KW_RETURN;
	if (s == "sizeof") return KW_SIZEOF;
	if (s == "SOA") return KW_SOA;
	if (s == "struct") return KW_STRUCT;
	if (s == "switch") return KW_SWITCH;
	// if (s == "this") return KW_THIS;
	if (s == "true") return KW_TRUE;
	if (s == "while") return KW_WHILE;
	
	return IDENTIFIER;
}

