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
#include <unordered_map>    //unordered_map, aka hashmap
#include "Constants.h"
#include "Scanner.h"
#include "AST.h"


class Parser
{
    
private:
    Scanner scanner;
    Token token;
    unordered_map<string, int> constants;

    
public:
    Parser( ifstream& i );
    
    void parse(); //main
    Token advance();
    Token match( int tokenType );
    bool check( int tokenType );
    
    // Baby parsers
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
	
    ASTNode* parseProgram();
	Block* parseBlock();
	ConstDecl* parseConstDecl();
	VarDecl* parseVarDecl();
	ProcDecl* parseProcDecl();
	Stmt* parseStmt();
	Stmt* parseStmtID( Token t ); //return terminal ID Stmt here
	Stmt* parseStmtIf( Expr* e, Stmt* tr ); //return terminal If Stmt here
	Stmt* parseStmtPrompt( string m ); //return terminal Prompt Stmt here
	Param* parseParam();
	Expr* parseExpr();
	Expr* parseSimpleExpr();
	Expr* parseExprRest( Expr* e ); //return terminal Expr here
	Expr* parseTerm();
	Expr* parseSimpleExprRest( Expr* e ); //return terminal Expr here
	Expr* parseFactor();
	Expr* parseTermRest( Expr* e ); //return terminal Expr here
	Item* parseItem();
	
	Op2 parseRelOp();
	Op2 parseAddOp();
	Op2 parseMulOp();
	
	string parseSign();
	Type parseType();
};


#endif /* defined(__Project2_Parser__Parser__) */
