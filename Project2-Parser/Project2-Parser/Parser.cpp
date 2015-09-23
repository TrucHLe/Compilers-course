//
//  Parser.cpp
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "Parser.h"


//===----------------------------------------------------------------------===//
// Parser constructor:
//
// - Initializes scanner in initilization list because it is a const
// thus cannot be initialized (aka changed) inside the constructor.
// - Checks sysntax errors and pushes all scanned tokens in vector<Token> tokens.
// - Checks grammar errors.
//===----------------------------------------------------------------------===//
Parser::Parser( Scanner s ) : scanner( s )
{
    Token token;
    
    do
    {
        token = scanner.next();
        
        if ( noSyntaxError( token ) )
            tokens.push_back( token );

    } while ( token.type != EOF_T );

    
    current_index = 0;
    current_token = tokens.at( current_index );

    parse();
}



//===----------------------------------------------------------------------===//
// Check if token has any syntax error state
//===----------------------------------------------------------------------===//
bool Parser::noSyntaxError( Token token )
{
    switch ( token.type )
    {
        case SLASHERROR_T:
            cout << token << endl;
            return false;
            
        case BRACKETERROR_T:
            cout << token << endl;
            return false;
            
        case OTHERERROR:
            cout << token << endl;
            return false;
            
        default:
            return false;
    }
}



//===----------------------------------------------------------------------===//
// Advance to the next token in tokens
//===----------------------------------------------------------------------===//
void Parser::advance( int steps )
{
    if ( current_index < tokens.size() )
    {
        current_index += steps ;
        current_token = tokens.at( current_index );
    }
}



