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
#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <map>
using namespace std;


//===----------------------------------------------------------------------===//
// AST's struct, enum, and functions declarations for tree-parsing
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


enum DataType
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



//===----------------------------------------------------------------------===//
// SymbolTable, Value struct, and enum for tree-interpreting
//===----------------------------------------------------------------------===//
struct SymbolTable;
struct Value;
struct IntValue;
struct BoolValue;
struct IntCell;
struct BoolCell;
struct ProcValue;


enum ValueType
{
	Value_Undefined,
	Value_IntValue,
	Value_BoolValue,
	Value_IntCell,
	Value_BoolCell,
	Value_ProcValue
};


string nameOf( DataType dataType );
string nameOf( Op1 op1 );
string nameOf( Op2 op2 );
string nameOf( ValueType valueType );



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
	virtual void interpret() = 0;
	virtual void interpret( SymbolTable t ) = 0;
	virtual Value* interpretValue( SymbolTable t ) = 0;
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );};



//===----------------------------------------------------------------------===//
// Variable Declaration Node
//===----------------------------------------------------------------------===//
struct VarDecl : ASTNode
{
	string ID;
	DataType data_type;
	
	VarDecl( string i, DataType t, int lin, int col )
	{
		ID			= i;
		data_type	= t;
		line		= lin;
		column		= col;
		node_type	= Node_VarDecl;
	}
	
	~VarDecl() {}
	
	string toString( string indent );
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



//===----------------------------------------------------------------------===//
// Parameter Node
//===----------------------------------------------------------------------===//
struct Param : ASTNode {};

struct ValParam : Param
{
	string ID;
	DataType data_type;
	
	ValParam( string i, DataType t, int lin, int col )
	{
		ID			= i;
		data_type	= t;
		line		= lin;
		column		= col;
		node_type	= Node_ValParam;
	}
	
	~ValParam() {}
	
	string toString( string indent );
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



struct VarParam : Param
{
	string ID;
	DataType data_type;
	
	VarParam( string i, DataType t, int lin, int col )
	{
		ID			= i;
		data_type	= t;
		line		= lin;
		column		= col;
		node_type	= Node_VarParam;
	}
	
	~VarParam() {}
	
	string toString( string indent );
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



//===----------------------------------------------------------------------===//
// Statement Node
//===----------------------------------------------------------------------===//
struct Stmt : ASTNode {};

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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
	void call( list<Param*> params, Block* block, list<Value*> args, SymbolTable	t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



//===----------------------------------------------------------------------===//
// Item Node
//===----------------------------------------------------------------------===//
struct Item : ASTNode {};

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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



//===----------------------------------------------------------------------===//
// Expression Node
//===----------------------------------------------------------------------===//
struct Expr : ASTNode {};

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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
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
	void interpret();
	void interpret( SymbolTable t );
	Value* interpretValue( SymbolTable t );
};



//===----------------------------------------------------------------------===//
// Value astract base class and its 5 subclasses
//===----------------------------------------------------------------------===//
struct Value
{
	int line;	// for error
	int column;	// message
	ValueType value_type;
	
	Value()
	{
		line		= 1;
		column		= 1;
		value_type	= Value_Undefined;
	}
	
	Value( int lin, int col )
	{
		line		= lin;
		column		= col;
		value_type	= Value_Undefined;
	}
	
	virtual ~Value() {}
};


struct IntValue : Value
{
	int int_value;
	
	IntValue( int i, int line, int column ) : Value( line, column )
	{
		int_value	= i;
		value_type	= Value_IntValue;
	}
	
	~IntValue() {}
};


struct BoolValue : Value
{
	bool bool_value;
	
	BoolValue( bool b, int line, int column ) : Value( line, column )
	{
		bool_value	= b;
		value_type	= Value_BoolValue;
	}
	
	~BoolValue() {}
};


struct IntCell : Value
{
	int int_value;
	
	IntCell( int i, int line, int column ) : Value( line, column )
	{
		int_value	= i;
		value_type	= Value_IntCell;
	}
	
	~IntCell() {}
	void set( int i );
};


struct BoolCell : Value
{
	bool bool_value;
	
	BoolCell( bool b, int line, int column ) : Value( line, column )
	{
		bool_value	= b;
		value_type	= Value_BoolCell;
	}
	
	~BoolCell() {}
	void set( bool b );
};


struct ProcValue : Value
{
	list<Param*> params; //can point to but cannot own a Param because it's an abstract class
	Block* block;
	
	ProcValue( list<Param*> p, Block* b, int line, int column ) : Value( line, column )
	{
		params		= p;
		block		= b;
		value_type	= Value_ProcValue;
	}
	
	~ProcValue();
};



//===----------------------------------------------------------------------===//
// Symbol Table
//===----------------------------------------------------------------------===//
struct SymbolTable
{
	stack<pair<string, map<string, Value>* > > symbol_table;
	
	SymbolTable()
	{
		symbol_table = stack<pair<string, map<string, Value>* > >();
	}
	
	void enterTable( string ID, int line, int column );
	void exitTable();
	void bind( string ID, int line, int column, Value* v );
	Value* lookUp( string ID, int line, int column );
};


#endif /* defined(__Project4_Interpreter__AST__) */