#ifndef _PARSER_H
#define _PARSER_H

#include "token.h"
#include <list>
#include <vector>

using namespace std;

typedef struct TreeNode {
	Token data;
	list<TreeNode*> children;

	TreeNode(Token d) : data(d) {}
}TreeNode;

TreeNode* parseTokens(const vector<Token>& tokens);

#endif