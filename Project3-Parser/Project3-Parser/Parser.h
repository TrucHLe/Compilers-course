//
//  Parser.h
//  Project2-Parser
//
//  Created by Truc Le on 9/22/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project3_Parser__Parser__
#define __Project3_Parser__Parser__

#include <stdio.h>  
#include <fstream>          //ifstream
#include <unordered_map>    //unordered_map, aka hashmap
#include "Constants.h"
#include "Scanner.h"
#include "AST.h"
//#include "AST.cpp"

class Parser
{
    
private:
    Scanner scanner;
    Token token;
    unordered_map<string, int> constants;
    
public:
    Parser( ifstream& i );
    
    ASTNode* parse(); //main
    Token advance();
    Token match( int tokenType );
    bool check( int tokenType );
	
	
    // Baby parsers
	Program* parseProgram();
	Block* parseBlock();
	ConstDecl* parseConstDecl();
	VarDecl* parseVarDecl();
	ProcDecl* parseProcDecl();
	Stmt* parseStmt();
	Stmt* parseStmtID( string i, int lin, int col ); //return terminal ID Stmt here
	Stmt* parseStmtIf( Expr* t, Stmt* tr, int lin, int col ); //return terminal If Stmt here
	Stmt* parseStmtPrompt( string m, int lin, int col ); //return terminal Prompt Stmt here
	Param* parseParam();
	Expr* parseExpr();
	Expr* parseExprRest( Expr* e, int lin, int col ); //return terminal Expr here
	Expr* parseSimpleExpr();
	Expr* parseSimpleExprRest( Expr* e, int lin, int col ); //return terminal Expr here
	Expr* parseTerm();
	Expr* parseTermRest( Expr* e, int lin, int col ); //return terminal Expr here
	Expr* parseFactor();
	Item* parseItem();
	
	Op2 parseRelOp();
	Op2 parseAddOp();
	Op2 parseMulOp();
	
	string parseSign();
	Type parseType();
	
	list<ConstDecl*> parseConstDecls();
	list<VarDecl*> parseVarDecls();
	list<ProcDecl*> parseProcDecls();
	list<Stmt*> parseStmts();
	list<Param*> parseParamList();
	list<Param*> parseParams();
	list<Param*> parseParamRest();
	list<Expr*> parseArgList();
	list<Expr*> parseArgs();
	list<Expr*> parseArgsRest();
	list<Item*> parseItems();
	list<Item*> parseItemsRest();
};


#endif /* defined(__Project3_Parser__Parser__) */
