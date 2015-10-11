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
		case IDENT_T:			return "ID";
        case PRGRM_T:			return "PROGRAM";
        case CONST_T:			return "CONST";
        case BEGIN_T:			return "BEGIN";
        case END_T:				return "END";
        case PRINT_ST:			return "PRINT";
        case DIV_ST:			return "DIV";
        case MOD_ST:			return "MOD";
		case VAR_ID:			return "VAR";
		case INT_T:				return "INT";
		case BOOLEAN_T:			return "BOOL";
		case PROC_T:			return "PROC";
		case IF_T:				return "IF-THEN";
		case ELSE_T:			return "IF-THEN-ELSE";
		case WHILE_T:			return "WHILE";
		case DO_T:				return "DO";
		case PROMPT_T:			return "PROMPT";
		case AND_ST:			return "AND";
		case OR_ST:				return "OR";
		case NOT_ST:			return "NOT";
		case TRUE_T:			return "TRUE";
		case FALSE_T:			return "FALSE";
        case SINGLE0_T:			return "NUM";
        case INTEGER_T:			return "NUM";
        case SETEQUAL_T:		return "ASSIGN";
		case ISEQUAL_ST:		return "EQ";
		case LESSER_ST:			return "LT";
		case LESSEQUAL_ST:		return "LE";
		case NOTEQUAL_ST:		return "NE";
		case GREATER_ST:		return "GT";
		case GREQUAL_ST:		return "GE";
        case ADDITION_ST:		return "PLUS";
        case SUBTRACT_ST:		return "MINUS";
        case ASTERISK_ST:		return "TIMES";
		case COLON_T:			return "COLON";
        case SEMICOLON_T:		return "SEMI";
		case COMMA_T:			return "COMMA";
        case PERIOD_T:			return "PERIOD";
		case STRINGCONST_T:		return "STRING";
		case LEFTPARENTH_T:		return "PAREN-LEFT";
		case RIGHTPARENTH_T:	return "PAREN-RIGHT";
        case EOF_T:				return "EOF";
			
			
        // Still leave error cases here although next()
        // will have already checked to help future debugging
		case CRERRORCONST_T:	return "(!) Cannot insert new line in a string";
		case EOFERRORCONST_T:	return "(!) Found end-of-file while scanning a string";
        case SLASHERROR_T:		return "(!) Missing a second slash '/'";
        case BRACKETERROR_T:	return "(!) Missing a closing bracket '}'";
        case OTHERERROR:		return "(!) Unrecognized character";
            
        default:				return "";
    }
}


