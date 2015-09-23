//
//  Scanner.h
//  Project1-Scanner
//
//  Created by Truc Le on 9/6/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project1_Scanner__Scanner__
#define __Project1_Scanner__Scanner__

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
    int index; //to use in Parser
    string lexeme;
    
    void setToken( int l, int c, int t, int i, string lex );
    
};

// Format a Token for display on output stream (cout)
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
    int token_index;
    
public:
    Scanner( ifstream& i );
    
    Token next(); //main
    
    char getCurrentChar();
    void advance();
    int getNextState( int currentState, char currentChar );
    int getNextTerminalState();
    
};


#endif /* defined(__Project1_Scanner__Scanner__) */
