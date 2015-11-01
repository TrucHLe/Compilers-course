//
//  main.cpp
//  Project4-Interpreter
//
//  Created by Truc Le on 10/31/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Constants.h"	//for testing, removable
#include "Scanner.h"	//for testing, removable
#include "Parser.h"		//for testing, removable

using namespace std;
using std::ifstream;

int main(int argc, const char * argv[])
{
	string filename;
	ifstream input;
	
	
	// CLI
	
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
	
	
	// Xcode
	/*
	 cout << "Enter file name: ";
	 cin >> filename;
	 
	 input.open( filename );
	 
	 if ( input.fail() )
	 {
		cout << "(!) Cannot find " << filename << endl;
		cout << endl;
		return 0;
	 }
	 */
	
	
	
	// Parse
	
	Parser parser( input );
	ASTNode* node = parser.parse();
	cout << node->toString("");
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
