//
//  Parser.cpp
//	Project4-Interpreter
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "Parser.h"


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
		//Strategy 1: print error and end -- used
		//Strategy 2: ptint error and return the expected Token with empty lexeme
		//Strategy 3: keep skipping over the rest of the Tokens until found the expected Token
		
		cout << "(!) Expected " << nameOf( tokenType ) << " at " << token.line << ":" << token.column << endl;
		//Token t = *new Token();
		//return t;
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
Program* Parser::parseProgram()
{
	Token program = match( PRGRM_T );
	Token ID = match( IDENT_T );
	match( SEMICOLON_T );
	Block* block = parseBlock();
	match( PERIOD_T );
	
	Program* node = new Program( ID.lexeme, block, program.line, program.column );
	return node;
}



Block* Parser::parseBlock()
{
	list<ConstDecl*> constDecls = parseConstDecls();
	list<VarDecl*> varDecls = parseVarDecls();
	list<ProcDecl*> procDecls = parseProcDecls();
	Token begin = match( BEGIN_T );
	list<Stmt*> stmts = parseStmts();
	match( END_T );
	
	Block* node = new Block( constDecls, varDecls, procDecls, stmts, begin.line, begin.column );
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
		val = -val;
	
	ConstDecl* node = new ConstDecl( ID.lexeme, val, constant.line, constant.column );
	return node;
}



VarDecl* Parser::parseVarDecl()
{
	Token var = match( VAR_ID );
	Token ID = match( IDENT_T );
	match( COLON_T );
	DataType type = parseType();
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
	else
	{
		Token print = match( PRINT_ST );
		list<Item*> items = parseItems();
		match( SEMICOLON_T );
		Print* node = new Print( items, print.line, print.column );
		
		return node;
	}
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
}


Param* Parser::parseParam()
{
	if ( check( IDENT_T ) )
	{
		Token ID = match( IDENT_T );
		match( COLON_T );
		DataType type = parseType();
		ValParam* node = new ValParam( ID.lexeme, type, ID.line, ID.column );
		return node;
	}
	else
	{
		Token var = match( VAR_ID );
		Token ID = match( IDENT_T );
		match( COLON_T );
		DataType type = parseType();
		VarParam* node = new VarParam( ID.lexeme, type, var.line, var.column );
		return node;
	}
}



Expr* Parser::parseExpr()
{
	Expr* simpleExpr = parseSimpleExpr();
	Expr* node = parseExprRest( simpleExpr, simpleExpr->line, simpleExpr->column );
	return node;
}



Expr* Parser::parseExprRest( Expr* e, int lin, int col )
{
	if ( check( ISEQUAL_ST ) || check( NOTEQUAL_ST ) || check( LESSER_ST ) || check( GREATER_ST ) || check( LESSEQUAL_ST ) || check( GREQUAL_ST ) )
	{
		Op2 relOp = parseRelOp();
		Expr* simpleExpr = parseSimpleExpr();
		BinOp* node = new BinOp( e, relOp, simpleExpr, lin, col );
		
		return node;
	}
	else
		return e;
}



Expr* Parser::parseSimpleExpr()
{
	Expr* term = parseTerm();
	Expr* node = parseSimpleExprRest( term, term->line, term->column );
	
	return node;
}



Expr* Parser::parseSimpleExprRest( Expr* e, int lin, int col )
{
	if ( check( ADDITION_ST ) || check( SUBTRACT_ST ) || check( OR_ST ) )
	{
		Op2 addOp = parseAddOp();
		Expr* term = parseTerm();
		BinOp* binOp = new BinOp( e, addOp, term, lin, col );
		Expr* node = parseSimpleExprRest( binOp, binOp->line, binOp->column );
		
		return node;
	}
	else
		return e;
}



Expr* Parser::parseTerm()
{
	Expr* factor = parseFactor();
	Expr* node = parseTermRest( factor, factor->line, factor->column );
	
	return node;
}



