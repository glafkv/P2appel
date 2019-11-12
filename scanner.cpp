#include "scanner.h"
#include "token.h"
#include <string>
#include <cctype>
#include <cstddef>
#include <iostream>
#include <stdlib.h>

using namespace std;

//from token.h
int tokenIndex;

int currentIndex;
int lineIndex;
vector <string> fileString;
static int commentFlag;

//function that wil read the entire input file and store each line in a vector
void readFile(istream &in)
{
	//initiaize maps
	popOperatorMap();
	popKeywordMap();
		
	//current line of input
	string inputLine;
	currentIndex = 0;
	Token token;
	
	int counter = 0;
	
	//read each line until EOF. store in the vector
	while(getline(in, inputLine))
	{
		currentIndex = 0;
	
		//remove comments and whitespace
		filter(inputLine);
		
		//store in vector
		if(inputLine.length() > 0)
		{
			fileString.push_back(inputLine);
			counter++;
		}
	}
}

int filter(string &inputString)
{
	//to prevent going out of bounds
	if(currentIndex >= inputString.length())
		return 0;
		
	//filtered string
	string filteredString;
	
	char currentChar;
	const char SPACE = ' ';
	char prevChar = ' ';	
	
	//iterate through each character
	for(int counter = currentIndex; counter < inputString.length(); counter++)
	{
		//get the current character
		currentChar = inputString.at(counter);
	
		if(counter > 0)
			prevChar = inputString.at(counter - 1);
		
		//if the current character is #
		if(currentChar == COMMENT_DELIM)
		{
			//add a space so comments will be ignored
			filteredString.push_back(SPACE);
			
			commentFlag = !commentFlag;
		}
		else if(!commentFlag)
		{
			//filter the string so it doesn't contain whitespace
			if(isspace(currentChar))
			{
				if(!isspace(prevChar))
					filteredString.push_back(currentChar);
			}
			else if(isValidChar(currentChar) == -1)
			{
				//if its not part of the alphabet, print an error
				cout << "Error: '" << currentChar << "' is not a valid character\n";
				exit(EXIT_FAILURE);
			}
			else
			{
				filteredString.push_back(currentChar);
			}
		}
		//increment current index
		currentIndex++;
	}
	
	string whitespaces = " \t\f\v\n\r";
	
	//strip the whitespaces
	size_t trailingWhitespaceIndex = filteredString.find_last_not_of(whitespaces);
	
	//remove the trailing whitespaces
	if(trailingWhitespaceIndex != string::npos)
	{
		filteredString.erase(trailingWhitespaceIndex + 1);
	}
	else
	{
		//check if the entire line is spaces
		bool allSpacesFlag = true;
		
		//iterate through filtered string
		for(int i = 0; i < filteredString.length(); i++)
		{
			if(filteredString.at(i) != SPACE)
			{
				allSpacesFlag = false;
				break;
			}
		}
		//if the whole string is comments, reassign to empty value
		if(allSpacesFlag)
			filteredString.assign("");
	}
	//reassign input string to filtered string 
	inputString.assign(filteredString);
	
	return currentIndex;
}

int isValidChar(char ch)
{
	//check if this operator is valid
	if(!isOperator(ch) && !isdigit(ch) && !isalpha(ch))
		return -1;
	else
		return 0;
}
//get the current line from the file
string getString()
{
	return fileString[lineIndex];
}

int scanner(Token &token)
{
	string inputString = getString();
	
	if(tokenIndex == inputString.length())
	{
		lineIndex++;
		tokenIndex = 0;
		
		if(lineIndex < fileString.size())
			inputString = fileString[lineIndex];
		else
		{
			token.desc = "EOF";
			token.ID = EOFtk;
			token.lineNumber = (lineIndex - 1);
			return 1;
		}
	}
	//get the line number for the current token
	token.lineNumber = (lineIndex + 1);
	
	//begin at state 0 when identifying the token
	int currentState = 0;

	//for the table
	int nextState;
	int nextCol;
	
	//token description
	string tokenDesc;
	
	//next character
	char nextChar;
	
	//space character
	const char SPACE = ' ';
	
	//contine reading the line and identifying tokens
	while(tokenIndex <= inputString.length())
	{
		if(tokenIndex < inputString.length())
			nextChar = inputString.at(tokenIndex);
		else
			nextChar = SPACE;
		
		//get the next column of the token in the table
		nextCol = getCol(nextChar);
		nextState = FSA_TABLE[currentState][nextCol];

		//if table returns a negative int, its an error
		if(nextState < 0)
		{
			//reset to 0
			currentState = 0;
		
			//print the error
			errorOutput(nextState, inputString);
			
			//print the error where it occured
			cout << inputString.substr(0, tokenIndex + 1) << "\n";
			cout << string(tokenIndex, SPACE) << "^\n";	
			
			tokenIndex++;
		
			exit(EXIT_FAILURE);
		}
		else if(nextState > FINAL_STATES)
		{
			//match the returned value to the final states
			token.desc = tokenDesc;
			
			//identify the final state
			switch(nextState)
			{
				//if its an id final state, see if its a keyword
				case IDENTIFIER_FINAL_STATE:
					if(isKeyword(token) != -1)
					{
						token.ID = KEYWORDtk;
						token.desc.assign(" " + tokenDesc);
					}
					else
					{
						token.ID = IDtk;
						token.desc.assign("IDtk " + tokenDesc);
					}
					break;
				//if its an int final state, assign the token id to inttk and assign desc
				case INTEGER_FINAL_STATE:
					token.ID = INTtk;
					token.desc.assign("INTtk " + tokenDesc);
					break;
				//final operator state
				case OPERATOR_FINAL_STATE:
					token.ID = OPtk;
					getOperator(token);
					token.desc.assign(tokenDesc);
					break;
			}
			return 0;
		}
		//if the nextState was not an error or final, assign the current state to next state
		currentState = nextState;
	
		//increment the index
		tokenIndex++;
		//if next character is not a space, add this char to token desc	
		if(!isspace(nextChar))
			tokenDesc.push_back(nextChar);
	}
	return -1;
}
//get the column that corresponds to the FSA table
int getCol(char ch)
{
	//determine if its an upper or lower
	if(isalpha(ch))
	{
		if(islower(ch))
			return 0;
		else
			return 1;
	}
	else if(isdigit(ch))
		return 2;
	else if(isspace(ch))
		return 3;
	else if(isOperator(ch))
		return 5;
	else
		return -1;
}
//print the error
void errorOutput(int state, string inputString)
{
	cout << "Scanner error: Line " << lineIndex << ": ";
	
	//print the error
	if(state == ERROR_STATE_UPPERCASE)
	{
		cout << "All tokens with alphabetical characters must begin with a lowercase letter\n";
	}
	else if(state == ERROR_STATE_INTEGER)
	{
		cout << "All integer tokens must contain only digits\n";
	}
}
