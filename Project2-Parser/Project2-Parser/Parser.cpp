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
        {
            tokens.push_back( token );
        }

    } while ( token.type != EOF_T );
    
    current_index = 0;
    current_token = tokens.at( 0 );

//    for ( int i = 0; i < tokens.size(); ++i )
//    {
//        cout << tokens.at( i ) << endl;
//    }

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
            exit( 1 );
            
        case BRACKETERROR_T:
            cout << token << endl;
            exit( 1 );
            
        case OTHERERROR:
            cout << token << endl;
            exit( 1 );
            
        default:
            return true;
    }
}



//===----------------------------------------------------------------------===//
// Advance to the next token in tokens
//===----------------------------------------------------------------------===//
Token Parser::advance()
{
    Token token = current_token;
    
    if ( current_index + 1 < tokens.size() )
    {
        ++current_index;
        current_token = tokens.at( current_index );
    }
    
    return token;
}



//===----------------------------------------------------------------------===//
// Match current_token's type with each standard statement structure
// match should move to the next checked token
// if it is matched, advance and return the old token
//===----------------------------------------------------------------------===//
Token Parser::match( int tokenType )
{
    if ( check( tokenType ) )
        return advance();
    else
    {
        cout << "(!) Expected " << nameOf( tokenType ) << " at " << current_token.line << ":" << current_token.column << endl;
        exit( 1 );
    }
}



//===----------------------------------------------------------------------===//
// Check whether the current token's type equal tokenType
//===----------------------------------------------------------------------===//
bool Parser::check( int tokenType )
{
//    cout << "Current: " << current_token << endl;
//    cout << "Expected: " << nameOf( tokenType ) << endl;
//    cout << endl;
    
    if ( nameOf( tokenType ) == "NUM" && nameOf( current_token.type ) == "NUM" )
        return true;
    else if ( current_token.type == tokenType )
        return true;
    else
        return false;
}



//===----------------------------------------------------------------------===//
// Parsers based on grammar in Grammar.txt
//
void Parser::parseProgram()
{
    match( PRGRM_T );
    match( IDENT_T );
    match( SEMICOLON_T );
    parseBlock();
    match( PERIOD_T );
}



void Parser::parseBlock()
{
    parseConstantDeclarations();
    match( BEGIN_T );
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
    Token key = match( IDENT_T );
    match( SETEQUAL_T );
    Token value = match( INTEGER_T );
    match( SEMICOLON_T );
    
    pair<string, int> constant = make_pair( key.lexeme, stoi( value.lexeme ) );
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
    Token print = match( PRINT_ST );
    parseExpression();
    match( SEMICOLON_T );
    cout << nameOf( print.type ) << endl;
}



void Parser::parseExpression()
{
    parseTerm();
    parseExpressionRest();
}



void Parser::parseExpressionRest()
{
    if ( check( ADDITION_ST ) )
    {
        Token add = match( ADDITION_ST );
        parseTerm();
        cout << add.lexeme << endl; //this one stays here. Check Grammar.txt if confused.
        parseExpressionRest();
    }
    else if ( check( SUBTRACT_ST ) )
    {
        Token subtract = match( SUBTRACT_ST );
        parseTerm();
        cout << subtract.lexeme << endl;
        parseExpressionRest();
    }
    else {}
    
}



void Parser::parseTerm()
{
    parseFactor();
    parseTermRest();
}



void Parser::parseTermRest()
{
    if ( check( ASTERISK_ST ) )
    {
        Token star = match( ASTERISK_ST );
        parseFactor();
        cout << star.lexeme << endl;
        parseTermRest();
    }
    else if ( check( DIV_ST ) )
    {
        Token div = match( DIV_ST );
        parseFactor();
        cout << nameOf( div.type ) << endl;
        parseTermRest();
    }
    else if ( check( MOD_ST ) )
    {
        Token mod = match( MOD_ST );
        parseFactor();
        cout << nameOf( mod.type ) << endl;
        parseTermRest();
    }
    else {}
}



void Parser::parseFactor()
{
    if ( check( SINGLE0_T ) )
    {
        Token num = match( SINGLE0_T );
        cout << num.lexeme << endl;
    }
    else if ( check( INTEGER_T ) )
    {
        Token num = match( INTEGER_T );
        cout << num.lexeme << endl;
    }
    else if ( check( IDENT_T ) )
    {
        Token id = match( IDENT_T );
        
        // unordered_map auto throws out-of-range
        // error if it doesn't have the passed key
        int value = constants.at( id.lexeme );
        cout << value << endl;
    }
    else
    {
        cout << "(!) Expected either a NUM or an ID at " << current_token.line << ":" << current_token.column << endl;
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