Expr* Parser::parseTermRest( Expr* e, int lin, int col )
{
	if ( check( ASTERISK_ST ) || check( DIV_ST ) || check( MOD_ST ) || check( AND_ST ) )
	{
		Op2 mulOp = parseMulOp();
		Expr* factor = parseFactor();
		BinOp* binOp = new BinOp( e, mulOp, factor, lin, col );
		Expr* node = parseTermRest( binOp, binOp->line, binOp->column );
		
		return node;
	}
	else
		return e;
}



Expr* Parser::parseFactor()
{
	if ( check( INTEGER_T ) )
	{
		Token num = match( INTEGER_T );
		Num* node = new Num( stoi( num.lexeme ), num.line, num.column );
		
		return node;
	}
	else if ( check( IDENT_T ) )
	{
		Token ID = match( IDENT_T );
		Id* node = new Id( ID.lexeme, ID.line, ID.column );
		
		return node;
	}
	else if ( check( TRUE_T ) )
	{
		Token trueToken = match( TRUE_T );
		True* node = new True( trueToken.line, trueToken.column );
		
		return node;
	}
	else if ( check( FALSE_T ) )
	{
		Token falseToken = match( FALSE_T );
		False* node = new False( falseToken.line, falseToken.column );
		
		return node;
	}
	else if ( check( SUBTRACT_ST ) )
	{
		Token unOp = match( SUBTRACT_ST );
		Expr* factor = parseFactor();
		UnOp* node = new UnOp( Neg, factor, unOp.line, unOp.column );
		
		return node;
	}
	else if ( check( NOT_ST ) )
	{
		Token unOp = match( NOT_ST );
		Expr* factor = parseFactor();
		UnOp* node = new UnOp( Not, factor, unOp.line, unOp.column );
		
		return node;
	}
	else
	{
		Token leftParen = match( LEFTPARENTH_T );
		Expr* node = parseExpr();
		match( RIGHTPARENTH_T );
		
		return node;
	}
}



Item* Parser::parseItem()
{
	if ( check( STRINGCONST_T ) )
	{
		Token stringToken = match( STRINGCONST_T );
		StringItem* node = new StringItem( stringToken.lexeme, stringToken.line, stringToken.column );
		
		return node;
	}
	else
	{
		Expr* expr = parseExpr();
		ExprItem* node = new ExprItem( expr, expr->line, expr->column );
		
		return node;
	}
}



Op2 Parser::parseRelOp()
{
	if ( check( ISEQUAL_ST ) )
	{
		match( ISEQUAL_ST );
		return EQ;
	}
	else if ( check( NOTEQUAL_ST ) )
	{
		match( NOTEQUAL_ST );
		return NE;
	}
	else if ( check( LESSER_ST ) )
	{
		match( LESSER_ST );
		return LT;
	}
	else if ( check( GREATER_ST ) )
	{
		match( GREATER_ST );
		return GT;
	}
	else if ( check( LESSEQUAL_ST ) )
	{
		match( LESSEQUAL_ST );
		return LE;
	}
	else
	{
		match( GREQUAL_ST );
		return GE;
	}
}



Op2 Parser::parseAddOp()
{
	if ( check( ADDITION_ST ) )
	{
		match( ADDITION_ST );
		return Plus;
	}
	else if ( check( SUBTRACT_ST ) )
	{
		match( SUBTRACT_ST );
		return Minus;
	}
	else
	{
		match( OR_ST );
		return Or;
	}
}



Op2 Parser::parseMulOp()
{
	if ( check( ASTERISK_ST ) )
	{
		match( ASTERISK_ST );
		return Times;
	}
	else if ( check( DIV_ST ) )
	{
		match( DIV_ST );
		return Div;
	}
	else if ( check( MOD_ST ) )
	{
		match( MOD_ST );
		return Mod;
	}
	else
	{
		match( AND_ST );
		return And;
	}
}



