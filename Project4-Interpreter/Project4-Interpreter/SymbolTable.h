//
//  SymbolTable.h
//  Project4-Interpreter
//
//  Created by Truc Le on 11/1/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#ifndef __Project4_Interpreter__SymbolTable__
#define __Project4_Interpreter__SymbolTable__

#include <stdio.h>
#include <map>
#include "AST.h"
using namespace std;

//===----------------------------------------------------------------------===//
// struct headers
//===----------------------------------------------------------------------===//
struct SymbolTable;
struct Value;
struct IntValue;
struct BoolValue;
struct IntCell;
struct BoolCell;
struct ProcValue;
struct SymbolTable;



//===----------------------------------------------------------------------===//
// Value astract base class
//===----------------------------------------------------------------------===//
struct Value
{
	Value() {}
	virtual ~Value() {}
};



//===----------------------------------------------------------------------===//
// 5 kinds of Values
//===----------------------------------------------------------------------===//
struct IntValue : Value
{
	int int_value;
	
	IntValue( int i )
	{
		int_value = i;
	}
	
	~IntValue() {}
};


struct BoolValue : Value
{
	bool bool_value;
	
	BoolValue( bool b )
	{
		bool_value = b;
	}
	
	~BoolValue() {}
};


struct IntCell : Value
{
	int int_value;
	
	IntCell( int i )
	{
		int_value = i;
	}
	
	~IntCell() {}
	void set( int i );
};


struct BoolCell : Value
{
	bool bool_value;
	
	BoolCell( bool b )
	{
		bool_value = b;
	}
	
	~BoolCell() {}
	void set( bool b );
};


struct ProcValue : Value
{
	Param* params; //can point to but cannot own a Param because it's an abstract class
	Block* block;
	
	ProcValue( Param* p, Block* b )
	{
		params	= p;
		block	= b;
	}
	
	~ProcValue()
	{
		delete params;
		delete block;
	}
};



//===----------------------------------------------------------------------===//
// Symbol Table
//===----------------------------------------------------------------------===//
struct SymbolTable
{
	stack<pair<string, map<string, Value> > > symbol_table;
	
	SymbolTable()
	{
		symbol_table = stack<pair<string, map<string, Value> > >();
	}
	
	void enter( string ID );
	void exit();
	void bind( string ID, Value v );
	Value lookUp( string ID );
};

#endif /* defined(__Project4_Interpreter__SymbolTable__) */
