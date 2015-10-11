//
//  AST.cpp -- Abstract Syntax Tree
//  Project3-Parser
//
//  Created by Truc Le on 10/10/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "AST.h"



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
	
	~Program()
	{
		delete block;
	}
	
	string toString( string indent )
	{
		return indent + "Program " + name + "\n";
	}
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
	
	~Block()
	{
		consts.remove_if( deleteAllConstDecl );
		vars.remove_if( deleteAllVarDecl );
		procs.remove_if( deleteAllProcDecl );
		body.remove_if( deleteAllStmt );
	}
	
	string toString( string indent )
	{
		return indent + "Block\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Const " + ID + " = " + to_string( value ) + "\n";
	}
	
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
	
	string toString( string indent )
	{
		return "Var " + ID + " : " + nameOf( type ) + "\n";
	}
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
	
	~ProcDecl()
	{
		params.remove_if( deleteAllParam );
		delete block;
	}
	
	string toString( string indent )
	{
		return indent + "Proc " + ID + "\n";
	}
};



//===----------------------------------------------------------------------===//
// Parameter Node
//===----------------------------------------------------------------------===//
struct Param : ASTNode
{
	Param()	{}
	~Param() {}
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
	
	string toString( string indent )
	{
		return indent + "Val " + ID + " : " + nameOf( type ) + "\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Var " + ID + " : " + nameOf( type ) + "\n";
	}
};



//===----------------------------------------------------------------------===//
// Statement Node
//===----------------------------------------------------------------------===//
struct Stmt : ASTNode
{
	Stmt() {}
	~Stmt() {}
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
	
	~Assign()
	{
		delete expr;
	}
	
	string toString( string indent )
	{
		return "Assign " + ID + "\n";
	}
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
	
	~Call()
	{
		args.remove_if( deleteAllExpr );
	}
	
	string toString( string indent )
	{
		return indent + "Call " + ID + "\n";
	}
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
	
	~Sequence()
	{
		body.remove_if( deleteAllStmt );
	}
	
	string toString( string indent )
	{
		return indent + "Sequence\n";
	}
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
	
	~IfThen()
	{
		delete test;
		delete trueClause;
	}
	
	string toString( string indent )
	{
		return indent + "IfThen\n";
	}
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
	
	~IfThenElse()
	{
		delete test;
		delete trueClause;
		delete falseClause;
	}
	
	string toString( string indent )
	{
		return indent + "IfThenElse\n";
	}
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
	
	~While()
	{
		delete test;
		delete body;
	}
	
	string toString( string indent )
	{
		return indent + "While\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Prompt " + message + "\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Prompt2 " + message + ", " + ID + "\n";
	}
	
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
	
	~Print()
	{
		items.remove_if( deleteAllItem );
	}
	
	string toString( string indent )
	{
		return indent + "Print\n";
	}
};



//===----------------------------------------------------------------------===//
// Item Node
//===----------------------------------------------------------------------===//
struct Item : ASTNode
{
	Item() {}
	~Item() {}
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
	
	~ExprItem()
	{
		delete expr;
	}
	
	string toString( string indent )
	{
		return indent + "ExprItem\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "StringItem " + message + "\n";
	}
};



//===----------------------------------------------------------------------===//
// Expression Node
//===----------------------------------------------------------------------===//
struct Expr : ASTNode
{
	Expr() {}
	~Expr() {}
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
	
	~BinOp()
	{
		delete left;
		delete right;
	}
	
	string toString( string indent )
	{
		return indent + "BinOp " + nameOf( op )+ "\n";
	}
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
	
	~UnOp()
	{
		delete expr;
	}
	
	string toString( string indent )
	{
		return indent + "UnOp " + nameOf( op ) + "\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Num " + to_string( value ) + "\n";
	}
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
	
	string toString( string indent )
	{
		return indent + "Id " + ID + "\n";
	}
};



struct True : Expr
{
	bool boolean;
	
	True()
	{
		boolean = true;
	}
	
	~True() {}
	
	string toString( string indent )
	{
		return indent + "True\n";
	}
};



struct False : Expr
{
	bool boolean;
	
	False()
	{
		boolean = false;
	}
	
	~False() {}
	
	string toString( string indent )
	{
		return indent + "False\n";
	}
};



//===----------------------------------------------------------------------===//
// Delete all pointers from list of pointers
//===----------------------------------------------------------------------===//
static bool deleteAllConstDecl( ConstDecl* c )
{
	delete c;
	return true;
};

static bool deleteAllVarDecl( VarDecl* v )
{
	delete v;
	return true;
}

static bool deleteAllProcDecl( ProcDecl* v )
{
	delete v;
	return true;
}

static bool deleteAllStmt( Stmt* s )
{
	delete s;
	return true;
}

static bool deleteAllParam( Param* p )
{
	delete p;
	return true;
}

static bool deleteAllExpr( Expr* e )
{
	delete e;
	return true;
}

static bool deleteAllItem( Item* i )
{
	delete i;
	return true;
}



//===----------------------------------------------------------------------===//
// Returns name of Type, Op1, Op2
//===----------------------------------------------------------------------===//
string nameOf( Type type )
{
	switch ( type )
	{
	  case IntType:		return "Int";
	  case BoolType:	return "Bool";
	}
}

string nameOf( Op1 op1 )
{
	switch ( op1 )
	{
		case Neg:	return "Neg";
		case Not:	return "Not";
	}
}

string nameOf( Op2 op2 )
{
	switch ( op2 )
	{
		case EQ:	return "EQ";
		case NE:	return "NE";
		case LE:	return "LE";
		case GE:	return "GE";
		case LT:	return "LT";
		case GT:	return "GT";
		case Plus:	return "Plus";
		case Minus:	return "Minus";
		case Times:	return "Times";
		case Div:	return "Div";
		case Mod:	return "Mod";
		case And:	return "And";
		case Or:	return "Or";
	}
}
