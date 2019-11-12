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

node_t* parser()
{
	//description of empty token
	EMPTY_TOKEN.desc = "EMPTY";
	
	//get the first token from scanner
	scanner(tk);
	
	//create the root node
	node_t *root = NULL;
	
	//assign the root node
	root = program();
	
	//check the last token of the program to see if its EOF
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
//for the <program> -> <vars> <block> production
node_t *program()
{
	//create the node for the program
	node_t *node = createNode("<program>");	
	
	
	//get token from scanner
	scanner(tk);
	//the child1 of <program> is <vars> and the child2 of <program> is <block>
	node->child1 = vars();
	node->child2 = block();
	return node;
	
}
//for the <vars> -> empty | var Identifier : Integer <vars> production
node_t *vars()
{

	//create the node associated with the <vars> production
	node_t *node = createNode("<vars>");
	
	//check the token is the let keyword
	if((tk.ID == KEYWORDtk) && (keywordMap[tk.desc] == "VARtk"))
	{
		//get the next token
		scanner(tk);
		
		//check if its an ID
		if(tk.ID == IDtk)
		{
			//store ID in node's vector
			node->tokens.push_back(tk);
			
			//get the next token
			scanner(tk);
		
			//check if is the = operator
			if((tk.ID == OPtk) && (operatorMap[tk.desc] == "COLONtk"))
			{
				//get the next token
				scanner(tk);
				
				//check if its an integer
				if(tk.ID == INTtk)
				{
					//store ID in node's vector
					node->tokens.push_back(tk);
			
					//get the next token
					scanner(tk);

					//the child1 of this node will be <vars>
					node->child1 = vars();
					
					return node;
				}
				else
				{
					expectedToken.assign("Integer");
					parserError();
				}
			}
			else
			{
				expectedToken.assign(":");
				parserError();
			}
		}
		else
		{
			expectedToken.assign("Identifier");
			parserError();
		}
	}
	else
	{
		//an empty produciton
		node->tokens.push_back(EMPTY_TOKEN);
		return node;
	}
}
//for the <block> -> start <vars> <stats> stop production
node_t *block()
{
	//create the node for the <block> production
	node_t *node = createNode("<block>");
		
	//check if the token is begin
	if((tk.ID == KEYWORDtk) && (keywordMap[tk.desc] == "STARTtk"))
	{
		//get the next token
		scanner(tk);
		
		//invoke <vars> and set child1 to node returned by vars
		node->child1 = vars();
		
		//invoke <stats> and set child2 to node returned by stats
		node->child2 = stats();
		
		//check if the token is the end keyword
		if((tk.ID == KEYWORDtk) && (keywordMap[tk.desc] == "STOPtk"))
		{
			//get next token
			scanner(tk);
			return node;
		}
		else
		{
			expectedToken.assign("stop");
			parserError();
		}
	}
	else 
	{
		expectedToken.assign("start");
		parserError();
	}
}
//for <expr> -> <A> + <expr> | <A>
node_t *expr()
{
	//create node
	node_t *node = createNode("<expr>");
	
	//invoke A() and set child1 to the node returned by <A>
	node->child1 = A();
	
	//check if the next token is an operator
	if(tk.ID == OPtk)
	{
		if(operatorMap[tk.desc] == "PLUStk")
		{
			//predicts the expr -> <A> + <expr> and store the + operator
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			//set the child2 to node of expr
			node->child2 = expr();
		}
	}
	return node;
}
//for <A> -> <N> - <A> | <N>
node_t *A()
{
	//create node
	node_t *node = createNode("<A>");
	
	//invoke N() and set child1 to node returned by <N>
	node->child1 = N();
	
	//check if next token is an operator
	if(tk.ID == OPtk)
	{
		if(operatorMap[tk.desc] == "MINUStk")
		{
			//predicts the <A> -> <N> - <A> and store the operator
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			//set the child2 to the node of A
			node->child2 = A();
		}
	}
	return node;
}	
//for the <N> -> <M> / <N> | <M> * <N> | <M>
node_t *N()
{
	node_t *node = createNode("<N>");
	
	//invoke M() and set child1 to the node returned by M
	node->child1 = M();
	
	//check if the token returned is an operator
	if(tk.ID == OPtk)
	{
		//check if operator is / or *
		if(operatorMap[tk.desc] == "SLASHtk")
		{
			//store the /
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			//set chil2 node to node returned by N
			node->child2 = N();
		}
		else if(operatorMap[tk.desc] == "ASTERIKtk")
		{
			//store *
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
	
			//set child2 to node returned by N
			node->child2 = N();
		}
	}
	return node;
}

//for the <M> -> -<M> | <R> production
node_t *M()
{
	//create the node
	node_t *node = createNode("<M>");
	
	//check for - 
	if((tk.ID == OPtk) && (operatorMap[tk.desc] == "MINUStk"))
	{
		//store in the <M> prodcution
		node->tokens.push_back(tk);
		
		//get the next token
		scanner(tk);
		
		//set child1 to the node returned by <M>
		node->child1 = M();
		
		return node;
	}

	//set child1 to the node returned by <R>
	node->child1 = R();	
	return node;
}
//for the <R> -> [<expr>] | Identifier | integer production
node_t *R()
{
	//create node
	node_t *node = createNode("<R>");
	
	//check if ( is there
	if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LEFTBRACKETtk"))
	{
		//get the next token
		scanner(tk);
		
		//set child1 to node returned by <expr>
		node->child1 = expr();
	
		//check if the ) token is there
		if((tk.ID == OPtk) && (operatorMap[tk.desc] == "RIGHTBRACKETtk"))
		{
			//get the next token
			scanner(tk);
			return node;
		}
		else
		{
			expectedToken.assign("]");
			parserError();
		}
	}
	else if(tk.ID == IDtk)
	{
		//if its an ID store it
		node->tokens.push_back(tk);
		
		//get the next token
		scanner(tk);
		return node;
	}
	else
	{	
		expectedToken.assign("[ or Identifier or Integer");
		parserError();
	}
}
//for the <stats> -> <stat> <mStat> production
node_t *stats()
{
	//create the node
	node_t *node = createNode("<stats>");
	
	//invoke <stat><mStat> and set child 1 and child2 to the nodes returns
	node->child1 = stat();
	//check if the token returned is an operator
	if(tk.ID == OPtk)
	{
		if(operatorMap[tk.desc] == "SEMICOLONtk")
		{
			//store the ; 
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			
			node->child2 = mStat();
		}
	}
	return node;
}
//for the <mStat> -> empty | <stat>;<mStat> production
node_t *mStat()
{
	//create the node
	node_t *node = createNode("<mStat>");
	
	//invoke stat() mStat() or empty
	if( ((tk.ID == KEYWORDtk) && ( (keywordMap[tk.desc] == "PRINTtk") || (keywordMap[tk.desc] == "READtk") || (keywordMap[tk.desc] == "BEGINtk") || (keywordMap[tk.desc] == "CONDtk") || (keywordMap[tk.desc] == "ITERtk"))) || (tk.ID == IDtk))
	{
		//set the nodes
		node->child1 = stat();
		//check if the token returned it an operator
		if(tk.ID == OPtk)
		{
			if(operatorMap[tk.desc] == "SEMICOLONtk")
			{
				//store the ;
				node->tokens.push_back(tk);
				//get the next token
				scanner(tk);
				node->child2 = mStat();
				return node;
			}
		}
	}
	else
	{
		//empty prodcution
		node->tokens.push_back(EMPTY_TOKEN);
		return node;
	}
}
//for the <stat> -> <in> | <out> | <block> | <if> | <loop> | <assign>
node_t *stat()
{
	//create node
	node_t *node = createNode("<stat>");
	
	//predicts read,print,cond,iter,or assign
	if(tk.ID == KEYWORDtk)
	{
		if(keywordMap[tk.desc] == "PRINTtk")
		{
			//get next token
			scanner(tk);
			
			//set the child1 to the <out> production
			node->child1 = out();
			return node;
		}
		else if(keywordMap[tk.desc] == "READtk")
		{
			//get next token
			scanner(tk);
			
			//set child1 to the <in> production
			node->child1 = in();
			return node;
		}
		else if(keywordMap[tk.desc] == "STARTtk")
		{
			//scanner(tk);
			//set child1 to the <block> production
			node->child1 = block();
			return node;
		}
		else if(keywordMap[tk.desc] == "CONDtk")
		{
			//get next token
			scanner(tk);
			
			//set child1 to the <if> production
			node->child1 = cond();
			return node;
		}
		else if(keywordMap[tk.desc] == "ITERtk")
		{
			//get next token
			scanner(tk);
			
			//set child1 to the <loop> production
			node->child1 = loop();
			return node;
		}
	}
	else if(tk.ID == IDtk)
	{
		//save the id in a temp token
		Token tempToken = tk;
		
		//get next token
		scanner(tk);
		
		//set child1 to the <assign> production
		node->child1 = assign();
	
		//push ID token
		node->child1->tokens.push_back(tempToken);
		return node;
	}
	else
	{
		expectedToken.assign("Read or Print or Cond or Iter or Identifier");
		parserError();
	}
}
//for the <in> -> in Identifier production
node_t *in()
{
	//create the node
	node_t *node = createNode("<in>");

	
	if(tk.ID == IDtk)
	{
		//store the node
		node->tokens.push_back(tk);
		//get the next token
		scanner(tk);
	}
	else
	{
		expectedToken.assign("Identifier");
		parserError();
	}
}
//for the <out> -> out<expr> production
node_t *out()
{
	//create the node
	node_t *node = createNode("<out>");
	
	//invoke <out> and set to child1
	node->child1 = expr();
}
//for the <if> -> cond(<expr><RO><expr>) <stat>
node_t *cond()
{
	//create the node
	node_t *node = createNode("<if>");
	
	//check if the token is (
	if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LEFTPARENtk"))
	{
		//get the next token
		scanner(tk);
		
		//invoke and set nodes
		node->child1 = expr();
		node->child2 = RO();
		node->child3 = expr();
		
		//check if the next token is )
		if((tk.ID == OPtk) && (operatorMap[tk.desc] == "RIGHTPARENtk"))
		{
			//get the next token
			scanner(tk);
		
			//invoke stat and set child4
			node->child4 = stat();
			
			return node;
		}
		else
		{
			expectedToken.assign(")");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("(");
		parserError();
	}
}
//for the <loop> -> iter(<expr><RO><expr>) <stat>
node_t *loop()
{
	//create the node
	node_t *node = createNode("<loop>");
	
	//check if the token is (
	if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LEFTPARENtk"))
	{
		//get the next token
		scanner(tk);
		
		//invoke and set the nodes
		node->child1 = expr();
		node->child2 = RO();
		node->child3 = expr();
		
		//check if the next node is )
		if((tk.ID == OPtk) && (operatorMap[tk.desc] == "RIGHTPARENtk"))
		{
			//get next token
			scanner(tk);
			
			//invoke and set child4
			node->child4 = stat();
			return node;
		}
		else
		{
			expectedToken.assign(")");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("(");
		parserError();
	}
}
//for the assign -> Identifier < < <expr>
node_t *assign()
{
	//create the node
	node_t *node = createNode("<assign>");
	
	//check if the next token is <
	if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LESSTHANtk"))
	{
	
		
		//get next token
		scanner(tk);
		
		if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LESSTHANtk"))
		{
			//get the next token
			scanner(tk);
			//invoke and set child1
			node->child1 = expr();
			return node;
		}
		else
		{
			expectedToken.assign("<");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("<");
		parserError();
	}
	
}
//for the <RO> -> < | < < | > | > > |  = | < >
node_t *RO()
{
	//creat the node
	node_t *node = createNode("<RO>");
	
	//check if the token returned is an operator
	if(tk.ID == OPtk)
	{
		if(operatorMap[tk.desc] == "LESSTHANtk")
		{
			//insert the token 
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			
			//check if its an operator and =
			if((tk.ID == OPtk) && (operatorMap[tk.desc] == "LESSTHANtk"))
			{
				//insert the = 
				node->tokens.push_back(tk);
				//get the next token
				scanner(tk);
				return node;
			}
			else if((tk.ID == OPtk) && (operatorMap[tk.desc] != "LESSTHANtk"))
			{
				expectedToken.assign("< <");
				parserError();
			}
			else
			{
				return node;
			}
		}
		else if(operatorMap[tk.desc] == "GREATERTHANtk")
		{
			//insert the token
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			
			//check if its the = op
			if((tk.ID == OPtk) && (operatorMap[tk.desc] == "GREATERTHANtk"))
			{
				//insert the node
				node->tokens.push_back(tk);
				//get the next token
				scanner(tk);
				return node;
			}
			else if((tk.ID == OPtk) && (operatorMap[tk.desc] != "GREATERTHANtk"))
			{
				expectedToken.assign("> >");
				parserError();
			}
			else
			{
				return node;
			}
		}
		else if(operatorMap[tk.desc] == "EQUALStk")
		{
			//insert the token
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			
			
			if((tk.ID == OPtk) && (operatorMap[tk.desc] != "EQUALStk"))
			{
				expectedToken.assign("=");
				parserError();
			}
			else
			{
				return node;
			}
		}
		else if(operatorMap[tk.desc] == "LESSTHANtk")
		{
			//insert the token
			node->tokens.push_back(tk);
			//get the next token
			scanner(tk);
			//check if the next one is >
			if((tk.ID == OPtk) && (operatorMap[tk.desc] == "GREATERTHANtk"))
			{
				//inser the the > token
				node->tokens.push_back(tk);
				//get the next token
				scanner(tk);
				return node;
			}
			else if((tk.ID == OPtk) && (operatorMap[tk.desc] != "GREATERTHANtk"))
			{
				expectedToken.assign("< >");
				parserError();
			}
			else
			{
				return node;
			}
		}
		else
		{
			expectedToken.assign("< or < < or > or > > or = or < > ");
			parserError();
		}
	}
	else
	{
		expectedToken.assign("< or < < or  > or  > > or = or < >");
		parserError();
	}
}
//print the line number and the error
void parserError()
{
	cout << "Parser error on line number " << tk.lineNumber << ": expected '" << expectedToken << "' but received '" << tk.desc << "'\n";
	exit(EXIT_FAILURE);
}
//create node with the given label
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
