#include <string>
#include <vector>
#include "compiler.h"
#include "token.h"
#include "utils.h"

using namespace std;

/*

TODO:
- Remove multi-character character constant support

*/

const string empty("");

Token makeToken(TokenType, string);
Token* makeErrorToken();

bool next(string&, int, int&, char&);
void prev(string&, int, int&, char&);
bool allowedFirstIdChar(char);
bool allowedIdChar(char);

TokenType getDirective(string);
TokenType getKwBtId(string);

int gLine;
int gCol;

int gCurLine;
int gCurCol;

void tokenize(string src, vector<Token>& tokens) {
	gLine = 1;
	gCol = 0; // Col starts at 1, but same as lexIndex initialization to -1: next increments them both at the start

	int lexIndex = -1;
	int tokenStart;
	int length = src.length();
	char c;

	bool tempb;
	string temps;
	TokenType tempt;

	while (next(src, length, lexIndex, c)) {
		tokenStart = lexIndex;
		gCurLine = gLine;
		gCurCol = gCol;

		// Whitespaces
		if (isWhitespace(c)) {
			while (next(src, length, lexIndex, c) && isWhitespace(c));
			prev(src, length, lexIndex, c);
		}

		// Both types of comments
		else if (c == '/' && lexIndex + 1 < length && (src[lexIndex + 1] == '/' || src[lexIndex + 1] == '*')) {
			next(src, length, lexIndex, c); // We already explicitly checked above and know it's safe
			if (c == '/') {
				while (next(src, length, lexIndex, c) && !isNewline(c));
			} else { // We can only enter this if the next char was '/' or '*', so we know for certain this else means "/*"
				tokenStart = 1; // Using tokenStart as a temp integer
				tempb = false;
				while (next(src, length, lexIndex, c)) {
					if (c == '/' && next(src, length, lexIndex, c) && c == '*') {
						++tokenStart;
					} else if (c == '*' && next(src, length, lexIndex, c) && c == '/') {
						if (--tokenStart <= 0) break;
					}
				}
			}
		}

		// Numbers
		else if (isNumeric(c) || (c == '.' && lexIndex + 1 < length && isNumeric(src[lexIndex + 1]))) {
			tempt = LITERAL_INTEGER;
			tempb = false;
			if (c == '0' && lexIndex + 1 < length && src[lexIndex + 1] == 'x') {
				next(src, length, lexIndex, c); // Skip 'x' in hex numbers (0x...)
			}

			while (true) {
				if (!next(src, length, lexIndex, c) || (!isNumeric(c) && c != '.')) {
					if (!isAlphabetical(c)) prev(src, length, lexIndex, c); // Allows literal suffixes like f for floats
					tokens.push_back(makeToken(tempt, src.substr(tokenStart, lexIndex - tokenStart + 1)));
					break;
				}

				if (c == '.') {
					if (tempt == LITERAL_FLOATING_POINT && !tempb) {
						logCompiler(ERROR, "too many decimal points in number", makeErrorToken());
						tempb = true;
					}

					tempt = LITERAL_FLOATING_POINT;
				}
			}
		}

		// Chars
		else if (c == '\'') {
			tempb = false;
			while (true) {
				if (!next(src, length, lexIndex, c) || isNewline(c)) {
					logCompiler(ERROR, "unterminated character literal", makeErrorToken());
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
				if (!next(src, length, lexIndex, c)) {
					logCompiler(ERROR, "unterminated string", makeErrorToken());
					return;
				}

				if (isNewline(c)) {
					// Once we hit a newline char (\n or \r), we negate any whitespace that follows to allow arbitrary indentation, including space indents (not just tab indents)
					while (next(src, length, lexIndex, c) && isNewline(c));
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
			if (!next(src, length, lexIndex, c) || (c != '<' && c != '=')) {
				tokens.push_back(makeToken(ANGLE_BRACKET_LEFT, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_BIN_LESS_EQUAL, empty));
			} else if (c == '<') {
				if (next(src, length, lexIndex, c) && c == '=') {
					tokens.push_back(makeToken(OP_SHIFT_LEFT_ASSIGN, empty));
				} else {
					tokens.push_back(makeToken(OP_BIN_BITWISE_SHIFT_LEFT, empty));
					prev(src, length, lexIndex, c);
				}
			}
		} else if (c == '>') {
			if (!next(src, length, lexIndex, c) || (c != '>' && c != '=')) {
				tokens.push_back(makeToken(ANGLE_BRACKET_RIGHT, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_BIN_GREATER_EQUAL, empty));
			} else if (c == '>') {
				if (next(src, length, lexIndex, c) && c == '=') {
					tokens.push_back(makeToken(OP_SHIFT_RIGHT_ASSIGN, empty));
				} else {
					tokens.push_back(makeToken(OP_BIN_BITWISE_SHIFT_RIGHT, empty));
					prev(src, length, lexIndex, c);
				}
			}
		}

		else if (c == ',') {
			tokens.push_back(makeToken(COMMA, empty));
		} else if (c == ';') {
			tokens.push_back(makeToken(SEMICOLON, empty));
		}

		else if (c == ':') {
			if (!next(src, length, lexIndex, c) || (c != ':' && c != '=')) {
				tokens.push_back(makeToken(OP_DECLARATION, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_INFERENCE, empty));
			} else if (c == ':') {
				tokens.push_back(makeToken(DOUBLE_COLON, empty));
			}
		}

		else if (c == '+') {
			if (!next(src, length, lexIndex, c) || (c != '+' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_ADD, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_ADD_ASSIGN, empty));
			} else if (c == '+') {
				tokens.push_back(makeToken(OP_UNI_INCREMENT, empty));
			}
		} else if (c == '-') {
			if (!next(src, length, lexIndex, c) || (c != '-' && c != '=' && c != '>')) {
				tokens.push_back(makeToken(OP_BIN_SUB, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_SUB_ASSIGN, empty));
			} else if (c == '-') {
				tokens.push_back(makeToken(OP_UNI_DECREMENT, empty));
			} else if (c == '>') {
				tokens.push_back(makeToken(OP_FUNC_RET, empty));
			}
		}

		else if (c == '*') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_MUL, empty));
				prev(src, length, lexIndex, c);
			} else {
				tokens.push_back(makeToken(OP_MUL_ASSIGN, empty));
			}
		} else if (c == '/') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_DIV, empty));
				prev(src, length, lexIndex, c);
			} else {
				tokens.push_back(makeToken(OP_DIV_ASSIGN, empty));
			}
		} else if (c == '%') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_MOD, empty));
				prev(src, length, lexIndex, c);
			} else {
				tokens.push_back(makeToken(OP_MOD_ASSIGN, empty));
			}
		} else if (c == '^') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_BIN_BITWISE_XOR, empty));
				prev(src, length, lexIndex, c);
			} else {
				tokens.push_back(makeToken(OP_XOR_ASSIGN, empty));
			}
		}

		else if (c == '&') {
			if (!next(src, length, lexIndex, c) || (c != '&' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_BITWISE_AND, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_AND_ASSIGN, empty));
			} else if (c == '&') {
				tokens.push_back(makeToken(OP_BIN_LOGICAL_AND, empty));
			}
		} else if (c == '|') {
			if (!next(src, length, lexIndex, c) || (c != '|' && c != '=')) {
				tokens.push_back(makeToken(OP_BIN_BITWISE_OR, empty));
				prev(src, length, lexIndex, c);
			} else if (c == '=') {
				tokens.push_back(makeToken(OP_OR_ASSIGN, empty));
			} else if (c == '|') {
				tokens.push_back(makeToken(OP_BIN_LOGICAL_OR, empty));
			}
		}

		else if (c == '=') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_ASSIGN, empty));
				prev(src, length, lexIndex, c);
			} else {
				tokens.push_back(makeToken(OP_BIN_EQUALS, empty));
			}
		} else if (c == '!') {
			if (!next(src, length, lexIndex, c) || c != '=') {
				tokens.push_back(makeToken(OP_UNI_NOT, empty));
				prev(src, length, lexIndex, c);
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
			while (next(src, length, lexIndex, c) && allowedIdChar(c));
			prev(src, length, lexIndex, c);
			if (lexIndex == tokenStart) {
				logCompiler(WARN, "empty directive does nothing", makeErrorToken());
			} else {
				temps = src.substr(tokenStart + 1, lexIndex - tokenStart);
				tempt = getDirective(temps);
				if (tempt == UNKNOWN) {
					logCompiler(WARN, ("unknown directive '#" + temps + "'").c_str(), makeErrorToken());
				} else {
					tokens.push_back(makeToken(tempt, empty));
				}
			}
		}

		// Keywords, builtin types, identifiers
		else if (allowedFirstIdChar(c)) {
			while (next(src, length, lexIndex, c) && allowedIdChar(c));
			temps = src.substr(tokenStart, lexIndex - tokenStart);
			prev(src, length, lexIndex, c);
			tempt = getKwBtId(temps);
			if (tempt == IDENTIFIER) {
				tokens.push_back(makeToken(tempt, temps));
			} else {
				tokens.push_back(makeToken(tempt, empty));
			}
		}

		else {
			logCompiler(ERROR, (string("unexpected symbol '") + c + "'").c_str(), makeErrorToken());
		}
	}
}

