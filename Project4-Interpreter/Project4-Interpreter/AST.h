//
//  AST.h -- Abstract Syntax Tree
//	Project4-Interpreter
//
//  Created by Truc Le on 10/31/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project4_Interpreter__AST__
#define __Project4_Interpreter__AST__

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




//===----------------------------------------------------------------------===//
// AST abstract base class
//===----------------------------------------------------------------------===//
struct ASTNode
{
	int column;
	int line;
	ASTNodeType node_type;
	
	ASTNode()
	{
		column		= 1;
		line		= 1;
		node_type	= Node_Undefined;
	}
	
	virtual ~ASTNode() {}
	
	virtual string toString( string indent ) = 0;
};



//===----------------------------------------------------------------------===//
// Program Node
//===----------------------------------------------------------------------===//
struct Program : ASTNode
{
	string name;
	Block* block;
	
	Program( string n, Block* b, int lin, int col )
	{
		name		= n;
		block		= b;
		line		= lin;
		column		= col;
		node_type	= Node_Program;
	}
	
	~Program();
	
	string toString( string indent );
	
};



//===----------------------------------------------------------------------===//
// Block Node
//===----------------------------------------------------------------------===//
struct Block : ASTNode
{
	list<ConstDecl*> consts;
	list<VarDecl*> vars;
	list<ProcDecl*> procs;
	list<Stmt*> body;
	
	Block( list<ConstDecl*> c, list<VarDecl*> v, list<ProcDecl*> p, list<Stmt*> b, int lin, int col )
	{
		consts		= c;
		vars		= v;
		procs		= p;
		body		= b;
		line		= lin;
		column		= col;
		node_type	= Node_Block;
	}
	
	~Block();
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Constant Declaration Node
//===----------------------------------------------------------------------===//
struct ConstDecl : ASTNode
{
	string ID;
	int value;
	
	ConstDecl( string i, int v, int lin, int col )
	{
		ID			= i;
		value		= v;
		line		= lin;
		column		= col;
		node_type	= Node_ConstDecl;
	}
	
	~ConstDecl() {}
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Variable Declaration Node
//===----------------------------------------------------------------------===//
struct VarDecl : ASTNode
{
	string ID;
	Type type;
	
	VarDecl( string i, Type t, int lin, int col )
	{
		ID			= i;
		type		= t;
		line		= lin;
		column		= col;
		node_type	= Node_VarDecl;
	}
	
	~VarDecl() {}
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Procedure Declaration Node
//===----------------------------------------------------------------------===//
struct ProcDecl : ASTNode
{
	string ID;
	list<Param*> params;
	Block* block;
	
	ProcDecl( string i, list<Param*> p, Block* b, int lin, int col )
	{
		ID			= i;
		params		= p;
		block		= b;
		line		= lin;
		column		= col;
		node_type	= Node_ProcDecl;
	}
	
	~ProcDecl();
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Parameter Node
//===----------------------------------------------------------------------===//
struct Param : ASTNode
{
	Param()	{}
	virtual ~Param() {}
};



struct ValParam : Param
{
	string ID;
	Type type;
	
	ValParam( string i, Type t, int lin, int col )
	{
		ID			= i;
		type		= t;
		line		= lin;
		column		= col;
		node_type	= Node_ValParam;
	}
	
	~ValParam() {}
	
	string toString( string indent );
};



struct VarParam : Param
{
	string ID;
	Type type;
	
	VarParam( string i, Type t, int lin, int col )
	{
		ID			= i;
		type		= t;
		line		= lin;
		column		= col;
		node_type	= Node_VarParam;
	}
	
	~VarParam() {}
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Statement Node
//===----------------------------------------------------------------------===//
struct Stmt : ASTNode
{
	Stmt() {}
	virtual ~Stmt() {}
};



struct Assign : Stmt
{
	string ID;
	Expr* expr;
	
	Assign( string i, Expr* e, int lin, int col )
	{
		ID			= i;
		expr		= e;
		line		= lin;
		column		= col;
		node_type	= Node_Assign;
	}
	
	~Assign();
	
	string toString( string indent );
};



struct Call : Stmt
{
	string ID;
	list<Expr*> args;
	
	Call( string i, list<Expr*> a, int lin, int col )
	{
		ID			= i;
		args		= a;
		line		= lin;
		column		= col;
		node_type	= Node_Call;
	}
	
	~Call();
	
	string toString( string indent );
};



struct Sequence : Stmt
{
	list<Stmt*> body;
	
	Sequence( list<Stmt*> b, int lin, int col )
	{
		body		= b;
		line		= lin;
		column		= col;
		node_type	= Node_Sequence;
	}
	
