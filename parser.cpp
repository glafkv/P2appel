#include "parser.h"
#include "token.h"
#include "scanner.h"
#include "node.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Token tk;
static string expectedToken;
static Token EMPTY_TOKEN;

node_t *parser()
{
	//description for empty token
	EMPTY_TOKEN.desc = "EMPTY";
	
	//get the first token
	scanner(tk);
	
	//create the root node
	node_t *root = NULL;
	
	root = program();

	//see if the last token is EOF
	if(tk.ID == EOFtk)
	{
		cout << "Parse OK\n";
		return root;
	}
	else
	{
		expectedToken.assign("EOF");
		parserError();
	}
}
//for the <program> -> <vars> <block>
node_t *program()
{
	//create the node
	node_t *node = createNode("<program>");
	
	if(keywordMap[tk.desc] == "VARtk" || keywordMap[tk.desc] == "STARTtk")
	{
		//child1 of program is <vars> and child2 is <block>
		node->child1 = vars();
		node->child2 = block();
		return node;
	}
	else
	{
		expectedToken.assign("VARtk or STARTtk");
		parserError();
	}
}
//for <block> -> start <vars><stats> stop
node_t *block()
{
	node_t *node = createNode("<block>");
	if(keywordMap[tk.desc] == "STARTtk")
	{
		scanner(tk);
		node->child1 = vars();
		node->child2 = stats();
		if(keywordMap[tk.desc] == "STOPtk")
		{
			scanner(tk);
			return node;
		}
		else
		{
			expectedToken.assign("STOPtk");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("STARTtk");
		parserError();
	}
}
//for the <vars>->empty | var Identifier : Integer <vars>
node_t *vars()
{
	node_t *node = createNode("<vars>");
	if(keywordMap[tk.desc] == "VARtk")
	{
		scanner(tk);
		if(tk.ID == IDtk)
		{
			node->tokens.push_back(tk);
			scanner(tk);
			if(operatorMap[tk.desc] == "COLONtk")
			{
				scanner(tk);
				if(tk.ID == INTtk)
				{
					node->tokens.push_back(tk);
					scanner(tk);
					node->child1 = vars();
					return node;
				}
				else
				{
					expectedToken.assign("INTtk");
					parserError();
				}
			}
			else 
			{
				expectedToken.assign("COLONtk");
				parserError();
			}
		}
		else
		{
			expectedToken.assign("IDtk");
			parserError();
		}
	}
	else
	{
		node->tokens.push_back(EMPTY_TOKEN);
		return node;
	}
}

//for the <expr> -> <A> + <expr> | <A>
node_t *expr()
{
	node_t *node = createNode("<expr>");
	node->child1 = A();
	if(operatorMap[tk.desc] == "PLUStk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		node->child1 = expr();
		return node;
	}
	else
	{
		expectedToken.assign("PLUStk");
		parserError();
	}
	return node;
}

//for the <A> -> <N> - <A> | <N>
node_t *A()
{
	node_t *node = createNode("<A>");
	
	node->child1 = N();
	
	if(operatorMap[tk.desc] == "MINUStk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		node->child2 = A();
		return node;
	}
	else
	{
		expectedToken.assign("MINUStk");
		parserError();
	}
	return node;
}		
//for the <N> -> <M> / <N> | <M> * <N> | <M>
node_t *N()
{
	node_t *node = createNode("<N>");
	
	node->child1 = M();
	
	if(operatorMap[tk.desc] == "SLASHtk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		node->child2 = N();
		return node;
	}
	else if(operatorMap[tk.desc] == "ASTERIKtk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		node->child2 = N();
		return node;
	}
	else
	{
		expectedToken.assign("SLASHtk or ASTERIKtk");
		parserError();
	}
	return node;
}
//for the <M> -> -<M> | <R>
node_t *M()
{
	node_t *node = createNode("<M>");
	if(operatorMap[tk.desc] == "MINUStk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		node->child1 = M();
		return node;
	}
	else
	{
		expectedToken.assign("MINUStk");
		parserError();
	}
	node->child1 = R();
	return node;
}
//for the <R> -> [<expr>] | Identifier | Integer
node_t *R()
{
	node_t *node = createNode("<R>");
	if(operatorMap[tk.desc] == "LEFTBRACKETtk")
	{
		scanner(tk);
		node->child1 = expr();
		if(operatorMap[tk.desc] == "RIGHTBRACKETtk")
		{
			scanner(tk);
			return node;
		}
		else
		{
			expectedToken.assign("RIGHTBRACKETtk");
			parserError();
		}
	}
	else if(tk.ID == IDtk)
	{
		node->tokens.push_back(tk);
		scanner(tk);
		return node;
	}
	else if(tk.ID == INTtk)
	{
		node->tokens.push_back(tk);
		scanner(tk);
		return node;
	}
	else
	{
		expectedToken.assign("LEFTBRACKETtk or IDtk or INTtk");
		parserError();
	}
}
//for the <stats> -> <stat> ; <mStat>
node_t *stats()
{
	node_t *node = createNode("<stats>");
	if(keywordMap[tk.desc] == "INtk" || keywordMap[tk.desc] == "OUTtk" || keywordMap[tk.desc] == "STARTtk" || keywordMap[tk.desc] == "CONDtk" || keywordMap[tk.desc] == "ITERtk" || tk.ID == IDtk)
	{
		node->child1 = stat();
		if(operatorMap[tk.desc] == "SEMICOLONtk")
		{
			scanner(tk);
		}
		else
		{
			expectedToken.assign("SEMICOLONtk");
			parserError();
		}
		node->child2 = mStat();
		return node;
	}
	else
	{
		expectedToken.assign("INtk or OUTtk or STARTtk or CONDtk or ITERtk or IDtk");
		parserError();
	}
}
//for <mStat> -> empty | <stat> ; <mStat>
node_t *mStat()
{
	node_t *node = createNode("<mStat>");
	if(keywordMap[tk.desc] == "INtk" || keywordMap[tk.desc] == "OUTtk" || keywordMap[tk.desc] == "STARTtk" || keywordMap[tk.desc] == "CONDtk" || keywordMap[tk.desc] == "ITERtk" || tk.ID == IDtk)
	{
		node->child1 = stat();
		if(operatorMap[tk.desc] == "SEMICOLONtk")
		{
			scanner(tk);
		}
		else
		{
			expectedToken.assign("SEMICOLONtk");
			parserError();
		}
		node->child2 = mStat();
		return node;
	}
	else
	{
		node->tokens.push_back(EMPTY_TOKEN);
		return node;
	}
}

//for <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
node_t *stat()
{
	node_t *node = createNode("<stat>");
	if(keywordMap[tk.desc] == "INtk")
	{
		node->child1 = in();
		return node;
	}
	else if(keywordMap[tk.desc] == "OUTtk")
	{
		node->child1 = out();
		return node;
	}	
	else if(keywordMap[tk.desc] == "STARTtk")
	{
		node->child1 = block();
		return node;
	}
	else if(keywordMap[tk.desc] == "CONDtk")
	{
		node->child1 = cond();
		return node;
	}
	else if(keywordMap[tk.desc] == "ITERtk")
	{
		node->child1 = loop();
		return node;
	}
	else if(tk.ID == IDtk)
	{
		node->child1 = assign();
		return node;
	}
	else
	{
		expectedToken.assign("INtk or OUTtk or STARTtk or CONDtk or ITERtk or IDtk");
		parserError();
	}
}

//for <in> -> in Identifier
node_t *in()
{
	//create the node
	node_t *node = createNode("<in>");
	
	if(keywordMap[tk.desc] == "INtk")
	{
		//get the next token
		scanner(tk);
		if(tk.ID == IDtk)
		{
			node->tokens.push_back(tk);
			scanner(tk);
			return node;
		}
		else
		{
			expectedToken.assign("IDtk");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("INtk");
		parserError();
	}

}
//for <out> -> out<expr>
node_t *out()
{
	//create the node
	node_t *node = createNode("<out>");
	if(keywordMap[tk.desc] == "OUTtk")
	{	
		scanner(tk);
		node->child1 = expr();
		return node;
	}
	else
	{
		expectedToken.assign("OUTtk");
		parserError();
	}
}
//for the <if> -> cond((<expr><RO><expr>))<stat>
node_t *cond()
{
	//create node
	node_t *node = createNode("<if>");
	
	//check if the keyword token is cond
	if(keywordMap[tk.desc] == "CONDtk")
	{
		//get next token
		scanner(tk);
		if(operatorMap[tk.desc] == "LEFTPARENtk")
		{
			//get next token
			scanner(tk);
			if(operatorMap[tk.desc] == "LEFTPARENtk")
			{
				//get next token
				scanner(tk);
				node->child1 = expr();
				node->child2 = RO();
				node->child3 = expr();
				if(operatorMap[tk.desc] == "RIGHTPARENTk")
				{
					//get next token
					scanner(tk);
					if(operatorMap[tk.desc] == "RIGHTPARENtk")
					{
						scanner(tk);
						node->child4 = stat();
						return node;
					}
					else{
						expectedToken.assign("RIGHTPARENtk");
						parserError();
					}
				}
				else{
					expectedToken.assign("RIGHTPARENtk");
					parserError();
				}
			}
			else
			{
				expectedToken.assign("LEFTPARENtk");
				parserError();
			}
		}
		else{
			expectedToken.assign("LEFTPARENtk");
			parserError();
		}
	}
	else{
		expectedToken.assign("CONDtk");
		parserError();
	}
		
}
//for <loop> -> iter((<expr><RO><expr>)) <stat>
node_t *loop()
{
	//create the node
	node_t *node = createNode("<loop>");
	
	//check for iter token
	if(keywordMap[tk.desc] == "ITERtk")
	{
		//get next token
		scanner(tk);
		if(operatorMap[tk.desc] == "LEFTPARENtk")
		{
			scanner(tk);
			if(operatorMap[tk.desc] == "LEFTPARENtk")
			{
				scanner(tk);
				node->child1 = expr();
				node->child2 = RO();
				node->child3 = expr();
				if(operatorMap[tk.desc] == "RIGHTPARENtk")
				{
					scanner(tk);
					if(operatorMap[tk.desc] == "RIGHTPARENtk")
					{
						scanner(tk);
						node->child4 = stat();
						return node;
					}
					else{
						expectedToken.assign("RIGHTPARENtk");
						parserError();
					}
				}
				else{
					expectedToken.assign("RIGHTPARENtk");
					parserError();
				}
			}
			else{
				expectedToken.assign("LEFTPARENtk");
				parserError();
			}
		}
		else{
			expectedToken.assign("LEFTPARENtk");
			parserError();
		}
	}
	else{
		expectedToken.assign("ITERtk");
		parserError();
	}

}
//for the <assign> -> Identifier < < <expr>
node_t *assign()
{
	//create node
	node_t *node = createNode("<assign>");
	//check if ID tk
	if(tk.ID == IDtk)
	{
		node->tokens.push_back(tk);
		scanner(tk);
		if(operatorMap[tk.desc] == "LESSTHANtk")
		{
			node->tokens.push_back(tk);
			scanner(tk);
			if(operatorMap[tk.desc] == "LESSTHANtk")
			{
				node->tokens.push_back(tk);
				scanner(tk);
				node->child1 = expr();
				return node;
			}
			else{
				expectedToken.assign("LESSTHANtk");
				parserError();
			}
		}
		else{
			expectedToken.assign("LESSTHANtk");
			parserError();
		}
	}
	else{
		expectedToken.assign("IDtk");
		parserError();
	}		
}
//for <RO> -> < | < < | > | > > | = | < >
node_t *RO()
{
	//create node
	node_t *node = createNode("<RO>");
	
	if(operatorMap[tk.desc] == "LESSTHANtk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		if(operatorMap[tk.desc] == "LESSTHANtk")
		{
			node->tokens.push_back(tk);
			scanner(tk);
			return node;
		}
		else if(operatorMap[tk.desc] == "GREATERTHANtk")
		{
			node->tokens.push_back(tk);
			scanner(tk);
			return node;
		}
	}
	else if(operatorMap[tk.desc] == "GREATERTHANtk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		if(operatorMap[tk.desc] == "GREATERTHANtk")
		{
			node->tokens.push_back(tk);
			scanner(tk);
			return node;
		}
	}
	else if(operatorMap[tk.desc] == "EQUALStk")
	{
		node->tokens.push_back(tk);
		scanner(tk);
		return node;
	}
	else 
	{
		expectedToken.assign("LESSTHANtk or GREATERTHANtk or EQUALStk");
		parserError();
	}	
	

}
//error
void parserError()
{
	cout << "Parser error on line number " << tk.lineNumber << ": expected '" << expectedToken << "' but received '" << tk.desc << "'\n";
	
	exit(EXIT_FAILURE);
}
//create node
node_t *createNode(string productionName)
{
	node_t *node = new node_t();
	node->child1 = NULL;
	node->child2 = NULL;
	node->child3 = NULL;
	node->child4 = NULL;
	
	node->label = productionName;
	
	return node;
}