Token makeToken(TokenType t, string s) {
	Token token;
	token.type = t;
	token.lexeme = s;
	token.line = gCurLine;
	token.col = gCurCol;
	return token;
}

Token errorToken = makeToken(UNKNOWN, empty);
Token* makeErrorToken() {
	errorToken.line = gCurLine;
	errorToken.col = gCurCol;
	return &errorToken;
}

bool next(string& data, int length, int& index, char& c) {
	if (++index < length) {
		c = data[index];

		if (c == '\n') { // Only \n, since we don't need \r\n counting as two newlines
			++gLine;
			gCol = 0; // Line starts with col 1, just that the upcoming call to next will increment gCol to 1, reading the first character
		} else {
			++gCol;
		}

		return true;
	}
	return false;
}

void prev(string& data, int length, int& index, char& c) {
	// Main use is to also decrement line and col globals appropriately to accomodate for look ahead calls to next
	if (c == '\n') {
		// We should set gCol to 0, since next should be called right after a call to prev, incrementing gCol to 1,
		// which is good, because it's the start of the line
		--gLine;
		gCol = 0;
	} else {
		--gCol;
	}

	c = data[--index];
}

bool allowedFirstIdChar(char c) {
	return isAlphabetical(c) || c == '_';
}

bool allowedIdChar(char c) {
	return allowedFirstIdChar(c) || isNumeric(c);
}

TokenType getDirective(string s) {
	if (s == "export") return DIR_EXPORT;
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
	if (s == "new") return KW_NEW;
	if (s == "null") return KW_NULL;
	if (s == "old") return KW_OLD;
	if (s == "opaque") return KW_OPAQUE;
	if (s == "return") return KW_RETURN;
	if (s == "sizeof") return KW_SIZEOF;
	if (s == "SOA") return KW_SOA;
	if (s == "struct") return KW_STRUCT;
	if (s == "switch") return KW_SWITCH;
	if (s == "true") return KW_TRUE;
	if (s == "while") return KW_WHILE;
	
	return IDENTIFIER;
}
