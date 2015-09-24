//
//  Scanner.cpp
//  Project1-Scanner
//
//  Created by Truc Le on 9/6/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "Scanner.h"



//===----------------------------------------------------------------------===//
// Format a Token for display on cout
//
string nameOf( int tokenType )
{
    switch ( tokenType )
    {
        case PRGRM_T:
            return "PROGRAM";
            
        case CONST_T:
            return "CONST";
            
        case BEGIN_T:
            return "BEGIN";
            
        case END_T:
            return "END";
            
        case PRINT_ST:
            return "PRINT";
            
        case DIV_ST:
            return "DIV";
            
        case MOD_ST:
            return "MOD";
            
        case IDENT_T:
            return "ID";
            
        case SINGLE0_T:
            return "NUM";
            
        case INTEGER_T:
            return "NUM";
            
        case SETEQUAL_T:
            return "ASSIGN";
            
        case ADDITION_ST:
            return "PLUS";
            
        case SUBTRACT_ST:
            return "MINUS";
            
        case ASTERISK_ST:
            return "STAR";
            
        case SEMICOLON_T:
            return "SEMI";
            
        case PERIOD_T:
            return "PERIOD";
            
        case EOF_T:
            return "EOF";
            
        case SLASHERROR_T:
            return "(!) Missing a second slash '/'";
            
        case BRACKETERROR_T:
            return "(!) Missing a closing bracket '}'";
            
        case OTHERERROR:
            return "(!) Unrecognized character";
            
        default:
            return "";
    }
}


ostream& operator << ( ostream& out, const Token& token )
{
    if ( token.type == IDENT_T ||
         token.type == SINGLE0_T ||
         token.type == INTEGER_T ||
         token.type == OTHERERROR )
        out << nameOf( token.type ) << " " << token.lexeme << " " << token.line << ":" << token.column;
    else
        out << nameOf( token.type ) << " " << token.line << ":" << token.column;
    
    return out;
}
//
// !Format a Token for display on cout
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// Token constructor
//===----------------------------------------------------------------------===//
Token::Token()
{
}



//===------------------------------===//
// Set token
//===------------------------------===//
void Token::setToken( int l, int c, int t, string lex )
{
    line    = l;
    column  = c;
    type    = t;
    lexeme  = lex;
}





//===----------------------------------------------------------------------===//
// Scanner constructor
//===----------------------------------------------------------------------===//
Scanner::Scanner( ifstream &i ) : input( i )
{
    line_number     = 1;
    column_number   = 1;
    getline( i, current_line ); //Set current line to first line of file
}



//===------------------------------===//
// Get current character
//===------------------------------===//
char Scanner::getCurrentChar()
{
    if ( column_number <= current_line.size() )
        // Still within current line
        return current_line[ column_number - 1 ];
    
    else if ( input )
        // At the end of current line
        // but input is still valid
        return '\n';
    
    else
        // Past end of input
        return char_traits<char>::eof();
}



//===------------------------------===//
// Advance to next character
//===------------------------------===//
void Scanner::advance()
{
    if ( column_number <= current_line.size() )
        // Still within current line
        ++column_number;
    
    else if ( input )
    {
        // At the end of current line,
        // attempt to read another line
        current_line.clear();
        getline( input, current_line );
        ++line_number;
        column_number = 1;
    }
    
    else {} // At EOF, do nothing
}



//===------------------------------===//
// Returns the next state by matching
// currentState (row) to currentChar (column)
// in state_transition_table
//===------------------------------===//
int Scanner::getNextState( int currentState, char currentChar )
{
    int column;
    static int state_transition_table[ 7 ][ 16 ] = {
    //  0       1       2       3       4       5       6       7       8       9       10      11      12      13      14      15
    //  L       0       D       =       +       -       *       ;       .       /       {       }       CR      WS      EOF     OTHER
        1,      163,	2,      3,      113,	114,	115,	123,	125,	4,      6,      102,	0,      0,      101,	102,
        1,      1,      1,      100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,
        130,	2,      2,      130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,
        112,	112,	112,	111,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,
        128,	128,	128,	128,	128,	128,	128,	128,	128,	5,      128,	128,	128,	128,	128,	128,
        5,      5,      5,      5,      5,      5,      5,      5,      5,      5,      5,      5,      0,      5,      0,      5,
        6,      6,      6,      6,      6,      6,      6,      6,      6,      6,      6,      0,      6,      6,      129,	6
    };
    
    
    
    if ( isalpha( currentChar ) )
        column = 0;
    else if ( currentChar == '0' )
        column = 1;
    else if ( isdigit( currentChar ) )
        column = 2;
    else if ( currentChar == '=' )
        column = 3;
    else if ( currentChar == '+' )
        column = 4;
    else if ( currentChar == '-' )
        column = 5;
    else if ( currentChar == '*' )
        column = 6;
    else if ( currentChar == ';' )
        column = 7;
    else if ( currentChar == '.' )
        column = 8;
    else if ( currentChar == '/' )
        column = 9;
    else if ( currentChar == '{' )
        column = 10;
    else if ( currentChar == '}' )
        column = 11;
    else if ( currentChar == '\n' ) // compare with CR before WS, otherwise
        column = 12;                // will miss CR because WS includes CR
    else if ( isspace( currentChar ) )
        column = 13;
    else if ( currentChar == EOF )
        column = 14;
    else
        column = 15;
    
    
    
    return state_transition_table[ currentState ][ column ];
    
}



