#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include <vector>

using namespace std;

//this vector will contain the line of input
extern vector<string> fileString;

//FSA table row and columns
const int STATES = 4;
const int COLUMNS = 6;

//all possible states in FSA table
const int STATE_ZERO = 0;
const int STATE_ONE = 1;
const int STATE_TWO = 2;
const int STATE_THREE = 3;
const int FINAL_STATES = 1000;
const int IDENTIFIER_FINAL_STATE = 1001;
const int INTEGER_FINAL_STATE = 1003;
const int OPERATOR_FINAL_STATE = 1004;
const int EOF_FINAL_STATE = 1005;
const int ERROR_STATE_UPPERCASE = -1;
const int ERROR_STATE_INTEGER = -2;

//fsa table
const int FSA_TABLE[STATES][COLUMNS] =
{
	{STATE_TWO, ERROR_STATE_UPPERCASE, STATE_THREE, STATE_ZERO, EOF_FINAL_STATE, STATE_ONE},
	
	{OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE, OPERATOR_FINAL_STATE},
	
	{STATE_TWO, STATE_TWO, STATE_TWO, IDENTIFIER_FINAL_STATE, IDENTIFIER_FINAL_STATE, IDENTIFIER_FINAL_STATE},
	
	{ERROR_STATE_INTEGER, ERROR_STATE_INTEGER, STATE_THREE, INTEGER_FINAL_STATE, INTEGER_FINAL_STATE, INTEGER_FINAL_STATE}

};

void readFile(istream&);
int filter(string&);
int isValidChar(char);
string getString();
int scanner(Token &);
int getCol(char);
void errorOutput(int, string);

#endif
