//
//  AST.h -- Abstract Syntax Tree
//  Project5-Generator
//
//  Created by Truc Le on 10/31/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project5_Generator__AST__
#define __Project5_Generator__AST__

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <iterator>
#include <type_traits>	//is_same
using namespace std;


//===----------------------------------------------------------------------===//
// AST's struct, enum, and functions declarations [AST]
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
// Tree-Walking Functions [F]
//===----------------------------------------------------------------------===//
template <typename T>
struct SymbolTable;

//===-------------------------------===//
// Interpretation [I]
//===-------------------------------===//
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


//===-------------------------------===//
// Typechecking [T]
//===-------------------------------===//
struct Val;
struct IntVal;
struct BoolVal;
struct IntVar;
struct BoolVar;
struct ProcVal;

enum ValType
{
	Val_Undefined,
	Val_IntVal,
	Val_BoolVal,
	Val_IntVar,
	Val_BoolVar,
	Val_ProcVal
};


//===-------------------------------===//
// YASM Code Generation [G]
//===-------------------------------===//
struct Info;
struct ConstInfo;
struct VarInfo;
struct RefInfo;
struct ProcInfo;

enum InfoType
{
	Info_Undefined,
	Info_ConstInfo,
	Info_VarInfo,
	Info_RefInfo,
	Info_ProcInfo
};

void lvalue( string ID, SymbolTable<Info>* t );
void printChar( string str );

//===-------------------------------===//
// Printing [P]
//===-------------------------------===//
string nameOf( DataType dataType );
string nameOf( Op1 op1 );
string nameOf( Op2 op2 );
string nameOf( ValueType valueType );
string nameOf( ValType valType );


//===----------------------------------------------------------------------===//
// [AST] Abstract base class
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
// [AST] Program Node
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
	Value* interpret();
	Val* typecheck();
	Info* generate();
};



//===----------------------------------------------------------------------===//
// [AST] Block Node
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Constant Declaration Node
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Variable Declaration Node
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Procedure Declaration Node
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Parameter Node
//===----------------------------------------------------------------------===//
struct Param : ASTNode
{
	string ID;
	DataType data_type;
	virtual Info* generate( SymbolTable<Info>* t ) = 0;
};

struct ValParam : Param
{
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
	Info* generate( SymbolTable<Info>* t );
};



