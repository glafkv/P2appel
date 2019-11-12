#include "token.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

map<string, string> operatorMap;
map<string, string> keywordMap;

//populate the operator map
void popOperatorMap()
{
	operatorMap.insert(make_pair("=", "EQUALStk"));
	operatorMap.insert(make_pair("<", "LESSTHANtk"));
	operatorMap.insert(make_pair(":", "COLONtk"));
	operatorMap.insert(make_pair(">", "GREATERTHANtk"));
	operatorMap.insert(make_pair("<=", "LESSTHANEQtk"));
	operatorMap.insert(make_pair(">=", "GREATERTHANEQtk"));
	operatorMap.insert(make_pair("==", "EQUALEQUALtk"));
	operatorMap.insert(make_pair("+", "PLUStk"));
	operatorMap.insert(make_pair("-", "MINUStk"));
	operatorMap.insert(make_pair("*", "ASTERIKtk"));
	operatorMap.insert(make_pair("/", "SLASHtk"));
	operatorMap.insert(make_pair("%", "MODULUStk"));
	operatorMap.insert(make_pair(".", "PERIODtk"));
	operatorMap.insert(make_pair("(", "LEFTPARENtk"));
	operatorMap.insert(make_pair(")", "RIGHTPARENtk"));
	operatorMap.insert(make_pair(",", "COMMAtk"));
	operatorMap.insert(make_pair("{", "LEFTBRACEtk"));
	operatorMap.insert(make_pair("}", "RIGHTBRACEtk"));
	operatorMap.insert(make_pair(";", "SEMICOLONtk"));
	operatorMap.insert(make_pair("[", "LEFTBRACKETtk"));
	operatorMap.insert(make_pair("]", "RIGHTBRACKETtk"));
}
//populate keyword map
void popKeywordMap()
{
	keywordMap.insert(make_pair("start", "STARTtk"));
	keywordMap.insert(make_pair("stop", "STOPtk"));
	keywordMap.insert(make_pair("iterate", "ITERtk"));
	keywordMap.insert(make_pair("void", "VOIDtk"));
	keywordMap.insert(make_pair("var", "VARtk"));
	keywordMap.insert(make_pair("return", "RETURNtk"));
	keywordMap.insert(make_pair("in", "INtk"));
	keywordMap.insert(make_pair("out", "OUTtk"));
	keywordMap.insert(make_pair("program", "PROGRAMtk"));
	keywordMap.insert(make_pair("cond", "CONDtk"));
	keywordMap.insert(make_pair("then", "THENtk"));
	keywordMap.insert(make_pair("let", "LETtk"));
}
//display line num and desc
void displayToken(Token token)
{
	cout << "Line number: " << token.lineNumber << " " << tokenNames[token.ID] << " " << token.desc << "\n";
}
//assign the operator token type to desc
void getOperator(Token &token)
{
	token.desc.assign(operatorMap[token.desc]);
}
int isOperator(char ch)
{
	for(int counter = 0; counter < NUM_OPERATORS; counter++)
	{
		if(ch == operators[counter])
			return 1;
	}
	return 0;
}
int isKeyword(Token &token)
{
	for(int counter = 0; counter < NUM_KEYWORDS; counter++)
	{
		if(token.desc == keywords[counter])
		{
			token.desc = keywordMap[token.desc];
			return counter;
		}
	}
	return -1;
}
