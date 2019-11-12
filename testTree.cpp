#include "testTree.h"
#include <string>
#include <iostream>

void preorder(node_t *node, int level)
{
	if(node == NULL)
		return;
	
	string line;
		
	for(int counter = 0; counter < level; counter++)
	{
		line.append(" ");
	}
	
	line.append(node->label);
	line.append(" ");
	
	for(int counter = 0; counter < node->tokens.size(); counter++)
	{
		line.append(node->tokens[counter].desc);
		
		if( ((counter+1) != node->tokens.size()) && (node->tokens[counter].ID != OPtk))
			line.append(",");
		
		line.append(" ");
	}
	
	cout << line << "\n";
	
	preorder(node->child1, level + 1);
	preorder(node->child2, level + 1);
	preorder(node->child3, level + 1);
	preorder(node->child4, level + 1);
}
