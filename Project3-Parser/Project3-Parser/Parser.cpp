//
//  Parser.cpp
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "Parser.h"
#include "AST.h"

//===----------------------------------------------------------------------===//
// Parser constructor:
//
// - Initialize scanner in initilization list because it is a const
// thus cannot be initialized (aka changed) inside the constructor.
// - Parse
//===----------------------------------------------------------------------===//
Parser::Parser( ifstream& i ) : scanner( i )
{
    token = scanner.next();
    parse();
}



//===----------------------------------------------------------------------===//
// Scan the next token
//===----------------------------------------------------------------------===//
Token Parser::advance()
{
    Token current_token = token;
    token = scanner.next();
    return current_token;
}



//===----------------------------------------------------------------------===//
// Match current token's type with each standard statement structure
// If it's matched, advance and return the old token
//===----------------------------------------------------------------------===//
Token Parser::match( int tokenType )
{
    if ( check( tokenType ) )
        return advance();
    else
    {
        cout << "(!) Expected " << nameOf( tokenType ) << " at " << token.line << ":" << token.column << endl;
        exit( 1 );
    }
}



//===----------------------------------------------------------------------===//
// Check whether the current token's type equal tokenType
//===----------------------------------------------------------------------===//
bool Parser::check( int tokenType )
{
    // NUM case to match both SINGLE0_T and INTEGER_T
    if ( nameOf( tokenType ) == "NUM" && nameOf( token.type ) == "NUM" )
        return true;
    else if ( token.type == tokenType )
        return true;
    else
        return false;
}



//===----------------------------------------------------------------------===//
// Parsers based on grammar in Grammar.txt
//
ASTNode* Parser::parseProgram()
{
    match( PRGRM_T );
    Token token = match( IDENT_T );
    match( SEMICOLON_T );
    Block* block = parseBlock();
    match( PERIOD_T );
	
	Program* node = new Program( token.lexeme, block, token.line, token.column );
	return node;
}



Block* Parser::parseBlock()
{
	list<ConstDecl*> constDecls = parseConstDecls();
	list<VarDecl*> varDecls = parseVarDecls();
	list<ProcDecl*> procDecls = parseProcDecls();
    match( BEGIN_T );
	list<Stmt*> stmts = parseStmts();
    match( END_T );
	
	ConstDecl* first = constDecls.front();
	Block* node = new Block( constDecls, varDecls, procDecls, stmts, first->line, first->column );
	return node;
}



ConstDecl* Parser::parseConstDecl()
{
	Token constant = match( CONST_T );
	Token ID = match( IDENT_T );
	match( SETEQUAL_T );
	string sign = parseSign();
	Token value = match( INTEGER_T );
	match( SEMICOLON_T );
	
	int val = stoi( value.lexeme );
	if ( sign.compare("-") == 0 )
	{
		val = -val;
	}
	
	ConstDecl* node = new ConstDecl( ID.lexeme, val, constant.line, constant.column );
	return node;
}



VarDecl* Parser::parseVarDecl()
{
	Token var = match( VAR_ID );
	Token ID = match( IDENT_T );
	match( COLON_T );
	Type type = parseType();
	match( SEMICOLON_T );
	
	VarDecl* node = new VarDecl( ID.lexeme, type, var.line, var.column );
	return node;
}



ProcDecl* Parser::parseProcDecl()
{
	Token procedure = match( PROC_T );
	Token ID = match( IDENT_T );
	list<Param*> params = parseParamList();
	match( SEMICOLON_T );
	Block* block = parseBlock();
	match( SEMICOLON_T );
	
	ProcDecl* node = new ProcDecl( ID.lexeme, params, block, procedure.line, procedure.column );
	return node;
}



