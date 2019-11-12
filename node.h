#ifndef NODE_H
#define NODE_H
#include <string>
#include <set>
#include <vector>
#include "token.h"

using namespace std;

struct node_t
{
	string label;
	int tokenLength;
	int level;
	//set<string> tokens;
	
	vector<Token> tokens;
	
	struct node_t *child1, *child2, *child3, *child4;
};

#endif
