#ifndef _PARSER_H
#define _PARSER_H

#define IS_FLAG_SET(a, b) ((a & b) != 0)

#include <list>
#include <vector>

#include "token.h"

using namespace std;

typedef struct TreeNode {
	Token data;
	list<TreeNode*> children;

	TreeNode(Token d) : data(d) {}
}TreeNode;

int most_significant_bit_set(int num);
TreeNode* parseTokens(const vector<Token>& tokens);

#endif