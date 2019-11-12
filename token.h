#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <map>

using namespace std;

const int NUM_TOKENS = 5; 
const int NUM_KEYWORDS = 12;
const int NUM_OPERATORS = 21;

extern int currentIndex;
extern int tokenIndex;
extern int lineIndex;

//map the operator to the token name
extern map<string, string> operatorMap;

//map the keyword to the token name
extern map<string, string> keywordMap;

//token categories
enum tokenID {IDtk, KEYWORDtk, OPtk, INTtk, EOFtk};

//name of each token category
const string tokenNames[NUM_TOKENS] = {"Identifier", "Keyword", "Operator", "Integer", "End of File"};

//name of each keyword
const string keywords[NUM_KEYWORDS] = {"start", "stop", "iterate", "void", "var", "return", "in", "out", "program", "cond", "then", "let"};

//name of each operator
const char operators[] = {'=', '<', '>', ':', '+', '-', '*', '/', '%', '.', '(', ')', ',', '{', '}', ';', '[', ']'};

//comment symbol
const char COMMENT_DELIM = '#';

struct Token
{
	tokenID ID;
	string desc;
	int lineNumber;
};

void popOperatorMap();
void popKeywordMap();
void displayToken(Token);
void getOperator(Token &);
int isOperator(char);
int isKeyword(Token &);

#endif
