//
//  SymbolTable.cpp
//  Project4-Interpreter
//
//  Created by Truc Le on 11/1/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "SymbolTable.h"


//===----------------------------------------------------------------------===//
// Push a new entry of type pair<string, map<string, value> > to the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::enter( string ID )
{
	pair<string, map<string, Value> > symbol = make_pair( ID, map<string, Value>() );
	symbol_table.push( symbol );
}



//===----------------------------------------------------------------------===//
// Pop the top element of the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::exit()
{
	symbol_table.pop();
}



//===----------------------------------------------------------------------===//
// Create a new entry of type pair<string, Value>
// for the map of the top entry of the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::bind( string ID, Value v )
{
	pair<string, Value> value = make_pair( ID, v );
	get<1>(symbol_table.top()).insert( value );
}



//===----------------------------------------------------------------------===//
// Look up the passed key in the map of
// the top entry of the symbol table
//===----------------------------------------------------------------------===//
Value SymbolTable::lookUp( string ID )
{
	map<string, Value> map = get<1>(symbol_table.top());
	if ( map.find( ID ) != map.end() )
		return map.at( ID );
	return Value();
	
	//Value* ptr = &map.at( ID );
	//if ( IntValue* intValue = dynamic_cast<IntValue*>( ptr ) ) //downcast to check class type of map.at( ID )
	//	return intValue->int_value; //safe to call
	
}



//===----------------------------------------------------------------------===//
// Value::set
//===----------------------------------------------------------------------===//
void IntCell::set( int i )
{
	int_value = i;
}

void BoolCell::set( bool b )
{
	bool_value = b;
}