ostream& operator << ( ostream& out, const Token& token )
{
    if ( token.type == IDENT_T || token.type == SINGLE0_T || token.type == INTEGER_T || token.type == STRINGCONST_T || token.type == OTHERERROR )
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
	static int state_transition_table[ 11 ][ 23 ] = {
	//	0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17		18		19		20		21		22
	//	L		0		D		=		<		>		+		-		*		:		;		,		.		"		(		)		/		{		}		CR		WS		EOF		OTHER
		1,		163,	2,		3,		7,		8,		113,	114,	115,	164,	123,	124,	125,	9,		118,	119,	4,		6,		102,	0,		0,		101,	102,
		1,		1,		1,		100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,	100,
		130,	2,		2,		130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,	130,
		112,	112,	112,	111,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,	112,
		128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	128,	5,		128,	128,	128,	128,	128,	128,
		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		5,		0,		5,		0,		5,
		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		6,		0,		6,		6,		129,	6,
		107,	107,	107,	109,	107,	110,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,	107,
		106,	106,	106,	105,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,	106,
		9,		9,		9,		9,		9,		9,		9,		9,		9,		9,		9,		9,		9,		10,		9,		9,		9,		9,		9,		121,	9,		122,	9,
		120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	120,	9,		120,	120,	120,	120,	120,	120,	120,	120,	120
	};
    
    
    
    if ( isalpha( currentChar ) )
        column = 0;
    else if ( currentChar == '0' )
        column = 1;
    else if ( isdigit( currentChar ) )
        column = 2;
    else if ( currentChar == '=' )
        column = 3;
	else if ( currentChar == '<' )
		column = 4;
	else if ( currentChar == '>' )
		column = 5;
    else if ( currentChar == '+' )
        column = 6;
    else if ( currentChar == '-' )
        column = 7;
    else if ( currentChar == '*' )
        column = 8;
	else if ( currentChar == ':' )
		column = 9;
    else if ( currentChar == ';' )
        column = 10;
	else if ( currentChar == ',' )
		column = 11;
    else if ( currentChar == '.' )
        column = 12;
	else if ( currentChar == '"' )
		column = 13;
	else if ( currentChar == '(' )
		column = 14;
	else if ( currentChar == ')' )
		column = 15;
    else if ( currentChar == '/' )
        column = 16;
    else if ( currentChar == '{' )
        column = 17;
    else if ( currentChar == '}' )
        column = 18;
    else if ( currentChar == '\n' ) // compare with CR before WS, otherwise
        column = 19;                // will miss CR because WS includes CR
    else if ( isspace( currentChar ) )
        column = 20;
    else if ( currentChar == EOF )
        column = 21;
    else
        column = 22;
    
    
    
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
			case ISEQUAL_ST:		return current_state;
			case NOTEQUAL_ST:		return current_state;
			case LESSER_ST:			return current_state;
			case LESSEQUAL_ST:		return current_state;
			case GREATER_ST:		return current_state;
			case GREQUAL_ST:		return current_state;
            case ADDITION_ST:       return current_state;
            case SUBTRACT_ST:       return current_state;
            case ASTERISK_ST:       return current_state;
			case COLON_T:			return current_state;
            case SEMICOLON_T:       return current_state;
			case COMMA_T:			return current_state;
            case PERIOD_T:          return current_state;
			case STRINGCONST_T:		return current_state;
			case CRERRORCONST_T:	return current_state;
			case EOFERRORCONST_T:	return current_state;
			case LEFTPARENTH_T:		return current_state;
			case RIGHTPARENTH_T:	return current_state;
            case SLASHERROR_T:      return current_state;
            case BRACKETERROR_T:    return current_state;
			case EOF_T:             return current_state;
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
            
            // Subtract current column number by 1
			// (unless current column is 0)
			// to read again because we detect an identifier
			// by looking 1 character ahead.
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
			
			else if ( strcasecmp( current_lexeme.c_str(), "var" ) == 0 )
			{
				token.setToken( token_line, token_column, VAR_ID, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "int" ) == 0 )
			{
				token.setToken( token_line, token_column, INT_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "bool" ) == 0 )
			{
				token.setToken( token_line, token_column, BOOLEAN_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "proc" ) == 0 )
			{
				token.setToken( token_line, token_column, PROC_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "if" ) == 0 )
			{
				token.setToken( token_line, token_column, IF_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "else" ) == 0 )
			{
				token.setToken( token_line, token_column, ELSE_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "while" ) == 0 )
			{
				token.setToken( token_line, token_column, WHILE_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "do" ) == 0 )
			{
				token.setToken( token_line, token_column, DO_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "prompt" ) == 0 )
			{
				token.setToken( token_line, token_column, PROMPT_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "and" ) == 0 )
			{
				token.setToken( token_line, token_column, AND_ST, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "or" ) == 0 )
			{
				token.setToken( token_line, token_column, OR_ST, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "not" ) == 0 )
			{
				token.setToken( token_line, token_column, NOT_ST, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "true" ) == 0 )
			{
				token.setToken( token_line, token_column, TRUE_T, current_lexeme );
				current_lexeme = "";
				return token;
			}
			
			else if ( strcasecmp( current_lexeme.c_str(), "false" ) == 0 )
			{
				token.setToken( token_line, token_column, FALSE_T, current_lexeme );
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
			
		case ISEQUAL_ST:
			token.setToken( token_line, token_column, ISEQUAL_ST, current_lexeme );
			current_lexeme = "";
			return token;
			
		case LESSER_ST:
			if ( column_number > 1 )
				--column_number;

			current_lexeme.erase( current_lexeme.length() - 1, 1 );
			token.setToken( token_line, token_column, LESSER_ST, current_lexeme );
			current_lexeme = "";
			return token;
			
		case LESSEQUAL_ST:
			token.setToken( token_line, token_column, LESSEQUAL_ST, current_lexeme );
			current_lexeme = "";
			return token;
			
		case NOTEQUAL_ST:
			token.setToken( token_line, token_column, NOTEQUAL_ST, current_lexeme );
			current_lexeme = "";
			return token;
			
		case GREATER_ST:
			if ( column_number > 1 )
				--column_number;
			
			current_lexeme.erase( current_lexeme.length() - 1, 1 );
			token.setToken( token_line, token_column, GREATER_ST, current_lexeme );
			current_lexeme = "";
			return token;
			
		case GREQUAL_ST:
			token.setToken( token_line, token_column, GREQUAL_ST, current_lexeme );
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
			
		case COLON_T:
			token.setToken( token_line, token_column, COLON_T, current_lexeme );
			current_lexeme = "";
			return token;
			
        case SEMICOLON_T:
            token.setToken( token_line, token_column, SEMICOLON_T, current_lexeme );
            current_lexeme = "";
            return token;

		case COMMA_T:
			token.setToken( token_line, token_column, COMMA_T, current_lexeme );
			current_lexeme = "";
			return token;
			
		case PERIOD_T:
			token.setToken( token_line, token_column, PERIOD_T, current_lexeme );
			current_lexeme = "";
			return token;
			
		case STRINGCONST_T:
			if ( column_number > 1 )
				--column_number;
			
			current_lexeme.erase( current_lexeme.length() - 1, 1 ); //erase the last char and the last quotation mark
			token.setToken( token_line, token_column, STRINGCONST_T, current_lexeme );
			current_lexeme = "";
			return token;
			
		case CRERRORCONST_T:
			token.setToken( token_line, token_column, CRERRORCONST_T, current_lexeme );
			current_lexeme = "";
			cout << "(!) Cannot insert new line in a string " << token.line << ":" << token.column << endl;
			exit( 1 );
			
		case EOFERRORCONST_T:
			token.setToken( token_line, token_column, EOFERRORCONST_T, current_lexeme );
			current_lexeme = "";
			cout << "(!) Cannot end file while scanning a string " << token.line << ":" << token.column << endl;
			exit( 1 );
			
		case LEFTPARENTH_T:
			token.setToken( token_line, token_column, LEFTPARENTH_T, current_lexeme );
			current_lexeme = "";
			return token;
			
		case RIGHTPARENTH_T:
			token.setToken( token_line, token_column, RIGHTPARENTH_T, current_lexeme );
			current_lexeme = "";
			return token;
			
        case EOF_T:
            token.setToken( token_line, token_column, EOF_T, current_lexeme );
            current_lexeme = "";
            return token;
            
        case SLASHERROR_T:
            token.setToken( token_line, token_column, SLASHERROR_T, current_lexeme );
            current_lexeme = "";
            cout << "(!) Missing a second slash '/' " << token.line << ":" << token.column << endl;
            exit( 1 );
            
            
        case BRACKETERROR_T:
            token.setToken( token_line, token_column, BRACKETERROR_T, current_lexeme );
            current_lexeme = "";
            cout << "(!) Missing a closing bracket '}' " << token.line << ":" << token.column << endl;
            exit( 1 );
        
        case OTHERERROR:
            token.setToken( token_line, token_column, OTHERERROR, current_lexeme );
            current_lexeme = "";
            cout << "(!) Unrecognized character " << token.lexeme << " " << token.line << ":" << token.column << endl;
            exit( 1 );
            
            
        // switch statement is exhaustive
        // This line is just to make Xcode happy.
        default:
            return token;
            
    }
}
