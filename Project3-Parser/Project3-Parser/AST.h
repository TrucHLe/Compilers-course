//
//  AST.h -- Abstract Syntax Tree
//  Project3-Parser
//
//  Created by Truc Le on 10/10/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project3_Parser__AST__
#define __Project3_Parser__AST__

#include <stdio.h>
#include <string>
#include <stack>
#include <list>
using namespace std;



//===----------------------------------------------------------------------===//
// AST's struct, enum, and functions declarations
//===----------------------------------------------------------------------===//
struct ASTNode;
struct Program;
struct Block;
struct ConstDecl;
struct VarDecl;
struct ProcDecl;
struct Param;
struct ValParam;
struct VarParam;
struct Stmt;
struct Assign;
struct Call;
struct Sequence;
struct IfThen;
struct IfThenElse;
struct While;
struct Prompt;
struct Prompt2;
struct Print;
struct Item;
struct ExprItem;
struct StringItem;
struct Expr;
struct BinOp;
struct UnOp;
struct Num;
struct Id;
struct True;
struct False;
static bool deleteAllConstDecl( ConstDecl* c );
static bool deleteAllVarDecl( VarDecl* v );
static bool deleteAllProcDecl( ProcDecl* v );
static bool deleteAllParam( Param* p );
static bool deleteAllStmt( Stmt* s );
static bool deleteAllExpr( Expr* e );
static bool deleteAllItem( Item* i );

enum ASTNodeType
{
	Node_Undefined,
	Node_Program,
	Node_Block,
	Node_ConstDecl,
	Node_VarDecl,
	Node_ProcDecl,
	Node_ValParam,
	Node_VarParam,
	Node_Assign,
	Node_Call,
	Node_Sequence,
	Node_IfThen,
	Node_IfThenElse,
	Node_While,
	Node_Prompt,
	Node_Prompt2,
	Node_Print,
	Node_ExprItem,
	Node_StringItem,
	Node_BinOp,
	Node_UnOp,
	Node_Num,
	Node_Id,
	Node_True,
	Node_False
};


enum Type
{
	IntType,
	BoolType
};


enum Op1
{
	Neg,
	Not
};


enum Op2
{
	EQ,
	NE,
	LE,
	GE,
	LT,
	GT,
	Plus,
	Minus,
	Times,
	Div,
	Mod,
	And,
	Or
};

string nameOf( Type type );
string nameOf( Op1 op1 );
string nameOf( Op2 op2 );


#endif /* defined(__Project3_Parser__AST__) */
