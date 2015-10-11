//
//  main.cpp
//  Project3-Parser
//
//  Created by Truc Le on 10/10/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Constants.h"
#include "Scanner.h"
#include "Parser.h"

using namespace std;
using std::ifstream;

int main(int argc, const char * argv[])
{
	
	//---------------------------------------------
	// Open text file and pass it to Scanner object
	//
	string filename;
	ifstream input;
	
	/* CLI running, uncomment when turn in
	if ( argc != 2 )
	{
		cout << "(!) Fail to run program. Please enter a filename." << endl;
		cout << "$ ./main <filename>" << endl;
		cout << endl;
		return 0;
	}
	
	filename = argv[1];
	*/
	
	cout << "Enter file name: "; // /Users/Bamboo/Developer/Compilers-course/Project3-Parser/Project3-Parser/test3.txt
	cin >> filename;
	
	input.open( filename );
	
	if ( input.fail() )
	{
		cout << "(!) Cannot find " << filename << endl;
		cout << endl;
		return 0;
	}
	
	
	Parser parser( input );
	//ASTNode* node = parser.parseProgram();
	//cout << node->toString() //toString takes indentation //look at Exam1 on I Drive
	//ask Program Node to print here
	//delete node //make destructor in AST.h
	
	
	
	//Test scanner
	Scanner scanner( input );
	Token token;
	do
	{
		token = scanner.next();
		cout << token << endl;
		
	} while ( token.type != EOF_T );
	cout << endl;
	// !Test scanner
	
	//
	// !Open text file and pass it to Scanner object
	//---------------------------------------------
	
	
	
	return 0;
}