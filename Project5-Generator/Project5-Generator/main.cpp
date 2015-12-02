//
//  main.cpp
//  Project5-Generator
//
//  Created by Truc Le on 11/30/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Parser.h"

using namespace std;
using std::ifstream;

int main(int argc, const char * argv[])
{
	string filename;
	ifstream input;
	
	
	// CLI
	/*
	if ( argc != 2 )
	{
		cout << "(!) Fail to run program. Please enter a filename." << endl;
		cout << "$ ./main <filename>" << endl;
		cout << endl;
		return 0;
	}
	
	filename = argv[1];
	
	input.open( filename );
	
	if ( input.fail() )
	{
		cout << "(!) Cannot find " << filename << endl;
		cout << endl;
		return 0;
	}
	*/
	
	
	// Xcode
	
	 cout << "Enter file name: ";
	 getline( cin, filename );
	 
	 input.open( filename );
	 
	 if ( input.fail() )
	 {
		cout << "(!) Cannot find " << filename << endl;
		cout << endl;
		return 0;
	 }
	
	
	
	// Interpret
	// /Users/Bamboo/Developer/Compilers-course/Project5-Generator/Project5-Generator/test.txt
	Parser parser( input );
	Program* node = parser.parse();
	
	//cout << node->toString("");	// Print AST
	//node->interpret();			// Interpret
	node->typecheck();			// Typecheck
	node->generate();			// Generate YASM Code
	delete node;

	
	
	// Scan
	/*
	Scanner scanner( input );
	Token token;
	do
	{
		token = scanner.next();
		cout << token << endl;
	} while ( token.type != EOF_T );
	cout << endl;
	*/
	
	
	return 0;
}
