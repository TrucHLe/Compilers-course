//
//  Parser.h
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project2_Parser__Parser__
#define __Project2_Parser__Parser__

#include <stdio.h>  
#include <fstream>          //ifstream
#include <vector>           //vector
#include <unordered_map>    //unordered_map, aka hashmap
#include "Scanner.h"
#include "Constants.h"


class Parser
{
    
private:
    Scanner scanner;
    Token token;
    unordered_map<string, int> constants;

    
public:
    Parser( ifstream& i );
    
    void parse(); //main
    Token advance();
    Token match( int tokenType );
    bool check( int tokenType );
    
    // Baby parsers <3
    void parseProgram();
    void parseBlock();
    void parseConstantDeclarations();
    void parseConstantDeclaration();
    void parseStatements();
    void parseStatement();
    void parseExpression();
    void parseExpressionRest();
    void parseTerm();
    void parseTermRest();
    void parseFactor();
};


#endif /* defined(__Project2_Parser__Parser__) */
