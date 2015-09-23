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

    parseProgram();
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
// Match current_token's type with each standard statement structure
// and advance token_index whenever matched a token
//===----------------------------------------------------------------------===//
bool Parser::match( int tokenType )
{

    switch ( tokenType )
    {
        case PRGRM_T:
            if ( current_token.type == PRGRM_T )
            {
                if ( tokens.at( current_token.index + 1 ).type == IDENT_T ) //2-step verification to print exact error message
                {
                    if ( tokens.at( current_token.index + 2 ).type == SEMICOLON_T )
                    {
                        current_index = current_token.index + 3; //+ 3 to jump pass program's 3 components: PROGRAM ID SEMI
                        return true;
                    }
                    else
                    {
                        cout << "(!) Expected a semicolon at " << tokens.at( current_token.index + 1 ).line << ":" << tokens.at( current_token.index + 1 ).column << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier at " << current_token.line << ":" << current_token.column << endl;
                    return false;
                }
            }
            else
                return false;
            
            
            
        case CONST_T:
            if ( current_token.type == CONST_T )
            {
                if ( tokens.at( current_token.index + 1 ).type == IDENT_T )
                {
                    if ( tokens.at( current_token.index + 2 ).type == SETEQUAL_T )
                    {
                        if ( tokens.at( current_token.index + 3 ).type == SINGLE0_T ||
                             tokens.at( current_token.index + 3 ).type == INTEGER_T )
                        {
                            if ( tokens.at( current_token.index + 4 ).type == SEMICOLON_T )
                            {
                                current_index = current_token.index + 5;
                                return true;
                            }
                            else
                            {
                                cout << "(!) Expected a semicolon at " << tokens.at( current_token.index + 3 ).line << ":" << tokens.at( current_token.index + 3 ).column << endl;
                                return false;
                            }
                        }
                        else
                        {
                            cout << "(!) Expected a number at " << tokens.at( current_token.index + 2 ).line << ":" << tokens.at( current_token.index + 2 ).column << endl;
                            return false;
                        }
                    }
                    else
                    {
                        cout << "(!) Expected an equal sign at " << tokens.at( current_token.index + 1 ).line << ":" << tokens.at( current_token.index + 1 ).column << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier at " << current_token.line << ":" << current_token.column << endl;
                    return false;
                }
            }
            else
                return false;
            
            
            
        case BEGIN_T:
            if ( current_token.type == BEGIN_T )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
            
        case END_T:
            if ( current_token.type == END_T )
            {
                if ( tokens.at( current_token.index + 1 ).type == PERIOD_T )
                {
                    current_index = current_token.index + 2;
                    return true;
                }
                else
                {
                    cout << "(!) Expected a period at " << current_token.line << ":" << current_token.column << endl;
                    return false;
                }
                
            }
            else
                return false;
            
            
            
        case PRINT_ST:
            if ( current_token.type == PRINT_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        case DIV_ST:
            if ( current_token.type == DIV_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        case MOD_ST:
            if ( current_token.type == MOD_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        case ADDITION_ST:
            if ( current_token.type == ADDITION_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        case SUBTRACT_ST:
            if ( current_token.type == SUBTRACT_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        case ASTERISK_ST:
            if ( current_token.type == ASTERISK_ST )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
            
        case SINGLE0_T:
            if ( current_token.type == SINGLE0_T )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
            
        case INTEGER_T:
            if ( current_token.type == INTEGER_T )
            {
                current_index = current_token.index + 1;
                return true;
            }
            else
                return false;
            
            
        default:
            return false;
    }
}



//===----------------------------------------------------------------------===//
// Check whether the current statement contains tokenType
//===----------------------------------------------------------------------===//
bool Parser::check( int tokenType )
{
    for ( int i = current_token.index; i < tokens.size(); ++i )
        if ( tokens.at( i ).type == tokenType )
            return true;
    return false;
}



//===----------------------------------------------------------------------===//
// Parsers based on grammar in Grammar.txt
//
void Parser::parseProgram()
{
    if ( match( PRGRM_T ) )
    {
        current_token = tokens.at( current_index );
        parseBlock();
    }
    else
    {
        cout << "(!) Expected to see 'program' at the beginning of file" << endl;
        exit( 1 );
    }
}



void Parser::parseBlock()
{
    parseConstantDeclarations();
    
    if ( match( BEGIN_T ) )
    {
        current_token = tokens.at( current_index );
        parseStatements();
        
        if ( match( END_T ) )
        {
            //advance to EOF
            current_token = tokens.at( current_index );
        }
        else
        {
            cout << "(!) Expected 'end.' at " << current_token.line << ":" << current_token.column << endl;
        }
    }
    else
    {
        cout << "(!) Expected to see 'begin' at " << current_token.line << ":" << current_token.column << endl;
        exit( 1 );
    }
}



void Parser::parseConstantDeclarations()
{
    if ( check( CONST_T ) )
    {
        parseConstantDeclaration();
        parseConstantDeclarations();
    }
    else
    {
        //do nothing
    }
}



void Parser::parseConstantDeclaration()
{
    if ( match( CONST_T ) ) //this is the push pop math step. Add more later
    {
        current_token = tokens.at( current_index );
    }
}



void Parser::parseStatements()
{
    if ( check( PRINT_ST ) )
    {
        parseStatement();
        parseStatements();
    }
    else
    {
        //do nothing
    }
    
}



void Parser::parseStatement()
{
    if ( match( PRINT_ST ) )
    {
        current_token = tokens.at( current_index );
        parseExpression();
    }
}



void Parser::parseExpression()
{
    if ( check( ADDITION_ST ) )
    {
        parseExpression();
        if ( match( ADDITION_ST ) )
        {
            current_token = tokens.at( current_index );
            parseTerm(); //anything else after this?
        }
    }
    else if ( check( SUBTRACT_ST ) )
    {
        parseExpression();
        if ( match( SUBTRACT_ST ) )
        {
            current_token = tokens.at( current_index );
            parseTerm();
        }
    }
    else
        parseTerm();
}



void Parser::parseTerm()
{
    if ( check( ASTERISK_ST ) )
    {
        parseTerm();
        if ( match( ASTERISK_ST ) )
        {
            current_token = tokens.at( current_index );
            parseFactor();
        }
    }
    else if ( check( DIV_ST ) )
    {
        parseTerm();
        if ( match( DIV_ST ) )
        {
            current_token = tokens.at( current_index );
            parseFactor();
        }
        
    }
    else if ( check( MOD_ST ) )
    {
        parseTerm();
        if ( match( MOD_ST ) )
        {
            current_token = tokens.at( current_index );
            parseFactor();
        }
    }
    else
        parseFactor();
}



void Parser::parseFactor()
{
    if ( match( SINGLE0_T ) || match( INTEGER_T ) )
    {
        current_token = tokens.at( current_index );
    }
    else if ( match( IDENT_T ) )
    {
        current_token = tokens.at( current_index );
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
    
}



//===----------------------------------------------------------------------===//
// Advance to the next token in tokens
//===----------------------------------------------------------------------===//
void Parser::advance()
{
    if ( current_index < tokens.size() )
    {
        ++current_index;
        current_token = tokens.at( current_index );
    }
    
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