//===------------------------------===//
// Get the next terminal state
// Handle error input
//===------------------------------===//
int Scanner::getNextTerminalState()
{
    int current_state = 0;
    char current_char;
    
    do
    {
        current_char    = getCurrentChar();
        current_lexeme  = current_lexeme + current_char;
        
        
        
        // Check whether we found a comment
        string bracket_comment  = current_lexeme.substr( 0, 1 );
        string slash_comment    = current_lexeme.substr( 0, 2 );
        
        if ( ( bracket_comment == "{" && current_char == '}' ) || ( slash_comment == "//" && current_char == '\n' ) )
            current_lexeme = "";
        
        
        
        // Erase all whitespaces if the whitespace
        // is the only thing in current lexeme
        if ( isspace( current_char ) )
            if ( current_lexeme.length() == 1 )
                current_lexeme.erase( current_lexeme.length() - 1, 1 );
        
        
        
        // Mark beginning line and column of lexeme
        if ( current_lexeme.length() == 1 )
        {
            token_line      = line_number;
            token_column    = column_number;
        }
        
        
        
        // Get the next state
        current_state = getNextState( current_state, current_char );
        
        
        
        // Advance to the next character
        // for use in the subsequent loop
        advance();
        
        
        
        switch ( current_state )
        {
            case IDENT_T:           return current_state;
            case SINGLE0_T:         return current_state;
            case INTEGER_T:         return current_state;
            case SETEQUAL_T:        return current_state;
            case ADDITION_ST:       return current_state;
            case SUBTRACT_ST:       return current_state;
            case ASTERISK_ST:       return current_state;
            case SEMICOLON_T:       return current_state;
            case PERIOD_T:          return current_state;
            case EOF_T:             return current_state;
            case SLASHERROR_T:      return current_state;
            case BRACKETERROR_T:    return current_state;
            case OTHERERROR:        return current_state;
                
        }
        
    } while ( 1 );
    // Keep getting the next state until
    // found terminal state of the current lexeme
    
}



//===------------------------------===//
// Get the next Token from input
// Locate error input
//===------------------------------===//
Token Scanner::next()
{
    Token token;
    int state = getNextTerminalState();
    
    switch ( state )
    {
        case IDENT_T:
            
            // Subtract current column number by 1 to
            // read again since we just found a Token
            // except for the first input line
            if ( column_number > 1 )
                --column_number;
            
            // Erase the latest character since it does not
            // belong to the identifier and we only used it
            // to determine the identifier's end
            current_lexeme.erase( current_lexeme.length() - 1, 1 );
            
            
            // Create a pointer to the identifier using c_str
            // Compare lowercase identifier to each keyword using _stricmp
            // Check if identifier is a keyword
            if ( strcasecmp( current_lexeme.c_str(), "program" ) == 0 )
            {
                //    setToken( line, column, type, index, lexeme )
                token.setToken( token_line, token_column, PRGRM_T, current_lexeme );
                
                // Intentionally repeat the 2 lines below in each case so that
                // next() can end immediately after it matched a condition
                // and doesn't need to go through every other case
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "const" ) == 0 )
            {
                token.setToken( token_line, token_column, CONST_T, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "begin" ) == 0 )
            {
                token.setToken( token_line, token_column, BEGIN_T, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "end" ) == 0 )
            {
                token.setToken( token_line, token_column, END_T, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "print" ) == 0 )
            {
                token.setToken( token_line, token_column, PRINT_ST, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "div" ) == 0 )
            {
                token.setToken( token_line, token_column, DIV_ST, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else if ( strcasecmp( current_lexeme.c_str(), "mod" ) == 0 )
            {
                token.setToken( token_line, token_column, MOD_ST, current_lexeme );
                current_lexeme = "";
                return token;
                
            }
            
            else
            {
                token.setToken( token_line, token_column, IDENT_T, current_lexeme );
                current_lexeme = "";
                return token;
            }
            
            
         
        case SINGLE0_T:
            // Does not have to subtract last column number
            // or earse last character because single 0 (and
            // some other identifiers) ends on itself
            token.setToken( token_line, token_column, SINGLE0_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case INTEGER_T:
            if ( column_number > 1 )
                --column_number;
            
            current_lexeme.erase( current_lexeme.length() - 1, 1 );
            token.setToken( token_line, token_column, INTEGER_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case SETEQUAL_T:
            if ( column_number > 1 )
                --column_number;
            
            current_lexeme.erase( current_lexeme.length() - 1, 1 );
            token.setToken( token_line, token_column, SETEQUAL_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case ADDITION_ST:
            token.setToken( token_line, token_column, ADDITION_ST, current_lexeme );
            current_lexeme = "";
            return token;
            
        case SUBTRACT_ST:
            token.setToken( token_line, token_column, SUBTRACT_ST, current_lexeme );
            current_lexeme = "";
            return token;
            
        case ASTERISK_ST:
            token.setToken( token_line, token_column, ASTERISK_ST, current_lexeme );
            current_lexeme = "";
            return token;
            
        case SEMICOLON_T:
            token.setToken( token_line, token_column, SEMICOLON_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case PERIOD_T:
            token.setToken( token_line, token_column, PERIOD_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case EOF_T:
            token.setToken( token_line, token_column, EOF_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case SLASHERROR_T:
            token.setToken( token_line, token_column, SLASHERROR_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case BRACKETERROR_T:
            token.setToken( token_line, token_column, BRACKETERROR_T, current_lexeme );
            current_lexeme = "";
            return token;
        
        case OTHERERROR:
            token.setToken( token_line, token_column, OTHERERROR, current_lexeme );
            current_lexeme = "";
            return token;
            
    }
    
    // switch statement is exhaustive
    // This line is just to make Xcode happy.
    return token;
    
}
