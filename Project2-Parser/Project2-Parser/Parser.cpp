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
    token_index = 0;
    
    do
    {
        token = scanner.next();
        
        if ( noSyntaxError( token ) )
            tokens.push_back( token );

    } while ( token.type != EOF_T );
    
    
    // construction
    current_token_index = 0;
    // !construction

    current_token = tokens.at( 0 );
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
// Match current_token's type with each standard statement structure
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
                        return true;
                    else
                    {
                        cout << "(!) Expected a semicolon to follow " << tokens.at( current_token.index + 1 ) << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier to follow " << current_token << endl;
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
                                return true;
                            else
                            {
                                cout << "(!) Expected a semicolon to follow " << tokens.at( current_token.index + 3 ) << endl;
                                return false;
                            }
                        }
                        else
                        {
                            cout << "(!) Expected a number to follow " << tokens.at( current_token.index + 2 ) << endl;
                            return false;
                        }
                    }
                    else
                    {
                        cout << "(!) Expected an equal sign to follow " << tokens.at( current_token.index + 1 ) << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier to follow " << current_token << endl;
                    return false;
                }
            }
            else
                return false;
            
            
            
        case BEGIN_T:
            if ( current_token.type == BEGIN_T )
                return true;
            else
                return false;
            
            
            
        case END_T:
            if ( current_token.type == END_T )
            {
                if ( tokens.at( current_token.index + 1 ).type == PERIOD_T )
                    return true;
                else
                {
                    cout << "(!) Expected a period to follow " << current_token << endl;
                    return false;
                }
                
            }
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
            
            
        case INTEGER_T: //match both INTEGER_T and SINGLE0_T since they both represent type NUM
            if ( current_token.type == INTEGER_T || current_token.type == SINGLE0_T )
                return true;
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
    
}



void Parser::parseBlock()
{
    
}



void Parser::parseConstantDeclarations()
{
    
}



void Parser::parseConstantDeclaration()
{
    
}



void Parser::parseStatements()
{
    
}



void Parser::parseStatement()
{
    
}



void Parser::parseExpression()
{
    
}



void Parser::parseTerm()
{
    
}



void Parser::parseFactor()
{
    
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
    if ( current_token_index < tokens.size() )
    {
        ++current_token_index;
        current_token = tokens.at( current_token_index );
    }
    
}



//===----------------------------------------------------------------------===//
// Check if tokens has any grammar error
//===----------------------------------------------------------------------===//
bool Parser::noGrammarError()
{
    while ( current_token.type != EOF_T )
    {
        switch ( current_token.type )
        {
            case PRGRM_T:
                advance();
                if ( current_token.type == IDENT_T )
                {
                    advance();
                    if ( current_token.type == SEMICOLON_T )
                        {
                            advance();
                            //no break statement to advance to case CONST_T since CONST_T needs to follow PRGRM_T according to grammar
                        }
                        else
                        {
                            cout << "(!) Expected a semicolon to follow " << current_token << endl;
                            return false;
                        }
                    }
                else
                {
                    cout << "(!) Expected an identifier to follow " << current_token << endl;
                    return false;
                }
                
                
                
            case CONST_T:
                advance();
                if ( current_token.type == IDENT_T )
                {
                    advance();
                    if ( current_token.type == SINGLE0_T || current_token.type == INTEGER_T )
                    {
                        advance();
                        if ( current_token.type == SEMICOLON_T )
                        {
                            advance();
                            break;
                        }
                    }
                    else
                    {
                        cout << "(!) Expected a number to follow " << current_token << endl;
                        return false;
                    }
                }
                else
                {
                    cout << "(!) Expected an identifier to follow " << current_token << endl;
                    return false;
                }
                    
               


            
        }
    }
    return true;
}


//===----------------------------------------------------------------------===//
// Check if a signal Token is followed by
// tokens that match the standard format of an expression
//===----------------------------------------------------------------------===//
bool Parser::matches( int tokenType )
{
    vector<int> token_list;
    
    switch ( tokenType )
    {
        case PRGRM_T:
            if ( current_token.type == PRGRM_T )
            {
                
                advance();
                if ( current_token.type == IDENT_T )
                {
                    
                    advance();
                    if ( current_token.type == INTEGER_T || current_token.type == SINGLE0_T )
                    {
                        
                        advance();
                        if ( current_token.type == SEMICOLON_T )
                        {
                            return true;
                        }
                        else
                        {
                            cout << "(!) Expected a semicolon to follow " << current_token << endl;
                            exit( 1 );
                        }
                        
                    }
                    else
                    {
                        cout << "(!) Expected one or more digits to follow " << current_token << endl;
                        exit( 1 );
                    }
                    
                }
                else
                {
                    cout << "(!) Expected an identifier to follow " << current_token << endl;
                    exit( 1 );
                }
                
            }
            else
                return false;
            
            
        
        case CONST_T:
            if ( current_token.type == CONST_T )
            {
                
                advance();
                if ( current_token.type == IDENT_T )
                {
                    
                    advance();
                }
                else
                {
                    cout << "(!) Expected an identifier to follow " << endl;
                }
                
            }
            else
                return false;
            break;
            
            
        
    }
    return false;
}







//===----------------------------------------------------------------------===//
// Parse
//===----------------------------------------------------------------------===//
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
                    string id = tokens.at( current_token_index - 2 ).lexeme;
                    int num = stoi( tokens.at( current_token_index - 1 ).lexeme );
                    
                    constants_map.insert( { id, num } );
                    
                }
                
                advance();
                
            }
            
            //work through Howard's pseudocode
            
            
        }
        
        advance();
    }
}