string Parser::parseSign()
{
	if ( check( SUBTRACT_ST ) )
	{
		match( SUBTRACT_ST );
		return "-";
	}
	else
		return "";
}



DataType Parser::parseType()
{
	if ( check( INT_T ) )
	{
		match( INT_T );
		return IntType;
	}
	else
	{
		match( BOOLEAN_T );
		return BoolType;
	}
}



list<ConstDecl*> Parser::parseConstDecls()
{
	if ( check( CONST_T ) )
	{
		ConstDecl* constDecl = parseConstDecl();
		list<ConstDecl*> constDecls = parseConstDecls();
		constDecls.push_front( constDecl );
		return constDecls;
	}
	else
		return list<ConstDecl*>();
}



list<VarDecl*> Parser::parseVarDecls()
{
	if ( check( VAR_ID ) )
	{
		VarDecl* varDecl = parseVarDecl();
		list<VarDecl*> varDecls = parseVarDecls();
		varDecls.push_front( varDecl );
		return varDecls;
	}
	else
		return list<VarDecl*>();
}



list<ProcDecl*> Parser::parseProcDecls()
{
	if ( check( PROC_T ) )
	{
		ProcDecl* procDecl = parseProcDecl();
		list<ProcDecl*> procDecls = parseProcDecls();
		procDecls.push_front( procDecl );
		return procDecls;
	}
	else
		return list<ProcDecl*>();
}



list<Stmt*> Parser::parseStmts()
{
	if ( check( IDENT_T ) || check( BEGIN_T ) || check( IF_T ) || check( WHILE_T ) || check( PROMPT_T ) || check( PRINT_ST ) )
	{
		Stmt* stmt = parseStmt();
		list<Stmt*> stmts = parseStmts();
		stmts.push_front( stmt );
		return stmts;
	}
	else
		return list<Stmt*>();
}



list<Param*> Parser::parseParamList()
{
	if ( check( LEFTPARENTH_T ) )
	{
		match( LEFTPARENTH_T );
		list<Param*> params = parseParams();
		match( RIGHTPARENTH_T );
		return params;
	}
	else
		return list<Param*>();
}



list<Param*> Parser::parseParams()
{
	Param* param = parseParam();
	list<Param*> paramRest = parseParamRest();
	paramRest.push_front( param );
	return paramRest;
}



list<Param*> Parser::parseParamRest()
{
	if ( check( COMMA_T ) )
	{
		match( COMMA_T );
		list<Param*> params = parseParams();
		return params;
	}
	else
		return list<Param*>();
}



list<Expr*> Parser::parseArgList()
{
	if ( check( LEFTPARENTH_T ) )
	{
		match( LEFTPARENTH_T );
		list<Expr*> args = parseArgs();
		match( RIGHTPARENTH_T );
		return args;
	}
	else
		return list<Expr*>();
}



list<Expr*> Parser::parseArgs()
{
	Expr* expr = parseExpr();
	list<Expr*> argsRest = parseArgsRest();
	argsRest.push_front( expr );
	return argsRest;
}



list<Expr*> Parser::parseArgsRest()
{
	if ( check( COMMA_T ) )
	{
		match( COMMA_T );
		list<Expr*> args = parseArgs();
		return args;
	}
	else
		return list<Expr*>();
}



list<Item*> Parser::parseItems()
{
	Item* item = parseItem();
	list<Item*> itemRest = parseItemsRest();
	itemRest.push_front( item );
	return itemRest;
}



list<Item*> Parser::parseItemsRest()
{
	if ( check( COMMA_T ) )
	{
		match( COMMA_T );
		list<Item*> items = parseItems();
		return items;
	}
	else
		return list<Item*>();
}

//
// !Baby parsers
//===----------------------------------------------------------------------===//



//===----------------------------------------------------------------------===//
// The main Parser
//===----------------------------------------------------------------------===//
Program* Parser::parse()
{
	Program* node = parseProgram();
	match( EOF_T );
	return node;
}