	~Sequence();
	
	string toString( string indent );
};



struct IfThen : Stmt
{
	Expr* test;
	Stmt* trueClause;
	
	IfThen( Expr* t, Stmt* tr, int lin, int col )
	{
		test		= t;
		trueClause	= tr;
		line		= lin;
		column		= col;
		node_type	= Node_IfThen;
	}
	
	~IfThen();
	
	string toString( string indent );
};



struct IfThenElse : Stmt
{
	Expr* test;
	Stmt* trueClause;
	Stmt* falseClause;
	
	IfThenElse( Expr* t, Stmt* tr, Stmt* fa, int lin, int col )
	{
		test		= t;
		trueClause	= tr;
		falseClause	= fa;
		line		= lin;
		column		= col;
		node_type	= Node_IfThenElse;
	}
	
	~IfThenElse();
	
	string toString( string indent );
};



struct While : Stmt
{
	Expr* test;
	Stmt* body;
	
	While( Expr* t, Stmt* b, int lin, int col )
	{
		test		= t;
		body		= b;
		line		= lin;
		column		= col;
		node_type	= Node_While;
	}
	
	~While();
	
	string toString( string indent );
};



struct Prompt : Stmt
{
	string message;
	
	Prompt( string m, int lin, int col )
	{
		message		= m;
		line		= lin;
		column		= col;
		node_type	= Node_Prompt;
	}
	
	~Prompt() {}
	
	string toString( string indent );
};



struct Prompt2 : Stmt
{
	string message;
	string ID;
	
	Prompt2( string m, string i, int lin, int col )
	{
		message			= m;
		ID				= i;
		line			= lin;
		column			= col;
		node_type		= Node_Prompt2;
	}
	
	~Prompt2() {}
	
	string toString( string indent );
};



struct Print : Stmt
{
	list<Item*> items;
	
	Print( list<Item*> i, int lin, int col )
	{
		items			= i;
		line			= lin;
		column			= col;
		node_type		= Node_Print;
	}
	
	~Print();
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Item Node
//===----------------------------------------------------------------------===//
struct Item : ASTNode
{
	Item() {}
	virtual ~Item() {}
};



struct ExprItem : Item
{
	Expr* expr;
	
	ExprItem( Expr* e, int lin, int col )
	{
		expr		= e;
		line		= lin;
		column		= col;
		node_type	= Node_ExprItem;
	}
	
	~ExprItem();
	
	string toString( string indent );
};



struct StringItem : Item
{
	string message;
	
	StringItem( string m, int lin, int col )
	{
		message		= m;
		line		= lin;
		column		= col;
		node_type	= Node_StringItem;
	}
	
	~StringItem() {}
	
	string toString( string indent );
};



//===----------------------------------------------------------------------===//
// Expression Node
//===----------------------------------------------------------------------===//
struct Expr : ASTNode
{
	Expr() {}
	virtual ~Expr() {}
};



struct BinOp : Expr
{
	Op2 op;
	Expr* left;
	Expr* right;
	
	BinOp( Expr* l, Op2 o, Expr* r, int lin, int col )
	{
		op			= o;
		left		= l;
		right		= r;
		line		= lin;
		column		= col;
		node_type	= Node_BinOp;
	}
	
	~BinOp();
	
	string toString( string indent );
};



struct UnOp : Expr
{
	Op1 op;
	Expr* expr;
	
	UnOp( Op1 o, Expr* e, int lin, int col )
	{
		op			= o;
		expr		= e;
		line		= lin;
		column		= col;
		node_type	= Node_UnOp;
	}
	
	~UnOp();
	
	string toString( string indent );
};



struct Num : Expr
{
	int value;
	
	Num( int v, int lin, int col )
	{
		value		= v;
		line		= lin;
		column		= col;
		node_type	= Node_Num;
	}
	
	~Num() {}
	
	string toString( string indent );
};



struct Id : Expr
{
	string ID;
	
	Id( string i, int lin, int col )
	{
		ID			= i;
		line		= lin;
		column		= col;
		node_type	= Node_Id;
	}
	
	~Id() {}
	
	string toString( string indent );
};



struct True : Expr
{
	bool boolean;
	
	True( int lin, int col )
	{
		boolean		= true;
		line		= lin;
		column		= col;
	}
	
	~True() {}
	
	string toString( string indent );
};



struct False : Expr
{
	bool boolean;
	
	False( int lin, int col )
	{
		boolean		= false;
		line		= lin;
		column		= col;
	}
	
	~False() {}
	
	string toString( string indent );
};



#endif /* defined(__Project4_Interpreter__AST__) */