struct VarParam : Param
{
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
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Statement Node
//===----------------------------------------------------------------------===//
struct Stmt : ASTNode
{
	virtual Value* interpret( SymbolTable<Value>* t ) = 0;
	virtual Val* typecheck( SymbolTable<Val>* t ) = 0;
	virtual Info* generate( SymbolTable<Info>* t ) = 0;
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	void call( list<Param*> params, Block* block, list<Value*> args, SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	void match( list<Param*> params, list<Val*> args );
	Info* generate( SymbolTable<Info>* t );
	void setup( list<Param*> params, list<Expr*> args, SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
};



//===----------------------------------------------------------------------===//
// [AST] Item Node
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
// [AST] Expression Node
//===----------------------------------------------------------------------===//
struct Expr : ASTNode
{
	virtual Value* interpret( SymbolTable<Value>* t ) = 0;
	virtual Val* typecheck( SymbolTable<Val>* t ) = 0;
	virtual Info* generate( SymbolTable<Info>* t ) = 0;
	virtual Info* generate( SymbolTable<Info>* t, string y, string n ) = 0;
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
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
	Value* interpret( SymbolTable<Value>* t );
	Val* typecheck( SymbolTable<Val>* t );
	Info* generate( SymbolTable<Info>* t );
	Info* generate( SymbolTable<Info>* t, string y, string n );
};



//===----------------------------------------------------------------------===//
// [I] Value astract base class & its classes
//===----------------------------------------------------------------------===//
struct Value
{
	int line;	// for error
	int column;	// messages
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
	virtual int getIntValue( int lin, int col ) = 0;
	virtual bool getBoolValue( int lin, int col ) = 0;
	virtual void setValue( int i, int lin, int col ) = 0;
	virtual void setValue( bool b, int lin, int col ) = 0;
};


struct IntValue : Value
{
	int integer;
	
	IntValue( int i, int lin, int col )
	{
		integer		= i;
		line		= lin;
		column		= col;
		value_type	= Value_IntValue;
	}
	
	~IntValue() {}
	int getIntValue( int lin, int col );
	bool getBoolValue( int lin, int col );
	void setValue( int i, int lin, int col );
	void setValue( bool b, int lin, int col );
};


struct BoolValue : Value
{
	bool boolean;
	
	BoolValue( bool b, int lin, int col )
	{
		boolean		= b;
		line		= lin;
		column		= col;
		value_type	= Value_BoolValue;
	}
	
	~BoolValue() {}
	int getIntValue( int lin, int col );
	bool getBoolValue( int lin, int col );
	void setValue( int i, int lin, int col );
	void setValue( bool b, int lin, int col );
};


struct IntCell : Value
{
	int integer;
	
	IntCell( int i, int lin, int col )
	{
		integer		= i;
		line		= lin;
		column		= col;
		value_type	= Value_IntCell;
	}
	
	~IntCell() {}
	int getIntValue( int lin, int col );
	bool getBoolValue( int lin, int col );
	void setValue( int i, int lin, int col );
	void setValue( bool b, int lin, int col );
};


struct BoolCell : Value
{
	bool boolean;
	
	BoolCell( bool b, int lin, int col )
	{
		boolean		= b;
		line		= lin;
		column		= col;
		value_type	= Value_BoolCell;
	}
	
	~BoolCell() {}
	int getIntValue( int lin, int col );
	bool getBoolValue( int lin, int col );
	void setValue( int i, int lin, int col );
	void setValue( bool b, int lin, int col );
};


struct ProcValue : Value
{
	list<Param*> params;
	Block* block;
	
	ProcValue( list<Param*> p, Block* b, int lin, int col )
	{
		params		= p;
		block		= b;
		line		= lin;
		column		= col;
		value_type	= Value_ProcValue;
	}
	
	~ProcValue()
	{
		for ( Param* p : params )
			delete p;
		delete block;
	}
	int getIntValue( int lin, int col );
	bool getBoolValue( int lin, int col );
	void setValue( int i, int lin, int col );
	void setValue( bool b, int lin, int col );
};



//===----------------------------------------------------------------------===//
// [T] Val astract base class & its classes
//===----------------------------------------------------------------------===//
struct Val
{
	int line;
	int column;
	ValType val_type;
	
	Val()
	{
		line		= 1;
		column		= 1;
		val_type	= Val_Undefined;
	}
	Val( int lin, int col )
	{
		line		= lin;
		column		= col;
		val_type	= Val_Undefined;
	}
	
	virtual ~Val() {}
	virtual bool isVar() = 0;
};


struct IntVal : Val
{
	IntVal( int lin, int col )
	{
		line		= lin;
		column		= col;
		val_type	= Val_IntVal;
	}
	
	~IntVal() {}
	bool isVar();
};


struct BoolVal : Val
{
	BoolVal( int lin, int col )
	{
		line		= lin;
		column		= col;
		val_type	= Val_BoolVal;
	}
	
	~BoolVal() {}
	bool isVar();
};


struct IntVar : Val
{
	IntVar( int lin, int col )
	{
		line		= lin;
		column		= col;
		val_type	= Val_IntVar;
	}
	
	~IntVar() {}
	bool isVar();
};


struct BoolVar : Val
{
	BoolVar( int lin, int col )
	{
		line		= lin;
		column		= col;
		val_type	= Val_BoolVar;
	}
	
	~BoolVar() {}
	bool isVar();
};


struct ProcVal : Val
{
	list<Param*> params;

	ProcVal( list<Param*> p, int lin, int col )
	{
		params		= p;
		line		= lin;
		column		= col;
		val_type	= Val_ProcVal;
	}
	
	~ProcVal()
	{
		for ( Param* p : params )
			delete p;
	}
	bool isVar();
};



//===----------------------------------------------------------------------===//
// [G] Info astract base class & its classes
//===----------------------------------------------------------------------===//
struct Info
{
	InfoType info_type;
	
	Info()
	{
		info_type	= Info_Undefined;
	}
	
	virtual ~Info() {}
};


struct ConstInfo : Info
{
	int constant;
	
	ConstInfo( int cnst )
	{
		constant	= cnst;
		info_type	= Info_ConstInfo;
	}
	
	~ConstInfo() {}
};


struct VarInfo : Info
{
	int level;
	int offset;
	
	VarInfo( int l, int o )
	{
		level		= l;
		offset		= o;
		info_type	= Info_VarInfo;
	}
	
	~VarInfo() {}
};


struct RefInfo : Info
{
	int level;
	int offset;
	
	RefInfo( int l, int o )
	{
		level		= l;
		offset		= o;
		info_type	= Info_RefInfo;
	}
	
	~RefInfo() {}
};


struct ProcInfo : Info
{
	string label;
	list<Param*> params;
	
	ProcInfo( string l, list<Param*> p )
	{
		label		= l;
		params		= p;
		info_type	= Info_ProcInfo;
	}
	
	~ProcInfo()
	{
		for ( Param* p : params )
			delete p;
	}
};



//===----------------------------------------------------------------------===//
// [F] Symbol Table Template
//===----------------------------------------------------------------------===//
template <typename T>
struct SymbolTable
{
	vector<pair<string, map<string, T*>* > > symbol_table;
	int level;
	int offset;
	int param;
	int sequence;
	
	SymbolTable()
	{
		symbol_table	= vector<pair<string, map<string, T*>* > >();
		level			= 0;
		offset			= 0;
		param			= 0;
		sequence		= 0;
	}
	
	// Functions without line and column are for Code Generation, in which
	// printing error messages are already handled by Typechecking
	//
	T* lookUp( string ID, int line, int column );
	T* lookUp( string ID );
	void enterTable( string ID, int line, int column );
	void enterTable( string ID );
	void bind( string ID, int line, int column, T* v );
	void bind( string ID, T* v );
	void exitTable();
	int getLevel();
	string newLabel();
};


#endif /* defined(__Project5_Generator__AST__) */