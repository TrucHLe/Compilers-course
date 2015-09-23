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
    int token_index;
    unordered_map<string, int> constants_map;

    Token current_token; //consider deleting
    int current_token_index; //consider deleting
    
    
    
    
public:
    Parser( Scanner s );
    vector<Token> tokens; //Parser don't have instant access to Scanner. Hence, store all scanned tokens in vector<Token> tokens.
    
    void parse(); //main
    void advance(); //consider deleting
    bool noSyntaxError( Token token );
    
    bool match( int tokenType );
    bool check( int tokenType );
    
    // Hehe, look at all these baby parsers <3
    void parseProgram();
    void parseBlock();
    void parseConstantDeclarations();
    void parseConstantDeclaration();
    void parseStatements();
    void parseStatement();
    void parseExpression();
    void parseTerm();
    void parseFactor();
    
    
    //construction
    void parseDummy();
    bool noGrammarError(); //can actually make this a void function, but use bool for consistent appearance
    bool matches( int tokenType );
    //!construction
};


#endif /* defined(__Project2_Parser__Parser__) */
