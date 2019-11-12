#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "testTree.h"


using namespace std;

int main(int argc, char* argv[])
{
	//file stream to the input file
	ifstream inputFile;
	
	//input file plus extension
	char * inputFileExt;
	
	//if the argc is 2, then they put in a file name
	if(argc == 2)
	{
		//get the filename
		string fileName = argv[1];	
		
		//check if it has the .fs19 extension
		size_t index = fileName.find(".fs19");
		
		//if the input file doesn't have .fs19, add it
		if(index == string::npos)
		{
			fileName.append(".fs19");
		}
	
		
		inputFileExt = new char[fileName.length() + 1];
		//convert the string and copy its contents
		strcpy(inputFileExt, fileName.c_str());
		
		//open the input file
		inputFile.open(inputFileExt, ifstream::in);

		//check if it open successfully
		if(inputFile.is_open())
		{
			//read the file contents
			readFile(inputFile);
		}
		else
		{
			cout << "System error: Could not open the input file " << fileName << ". Terminating program\n";
			return -1;
		}
	}
	else if(argc == 1)
	{
		//get from keyboard
		readFile(cin);
	}
	else if(argc > 2)
	{
		cout << "Error: Too many arguments.\n";
		return -1;
	}
	
	//close the input file
	inputFile.close();
	
	node_t* root = parser();
	
	//to print a preorder traversal of parse tree, uncomment below
	preorder(root, root->level);
	
	//semantics(root);
	
	return 0;
}
