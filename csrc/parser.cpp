#include "parser.h"

using namespace std;

int most_significant_bit_set(int num)
{
	int n = 1 << 30;

	while((num & n) != 0)
		n = n >> 1;
	
	return n;
}

/*

Remember that the lexer naively passes:

ANGLE_BRACKET_LEFT - OP_BIN_LESS
ANGLE_BRACKET_RIGHT - OP_BIN_GREATER

OP_BIN_MUL - OP_DEREFERENCE
OP_BIN_BITWISE_AND - OP_ADDRESS

OP_DECLARATION - OP_TERNARY_COLON
DOUBLE_COLON - (OP_FUNC_DECLARATION | OP_STRUCT_DECLARATION)

*/

int parseTokens(const vector<Token>& tokens, TreeNode* tree)
{
	int msb = 0;
	int type = 0;
	int size = tokens.size();

	for (int i = 0 ; i < size ; i++)
	{
		type = tokens[i].type;
	}

	return 0; //for now
}