//===----------------------------------------------------------------------===//
// Match current_token's type with each standard statement structure
// match should move to the next checked token
//===----------------------------------------------------------------------===//
void Parser::match( int tokenType )
{

    switch ( tokenType )
    {
        case PRGRM_T:
            if ( current_token.type == PRGRM_T )
            {
                if ( tokens.at( current_index + 1 ).type == IDENT_T ) //multi-step verification to print exact error message
                {
                    if ( tokens.at( current_index + 2 ).type == SEMICOLON_T ) {}
                    else
                    {
                        cout << "(!) Expected ';' at " << tokens.at( current_index + 2 ).line << ":" << tokens.at( current_index + 2 ).column << endl;
                        exit( 1 );
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier at " << tokens.at( current_index + 1 ).line << ":" << tokens.at( current_index + 1 ).column << endl;
                    exit( 1 );
                }
            }
            else
            {
                cout << "(!) Expected 'program' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
            
        case CONST_T:
            if ( current_token.type == CONST_T )
            {
                if ( tokens.at( current_index + 1 ).type == IDENT_T )
                {
                    if ( tokens.at( current_index + 2 ).type == SETEQUAL_T )
                    {
                        if ( tokens.at( current_index + 3 ).type == SINGLE0_T ||
                             tokens.at( current_index + 3 ).type == INTEGER_T )
                        {
                            if ( tokens.at( current_index + 4 ).type == SEMICOLON_T ) {}
                            else
                            {
                                cout << "(!) Expected ';' at " << tokens.at( current_index + 4 ).line << ":" << tokens.at( current_index + 4 ).column << endl;
                                exit( 1 );
                            }
                        }
                        else
                        {
                            cout << "(!) Expected a number at " << tokens.at( current_index + 3 ).line << ":" << tokens.at( current_index + 3 ).column << endl;
                            exit( 1 );
                        }
                    }
                    else
                    {
                        cout << "(!) Expected '=' at " << tokens.at( current_index + 2 ).line << ":" << tokens.at( current_index + 2 ).column << endl;
                        exit( 1 );
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier at " << tokens.at( current_index + 1 ).line << ":" << tokens.at( current_index + 1 ).column << endl;
                    exit( 1 );
                }
            }
            else
            {
                cout << "(!) Expected 'const' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
            
        case BEGIN_T:
            if ( current_token.type == BEGIN_T ) {}
            else
            {
                cout << "(!) Expected 'begin' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case END_T:
            if ( current_token.type == END_T ) {}
            else
            {
                cout << "(!) Expected 'end' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case PRINT_ST:
            if ( current_token.type == PRINT_ST ) {}
            else
            {
                cout << "(!) Expected 'print' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case DIV_ST:
            if ( current_token.type == DIV_ST ) {}
            else
            {
                cout << "(!) Expected 'div' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case MOD_ST:
            if ( current_token.type == MOD_ST ) {}
            else
            {
                cout << "(!) Expected 'mod' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case SINGLE0_T:
            if ( current_token.type == SINGLE0_T ) {}
            else
            {
                cout << "(!) Expected a number at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case INTEGER_T:
            if ( current_token.type == INTEGER_T ) {}
            else
            {
                cout << "(!) Expected a number at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }

            
        case ADDITION_ST:
            if ( current_token.type == ADDITION_ST ) {}
            else
            {
                cout << "(!) Expected '+' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case SUBTRACT_ST:
            if ( current_token.type == SUBTRACT_ST ) {}
            else
            {
                cout << "(!) Expected '-' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case ASTERISK_ST:
            if ( current_token.type == ASTERISK_ST ) {}
            else
            {
                cout << "(!) Expected '*' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }

            
        case PERIOD_T:
            if ( current_token.type == PERIOD_T ) {}
            else
            {
                cout << "(!) Expected '.' at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
            
            
        case EOF_T:
            if ( current_token.type == EOF_T ) {}
            else
            {
                cout << "(!) Expected to reach end of file at " << current_token.line << ":" << current_token.column << endl;
                exit( 1 );
            }
    }
}



//===----------------------------------------------------------------------===//
// Check whether the current statement contains tokenType
// return if current type = expected type
//===----------------------------------------------------------------------===//
bool Parser::check( int tokenType )
{
    switch ( tokenType )
    {
        case PRGRM_T:
            if ( current_token.type == PRGRM_T )
                return true;
            else
                return false;
            
            
        case CONST_T:
            if ( current_token.type == CONST_T )
                return true;
            else
                return false;
            
            
        case BEGIN_T:
            if ( current_token.type == BEGIN_T )
                return true;
            else
                return false;
            
            
        case END_T:
            if ( current_token.type == END_T )
                return true;
            else
                return false;
            
            
        case PRINT_ST:
            if ( current_token.type == PRINT_ST )
                return true;
            else
                return false;
            
            
        case DIV_ST:
            if ( current_token.type == DIV_ST )
                return true;
            else
                return false;
            
            
        case MOD_ST:
            if ( current_token.type == MOD_ST )
                return true;
            else
                return false;
            
            
        case SINGLE0_T:
            if ( current_token.type == SINGLE0_T )
                return true;
            else
                return false;
            
            
        case INTEGER_T:
            if ( current_token.type == INTEGER_T )
                return true;
            else
                return false;
            
            
        case SETEQUAL_T:
            if ( current_token.type == SETEQUAL_T )
                return true;
            else
                return false;
            
            
        case ADDITION_ST:
            if ( current_token.type == ADDITION_ST )
                return true;
            else
                return false;
            
            
        case SUBTRACT_ST:
            if ( current_token.type == SUBTRACT_ST )
                return true;
            else
                return false;
            
            
        case ASTERISK_ST:
            if ( current_token.type == ASTERISK_ST )
                return true;
            else
                return false;
            
            
        case SEMICOLON_T:
            if ( current_token.type == SEMICOLON_T )
                return true;
            else
                return false;
            
            
        case PERIOD_T:
            if ( current_token.type == PERIOD_T )
                return true;
            else
                return false;
            
            
        case EOF_T:
            if ( current_token.type == EOF_T )
                return true;
            else
                return false;
            
            
        default:
            return false;
    }
}



//===----------------------------------------------------------------------===//
// Parsers based on grammar in Grammar.txt
//
void Parser::parseProgram()
{
    match( PRGRM_T );
    advance( 3 );
    
    parseBlock();
    
    match( PERIOD_T );
    advance( 1 );
}



void Parser::parseBlock()
{
    parseConstantDeclarations();
    
    match( BEGIN_T );
    advance( 1 );
    
    parseStatements();
    match( END_T );
}



void Parser::parseConstantDeclarations()
{
    if ( check( CONST_T ) )
    {
        parseConstantDeclaration();
        parseConstantDeclarations();
    }
    else {}
}



//===---------------------------------------===//
// Add the matched constant to constants hash map
//
void Parser::parseConstantDeclaration()
{
    match( CONST_T );
    advance( 5 );
    
    // Constant grammar = CONST ID ASSIGN NUM SEMI
    // After match( CONST_T ), current_index is now at SEMI's subsequent token
    // Hence, index of ID = current_index - 4, and index of NUM = current_index - 2
    //
    pair<string, int> constant = make_pair( tokens.at( current_index - 4 ).lexeme, stoi( tokens.at( current_index - 2 ).lexeme ) );
    constants.insert( constant );
}



void Parser::parseStatements()
{
    if ( check( PRINT_ST ) )
    {
        parseStatement();
        parseStatements();
    }
    else {}
}



void Parser::parseStatement()
{
    match( PRINT_ST );
    advance( 1 );
    
    parseExpression();
    
    match( SEMICOLON_T );
    advance( 1 );
}



void Parser::parseExpression()
{
    if ( check( ADDITION_ST ) )
    {
        parseExpression();
        
        match( ADDITION_ST );
        advance( 1 );
        
        parseTerm();
    }
    else if ( check( SUBTRACT_ST ) )
    {
        parseExpression();
        
        match( SUBTRACT_ST );
        advance( 1 );
        
        parseTerm();
    }
    else
        parseTerm();
}



void Parser::parseTerm()
{
    if ( check( ASTERISK_ST ) )
    {
        parseTerm();
        
        match( ASTERISK_ST );
        advance( 1 );
        
        parseFactor();
    }
    else if ( check( DIV_ST ) )
    {
        parseTerm();
        
        match( DIV_ST );
        advance( 1 );
        
        parseFactor();
    }
    else if ( check( MOD_ST ) )
    {
        parseTerm();
        
        match( MOD_ST );
        advance( 1 );
        
        parseFactor();
    }
    else
        parseFactor();
}



void Parser::parseFactor()
{
    if ( check( SINGLE0_T ) )
    {
        match( SINGLE0_T ); //push num to stack
        advance( 1 );
    }
    else if ( check( INTEGER_T ) )
    {
        match( INTEGER_T );
        advance( 1 );
    }
    else if ( check( IDENT_T ) ) //push num to stack
    {
        match( IDENT_T ); ////look up and push num to stack
        advance( 1 );
        
        int value = constants.at( tokens.at( current_index - 1 ).lexeme ); //unordered_map auto throws out-of-range error if it doesn't have the passed key
    }
    else
    {
        cout << "(!) Expected either a number or an identifier at " << current_token.line << ":" << current_token.column << endl;
        exit( 1 );
    }
}
//
// !baby parsers
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// The main Parser
//===----------------------------------------------------------------------===//
void Parser::parse()
{
    parseProgram();
    match( EOF_T );
}














//===----------------------------------------------------------------------===//
// Parse Dummy
//===----------------------------------------------------------------------===//
/*
void Parser::parseDummy()
{
    while ( current_token.type != EOF_T )
    {
        if ( matches( PRGRM_T ) )
        {
            advance();
            
            // do something in case begin doesn't follow immediately
            if ( matches( BEGIN_T ) )
            {
                advance();
                // do something with matches( END_T )
                if ( matches( CONST_T ) )
                {
                    
                    // The above matchedExpression condition has advanced
                    // current_token to SEMI, so we have to go back
                    // a couple cells to get the integer's ID and NUM
                    //
                    string id = tokens.at( current_index - 2 ).lexeme;
                    int num = stoi( tokens.at( current_index - 1 ).lexeme );
                    
                    constants_map.insert( { id, num } );
                    
                }
                
                advance();
                
            }
            
            //work through Howard's pseudocode
            
            
        }
        
        advance();
    }
}
*/
