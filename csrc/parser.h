#ifndef _PARSER_H
#define _PARSER_H

#define IS_FLAG_SET(a, b) ((a | b) == a)

#include "token.h"
#include <list>
#include <vector>

using namespace std;

typedef struct TreeNode {
	Token data;
	list<TreeNode*> children;

	TreeNode(Token d) : data(d) {}
}TreeNode;

int most_significant_bit_set(int num);
TreeNode* parseTokens(const vector<Token>& tokens);

#endif