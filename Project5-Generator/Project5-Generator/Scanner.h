//
//  Scanner.h
//  Project5-Generator
//
//  Created by Truc Le on 9/6/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project5_Generator__Scanner__
#define __Project5_Generator__Scanner__

#include <stdio.h>
#include <strings.h>    // strcasecmp
#include <string>       // string
#include <iostream>     // cin, cout
#include <fstream>      // ifstream
#include "Constants.h"
using namespace std;



//===----------------------------------------------------------------------===//
// Token Class
//===----------------------------------------------------------------------===//
class Token
{
    
public:
    Token();
    int line;
    int column;
    int type;
    string lexeme;
    
    void setToken( int l, int c, int t, string lex );
};

// Format a Token for display on output stream (cout)
string nameOf( int tokenType );
ostream& operator << ( ostream& out, const Token& token );



//===----------------------------------------------------------------------===//
// Scanner Class
//===----------------------------------------------------------------------===//
class Scanner
{
    
private:
    ifstream& input;
    string current_line;
    string current_lexeme;
    int line_number;
    int column_number;
    int token_line;
    int token_column;
    
public:
    Scanner( ifstream& i );
    
    Token next(); //main
    
    char getCurrentChar();
    void advance();
    int getNextState( int currentState, char currentChar );
    int getNextTerminalState();
    
};


#endif /* defined(__Project5_Generator__Scanner__) */
