#include "utils.h"

bool isNewline(char c) {
	return c == '\n' || c =='\r';
}

bool isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isNumeric(char c) {
	return c >= '0' && c <= '9';
}

bool isAlphabetical(char c) {
	return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
}