Stmt* Parser::parseStmt()
{
	if ( check( IDENT_T ) )
	{
		Token ID = match( IDENT_T );
		Stmt* node = parseStmtID( ID.lexeme, ID.line, ID.column );
		match( SEMICOLON_T );
		
		return node;
	}
	else if ( check( BEGIN_T ) )
	{
		Token begin = match( BEGIN_T );
		list<Stmt*> body = parseStmts();
		match( END_T );
		match( SEMICOLON_T );
		
		Sequence* node = new Sequence( body, begin.line, begin.column );
		return node;
	}
	else if ( check( IF_T ) )
	{
		Token ifToken = match( IF_T );
		Expr* test = parseExpr();
		match( THEN_T );
		Stmt* trueClause = parseStmt();
		Stmt* node = parseStmtIf( test, trueClause, ifToken.line, ifToken.column );
		
		return node;
	}
	else if ( check( WHILE_T ) )
	{
		Token whileToken = match( WHILE_T );
		Expr* test = parseExpr();
		match( DO_T );
		Stmt* body = parseStmt();
		
		While* node = new While( test, body, whileToken.line, whileToken.column );
		return node;
	}
	else if ( check( PROMPT_T ) )
	{
		Token prompt = match( PROMPT_T );
		Token message = match( STRINGCONST_T );
		Stmt* node = parseStmtPrompt( message.lexeme, prompt.line, prompt.column );
		match( SEMICOLON_T );
		
		return node;
	}
	else if ( check( PRINT_ST )	)
	{
		Token print = match( PRINT_ST );
		list<Item*> items = parseItems();
		match( SEMICOLON_T );
		
		Print* node = new Print( items, print.line, print.column );
		return node;
	}
	
	return NULL;
}



Stmt* Parser::parseStmtID( string i, int lin, int col )
{
	if ( check( SETEQUAL_T ) )
	{
		match( SETEQUAL_T );
		Expr* expr = parseExpr();
		Assign* node = new Assign( i, expr, lin, col );
		return node;
	}
	else
	{
		list<Expr*> args = parseArgList();
		Call* node = new Call( i, args, lin, col );
		return node;
	}
	
	return NULL;
}



Stmt* Parser::parseStmtIf( Expr* t, Stmt* tr, int lin, int col )
{
	if ( check( ELSE_T ) )
	{
		match( ELSE_T );
		Stmt* falseClause = parseStmt();
		IfThenElse* node = new IfThenElse( t, tr, falseClause, lin, col );
		return node;
	}
	else
	{
		IfThen* node = new IfThen( t, tr, lin, col );
		return node;
	}
	
	return NULL;
}



Stmt* Parser::parseStmtPrompt( string m, int lin, int col )
{
	if ( check( COMMA_T ) )
	{
		match( COMMA_T );
		Token ID = match( IDENT_T );
		Prompt2* node = new Prompt2( m, ID.lexeme, lin, col );
		return node;
	}
	else
	{
		Prompt* node = new Prompt( m, lin, col );
		return node;
	}
	
	return NULL;
}


Param* Parser::parseParam()
{
	if ( check( IDENT_T ) )
	{
		Token ID = match( IDENT_T );
		match( COLON_T );
		Type type = parseType();
		ValParam* node = new ValParam( ID.lexeme, type, ID.line, ID.column );
		return node;
	}
	else
	{
		Token var = match( VAR_ID );
		Token ID = match( IDENT_T );
		match( COLON_T );
		Type type = parseType();
		VarParam* node = new VarParam( ID.lexeme, type, ID.line, ID.column );
		return node;
	}
	
	return NULL;
}



ConstDecl* Parser::parseConstDecl()
{
    if ( check( CONST_T ) )
    {
		
        parseConstantDeclaration(); //return list
        parseConstantDeclarations();
    }
    else {} //return list
}



//===---------------------------------------===//
// Add the matched constant to constants hash map
//




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
		unordered_map<string, int>::const_iterator isDelcared = constants.find(id.lexeme);

		if (isDelcared == constants.end())
		{
			cout << "(!) Constant " << id.lexeme << " at " << token.line << ":" << token.column << " was not declared" << endl;
			exit( 1 );
		}
		else
		{
			int value = constants.at( id.lexeme );
			cout << value << endl;
		}
    }
    else
    {
        cout << "(!) Expected either NUM or ID at " << token.line << ":" << token.column << endl;
        exit( 1 );
    }
}
//
// !Baby parsers
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// The main Parser
//===----------------------------------------------------------------------===//
void Parser::parse()
{
    parseProgram();
    match( EOF_T );
}
