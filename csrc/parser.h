#ifndef _PARSER_H
#define _PARSER_H

#define IS_FLAG_SET(a, b) ((a & b) != 0)

#include <list>
#include <vector>

#include "token.h"

typedef struct TreeNode {
	Token data;
	std::list<TreeNode*> children;

	TreeNode(Token d) : data(d) {}
}TreeNode;
/*
int most_significant_bit_set(int num);
int parseTokens(const vector<Token>& tokens, TreeNode* tree);
*/
#endif
