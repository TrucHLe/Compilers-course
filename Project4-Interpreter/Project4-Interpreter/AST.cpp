//
//  AST.cpp -- Abstract Syntax Tree
//	Project4-Interpreter
//
//  Created by Truc Le on 10/31/15.
//  Copyright (c) 2015 Truc Le. All rights reserved.
//

#include "AST.h"


//===----------------------------------------------------------------------===//
// Destructors
//===----------------------------------------------------------------------===//
Program::~Program()
{
	delete block;
}

Block::~Block()
{
	consts.remove_if( deleteAllConstDecl );
	vars.remove_if( deleteAllVarDecl );
	procs.remove_if( deleteAllProcDecl );
	body.remove_if( deleteAllStmt );
}

ProcDecl::~ProcDecl()
{
	params.remove_if( deleteAllParam );
	delete block;
}

Assign::~Assign()
{
	delete expr;
}

Call::~Call()
{
	args.remove_if( deleteAllExpr );
}

Sequence::~Sequence()
{
	body.remove_if( deleteAllStmt );
}

IfThen::~IfThen()
{
	delete test;
	delete trueClause;
}

IfThenElse::~IfThenElse()
{
	delete test;
	delete trueClause;
	delete falseClause;
}

While::~While()
{
	delete test;
	delete body;
}

Print::~Print()
{
	items.remove_if( deleteAllItem );
}

ExprItem::~ExprItem()
{
	delete expr;
}

BinOp::~BinOp()
{
	delete left;
	delete right;
}

UnOp::~UnOp()
{
	delete expr;
}

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
// Print nodes of the abstract syntax tree
//===----------------------------------------------------------------------===//
string Program::toString( string indent )
{
	string result = indent + "Program " + name + "\n";
	result += block->toString( indent + "  " );
	return result;
}


string Block::toString( string indent )
{
	string result = indent + "Block\n";
	for ( ConstDecl* c : consts )
		result += c->toString( indent + "  " );
	for ( VarDecl* v : vars )
		result += v->toString( indent + "  " );
	for ( ProcDecl* p : procs )
		result += p->toString( indent + "  " );
	for ( Stmt* b : body )
		result += b->toString( indent + "  " );
	return result;
}


string ConstDecl::toString( string indent )
{
	string result = indent + "Const " + ID + " = " + to_string( value ) + "\n";
	return result;
}


string VarDecl::toString( string indent )
{
	string result = indent + "Var " + ID + " : " + nameOf( data_type ) + "\n";
	return result;
}


string ProcDecl::toString( string indent )
{
	string result = indent + "Proc " + ID + "\n";
	for ( Param* p : params )
		result += p->toString( indent + "  " );
	result += block->toString( indent + "  " );
	return result;
}


string ValParam::toString( string indent )
{
	string result = indent + "Val " + ID + " : " + nameOf( data_type ) + "\n";
	return result;
}


string VarParam::toString( string indent )
{
	string result = indent + "Var " + ID + " : " + nameOf( data_type ) + "\n";
	return result;
}


string Assign::toString( string indent )
{
	string result = indent + "Assign " + ID + "\n";
	result += expr->toString( indent + "  " );
	return result;
}


string Call::toString( string indent )
{
	string result = indent + "Call " + ID + "\n";
	for ( Expr* a : args )
		result += a->toString( indent + "  " );
	return result;
}


string Sequence::toString( string indent )
{
	string result = indent + "Sequence\n";
	for ( Stmt* b : body )
		result += b->toString( indent + "  " );
	return result;
}


string IfThen::toString( string indent )
{
	string result = indent + "IfThen\n";
	result += test->toString( indent + "  " );
	result += trueClause->toString( indent + "  " );
	return result;
}


string IfThenElse::toString( string indent )
{
	string result = indent + "IfThenElse\n";
	result += test->toString( indent + "  " );
	result += trueClause->toString( indent + "  " );
	result += falseClause->toString( indent + "  " );
	return result;
}


string While::toString( string indent )
{
	string result = indent + "While\n";
	result += test->toString( indent + "  " );
	result += body->toString( indent + "  " );
	return result;
}


string Prompt::toString( string indent )
{
	string result = indent + "Prompt " + message + "\n";
	return result;
}


string Prompt2::toString( string indent )
{
	string result = indent + "Prompt2 " + message + ", " + ID + "\n";
	return result;
}


string Print::toString( string indent )
{
	string result = indent + "Print\n";
	for ( Item* i : items )
		result += i->toString( indent + "  " );
	return result;
}


string ExprItem::toString( string indent )
{
	string result = indent + "ExprItem\n";
	result += expr->toString( indent + "  " );
	return result;
}


string StringItem::toString( string indent )
{
	string result = indent + "StringItem " + message + "\n";
	return result;
}


string BinOp::toString( string indent )
{
	string result = indent + "BinOp " + nameOf( op ) + "\n";
	result += left->toString( indent + "  " );
	result += right->toString( indent + "  " );
	return result;
}


string UnOp::toString( string indent )
{
	string result = indent + "UnOp " + nameOf( op ) + "\n";
	result += expr->toString( indent + "  " );
	return result;
}


string Num::toString( string indent )
{
	string result = indent + "Num " + to_string( value ) + "\n";
	return result;
}


string Id::toString( string indent )
{
	string result = indent + "Id " + ID + "\n";
	return result;
}


string True::toString( string indent )
{
	string result = indent + "True\n";
	return result;
}


string False::toString( string indent )
{
	string result = indent + "False\n";
	return result;
}



//===----------------------------------------------------------------------===//
// Interpret nodes and call functions
// Only delete pointers to Program, Block, ProcDecl, Assign, Call,
// Sequence, IfThen, IfThenElse, While, Print, ExprItem, BinOp, UnOp
//===----------------------------------------------------------------------===//
Value* Program::interpret()
{
	SymbolTable t = SymbolTable();
	t.enterTable( name, line, column );
	block->interpret( t );
	t.exitTable();
	return NULL;
}


Value* Block::interpret( SymbolTable t )
{
	for ( ConstDecl* c : consts )
		c->interpret( t );
	for ( VarDecl* v : vars )
		v->interpret( t );
	for ( ProcDecl* p : procs )
		p->interpret( t );
	for ( Stmt* b : body )
		b->interpret( t );
	return NULL;
}


Value* ConstDecl::interpret( SymbolTable t )
{
	t.bind( ID, line, column, new IntValue( value, line, column ) );
	return NULL;
}


Value* VarDecl::interpret( SymbolTable t )
{
	if ( data_type == IntType )
		t.bind( ID, line, column, new IntCell( 0, line, column ) );
	else
		t.bind( ID, line, column, new BoolCell( false, line, column ) );
	return NULL;
}


Value* ProcDecl::interpret( SymbolTable t )
{
	t.bind( ID, line, column, new ProcValue( params, block, line, column ) );
	return NULL;
}


Value* Assign::interpret( SymbolTable t )
{
	Value* lhs = t.lookUp( ID, line, column );
	
	if ( lhs->value_type == Value_IntCell || lhs->value_type == Value_BoolCell )
	{
		Value* rhs = expr->interpret( t );
		
		if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntValue )
		{
			IntCell* cell = dynamic_cast<IntCell*>( lhs );
			IntValue* value = dynamic_cast<IntValue*>( rhs );
			cell->set( value->integer );
		}
		else if ( lhs->value_type == Value_BoolCell && rhs->value_type == Value_BoolValue )
		{
			BoolCell* cell = dynamic_cast<BoolCell*>( lhs );
			BoolValue* value = dynamic_cast<BoolValue*>( rhs );
			cell->set( value->boolean );
		}
		else
		{
			cout << "(!) Cannot assign " << nameOf( rhs->value_type )
				 << " at " << rhs->line << ":" << rhs->column
				 << " to " << ID << " at " << line << ":" << column
				 << ", which is " << nameOf( lhs->value_type ) << endl;
			exit( 1 );
		}
	}
	else
	{
		cout << "(!) Cannot assign to " << ID
			 << " at " << line << ":" << column
			 << ", which is " << nameOf( lhs->value_type ) << endl;
		exit( 1 );
	}
	return NULL;
}


Value* Call::interpret( SymbolTable t )
{
	Value* look_up = t.lookUp( ID, line, column );
	if ( look_up->value_type != Value_ProcValue )
	{
		cout << "(!) Expected " << nameOf( Value_ProcValue )
			 << " at " << line << ":" << column
			 << " but found " << nameOf( look_up->value_type ) << endl;
		exit( 1 );
	}
	
	
	ProcValue* value = dynamic_cast<ProcValue*>( t.lookUp( ID, line, column ) );
	list<Value*> arguments;
	
	for ( Expr* arg : args )
	{
		Value* v = arg->interpret( t );
		arguments.push_back( v );
	}
	
	
	if ( value->params.size() != arguments.size() )
	{
		cout << "(!) The number of parameters does not match the number of arguments of " << ID
			 << " at " << line << ":" << column << endl;
		exit( 1 );
	}
	
	t.enterTable( ID, line, column );
	call( value->params, value->block, arguments, t );
	t.exitTable();
	return NULL;
}


void Call::call( list<Param*> params, Block* block, list<Value*> args, SymbolTable t )
{
	if ( params.empty() && args.empty() )
		block->interpret( t );
	else
	{
		Param* par = params.front();
		Value* arg = args.front();
		params.pop_front();
		args.pop_front();
		
		if ( par->node_type == Node_ValParam )
		{
			ValParam* param = dynamic_cast<ValParam*>( par );
			
			if ( param->data_type == IntType &&
				( arg->value_type == Value_IntValue || arg->value_type == Value_IntCell ) )
			{
				int value;
				
				if ( arg->value_type == Value_IntValue )
					value = dynamic_cast<IntValue*>( arg )->integer;
				else
					value = dynamic_cast<IntCell*>( arg )->integer;
				
				t.bind( param->ID, param->line, param->column,
					    new IntCell( value, param->line, param->column ) );
			}
			else if ( param->data_type == BoolType &&
					  ( arg->value_type == Value_BoolValue || arg->value_type == Value_BoolCell ) )
			{
				bool value;
				
				if ( arg->value_type == Value_BoolValue )
					value = dynamic_cast<BoolValue*>( arg )->boolean;
				else
					value = dynamic_cast<BoolCell*>( arg )->boolean;
				
				t.bind( param->ID, param->line, param->column,
					    new BoolCell( value, param->line, param->column ) );
			}
			else
			{
				cout << "(!) Cannot pass " << nameOf( arg->value_type )
					 << " " << ID << " declared at " << arg->line << ":" << arg->column
					 << " to a parameter of type " << nameOf( param->data_type )
					 << " at " << param->line << ":" << param->column << endl;
				exit( 1 );
			}
		}
		else
		{
			VarParam* param = dynamic_cast<VarParam*>( par );
			
			if ( param->data_type == IntType && arg->value_type == Value_IntCell )
				t.bind( param->ID, param->line, param->column, dynamic_cast<IntCell*>( arg ) );
			else if ( param->data_type == BoolType && arg->value_type == Value_BoolCell )
				t.bind( param->ID, param->line, param->column, dynamic_cast<BoolCell*>( arg ) );
			else
			{
				cout << "(!) Cannot pass " << nameOf( arg->value_type )
					 << " " << ID << " declared at " << arg->line << ":" << arg->column
					 << " to a parameter of type " << nameOf( param->data_type )
					 << " at " << param->line << ":" << param->column << endl;
				exit( 1 );
			}
		}
		call( params, block, args, t );
	}
}


Value* Sequence::interpret( SymbolTable t )
{
	for ( Stmt* b : body )
		b->interpret( t );
	return NULL;
}


Value* IfThen::interpret( SymbolTable t )
{
	BoolValue* value = dynamic_cast<BoolValue*>( test->interpret( t ) );
	if ( value->boolean )
		trueClause->interpret( t );
	return NULL;
}


Value* IfThenElse::interpret( SymbolTable t )
{
	BoolValue* value = dynamic_cast<BoolValue*>( test->interpret( t ) );
	if ( value->boolean )
		trueClause->interpret( t );
	else
		falseClause->interpret( t );
	return NULL;
}


Value* While::interpret( SymbolTable t )
{
	bool boolean;
	
	if ( test->interpret( t )->value_type == Value_BoolValue )
		boolean = dynamic_cast<BoolValue*>( test->interpret( t ) )->boolean;
	else
		boolean = dynamic_cast<BoolCell*>( test->interpret( t ) )->boolean;
	
	while ( boolean )
	{
		body->interpret( t );
		
		if ( test->interpret( t )->value_type == Value_BoolValue )
			boolean = dynamic_cast<BoolValue*>( test->interpret( t ) )->boolean;
		else
			boolean = dynamic_cast<BoolCell*>( test->interpret( t ) )->boolean;
	}
	return NULL;
}


Value* Prompt::interpret( SymbolTable t )
{
	string input;
	cout << message;
	getline( cin, input );
	return NULL;
}


Value* Prompt2::interpret( SymbolTable t )
{
	Value* lhs = t.lookUp( ID, line, column );
	
	if ( lhs->value_type == Value_IntCell )
	{
		int input;
		
		cout << message << " ";
		cin >> input;
		
		while( cin.fail() )
		{
			cout << "(!) Input has to be an integer" << endl;
			cout << message << " ";
			cin.clear();
			cin.ignore( 256, '\n' );
			cin >> input;
		}
		
		dynamic_cast<IntCell*>( lhs )->set( input );
	}
	else
	{
		cout << "(!) Expected " << nameOf( Value_IntCell )
			 << " at " << line << ":" << column
			 << " but found " << nameOf( lhs->value_type ) << endl;
		exit( 1 );
	}
	return NULL;
}


Value* Print::interpret( SymbolTable t )
{
	for ( Item* i : items )
	{
		if ( i->node_type == Node_ExprItem )
		{
			ExprItem* item = dynamic_cast<ExprItem*>( i );
			IntValue* value = dynamic_cast<IntValue*>( item->expr->interpret( t ) );
			cout << value->integer;
			delete item;
		}
		else
		{
			StringItem* item = dynamic_cast<StringItem*>( i );
			cout << item->message;
		}
	}
	cout << endl;
	return NULL;
}


Value* BinOp::interpret( SymbolTable t )
{
	Value* lhs = left->interpret( t );
	Value* rhs = right->interpret( t );
	
	if ( ( lhs->value_type == Value_BoolValue || lhs->value_type == Value_BoolCell ) &&
		 ( rhs->value_type == Value_BoolValue || rhs->value_type == Value_BoolCell ) &&
		 ( op == And || op == Or ) )
	{
		bool l_bool, r_bool;
		
		if ( lhs->value_type == Value_BoolValue )
			l_bool = dynamic_cast<BoolValue*>( lhs )->boolean;
		else
			l_bool = dynamic_cast<BoolCell*>( lhs )->boolean;
		if ( rhs->value_type == Value_BoolValue )
			r_bool = dynamic_cast<BoolValue*>( rhs )->boolean;
		else
			r_bool = dynamic_cast<BoolCell*>( rhs )->boolean;
		
		switch ( op )
		{
			case And:	return new BoolValue( l_bool && r_bool, line, column );
			case Or:	return new BoolValue( l_bool || r_bool, line, column );
			default:	return NULL;
		}
	}
	else if ( ( lhs->value_type == Value_IntValue || lhs->value_type == Value_IntCell ) &&
			  ( rhs->value_type == Value_IntValue || rhs->value_type == Value_IntCell ) &&
			  op != And && op != Or )
	{
		int l_int, r_int;
		
		if ( lhs->value_type == Value_IntValue )
			l_int = dynamic_cast<IntValue*>( lhs )->integer;
		else
			l_int = dynamic_cast<IntCell*>( lhs )->integer;
		if ( rhs->value_type == Value_IntValue )
			r_int = dynamic_cast<IntValue*>( rhs )->integer;
		else
			r_int = dynamic_cast<IntCell*>( rhs )->integer;
		
		switch ( op )
		{
			case EQ:	return new BoolValue( l_int == r_int, line, column );
			case NE:	return new BoolValue( l_int != r_int, line, column );
			case LE:	return new BoolValue( l_int <= r_int, line, column );
			case LT:	return new BoolValue( l_int < r_int, line, column );
			case GE:	return new BoolValue( l_int >= r_int, line, column );
			case GT:	return new BoolValue( l_int > r_int, line, column );
			case Plus:	return new IntValue( l_int + r_int, line, column );
			case Minus:	return new IntValue( l_int - r_int, line, column );
			case Times:	return new IntValue( l_int * r_int, line, column );
			case Div:	return new IntValue( l_int / r_int, line, column );
			case Mod:	return new IntValue( l_int % r_int, line, column );
			default:	return NULL;
		}
	}
	else
	{
		cout << "(!) Cannot perform operation " << nameOf( op )
			 << " on " << nameOf( lhs->value_type )
			 << " at " << lhs->line << ":" << lhs->column
			 << " and " << nameOf( rhs->value_type )
			 << " at " << rhs->line << ":" << rhs->column << endl;
		exit( 1 );
	}
}


Value* UnOp::interpret( SymbolTable t )
{
	Value* v = expr->interpret( t );
	
	if ( ( v->value_type == Value_IntValue || v->value_type == Value_IntCell ) && op == Neg )
	{
		int value;

		if ( v->value_type == Value_IntValue )
			value = dynamic_cast<IntValue*>( v )->integer;
		else
			value = dynamic_cast<IntCell*>( v )->integer;
		
		return new IntValue( -value, line, column );
	}
	else if ( ( v->value_type == Value_BoolValue || v->value_type == Value_BoolCell ) && op == Not )
	{
		bool value;
		
		if ( v->value_type == Value_BoolValue )
			value = dynamic_cast<BoolValue*>( v )->boolean;
		else
			value = dynamic_cast<BoolCell*>( v )->boolean;

		return new BoolValue( !value, line, column );
	}
	else
	{
		cout << "(!) Cannot perform operation " << nameOf( op )
			 << " on " << nameOf( v->value_type )
			 << " at " << v->line << ":" << v->column << endl;
		exit( 1 );
	}
}


Value* Num::interpret( SymbolTable t )
{
	return new IntValue( value, line, column );
}


Value* Id::interpret( SymbolTable t )
{
	Value* value = t.lookUp( ID, line, column );
	if ( value->value_type != Value_Undefined )
		return value;
	else
	{
		cout << "(!) Undefined " << ID << " at " << line << ":" << column << endl;
		exit( 1 );
	}
}


Value* True::interpret( SymbolTable t )
{
	return new BoolValue( boolean, line, column );
}


Value* False::interpret( SymbolTable t )
{
	return new BoolValue( boolean, line, column );
}



//===---------------------------------===//
// Dummy interpreters
//===---------------------------------===//
Value* Program::interpret( SymbolTable t ) { return NULL; }
Value* Block::interpret() { return NULL; }
Value* ConstDecl::interpret() { return NULL; }
Value* VarDecl::interpret() { return NULL; }
Value* ProcDecl::interpret() { return NULL; }
Value* ValParam::interpret() { return NULL; }
Value* ValParam::interpret( SymbolTable t ) { return NULL; }
Value* VarParam::interpret() { return NULL; }
Value* VarParam::interpret( SymbolTable t ) { return NULL; }
Value* Assign::interpret() { return NULL; }
Value* Call::interpret() { return NULL; }
Value* Sequence::interpret() { return NULL; }
Value* IfThen::interpret() { return NULL; }
Value* IfThenElse::interpret() { return NULL; }
Value* While::interpret() { return NULL; }
Value* Prompt::interpret() { return NULL; }
Value* Prompt2::interpret() { return NULL; }
Value* Print::interpret() { return NULL; }
Value* ExprItem::interpret() { return NULL; }
Value* ExprItem::interpret( SymbolTable t ) { return NULL; }
Value* StringItem::interpret() { return NULL; }
Value* StringItem::interpret( SymbolTable t ) { return NULL; }
Value* BinOp::interpret() { return NULL; }
Value* UnOp::interpret() { return NULL; }
Value* Num::interpret() { return NULL; }
Value* Id::interpret() { return NULL; }
Value* True::interpret() { return NULL; }
Value* False::interpret() { return NULL; }



//===----------------------------------------------------------------------===//
// Set and destroy Value
//===----------------------------------------------------------------------===//
void IntCell::set( int i )
{
	integer = i;
}

void BoolCell::set( bool b )
{
	boolean = b;
}

ProcValue::~ProcValue()
{
	params.remove_if( deleteAllParam );
	delete block;
}



//===----------------------------------------------------------------------===//
// Push a new pair of a program's ID and its corresponding map
// of value declerations and values to the symbol table
// Only called by Program and Call nodes
//===----------------------------------------------------------------------===//
void SymbolTable::enterTable( string ID, int line, int column )
{
	pair<string, map<string, Value*>* > symbol = make_pair( ID, new map<string, Value*>() );
	symbol_table.push( symbol );
}



//===----------------------------------------------------------------------===//
// Pop the top element of the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::exitTable()
{
	delete symbol_table.top().second;
	symbol_table.pop();
}



//===----------------------------------------------------------------------===//
// Create a new pair of value decleration and value
// and push it to the symbol table's top map
//===----------------------------------------------------------------------===//
void SymbolTable::bind( string ID, int line, int column, Value* v )
{
	Value* value = lookUp( ID, line, column );
	if ( value->value_type == Value_Undefined )
		symbol_table.top().second->insert( map<string, Value*>::value_type( ID, v ) );
	else
	{
		cout << "(!) " << ID << " is defined at " << value->line << ":" << value->column << " and should not be re-defined at " << line << ":" << column << endl;
		exit( 1 );
	}
	
	// debugging: print stack's top map
	for ( pair<string, Value*> p : *symbol_table.top().second )
	{
		cout << get<0>( p ) << ": " << nameOf( get<1>( p )->value_type );
		
		if ( get<1>( p )->value_type == Value_IntCell )
		{
			IntCell* c = dynamic_cast<IntCell*>( get<1>( p ) );
			cout << " = " << c->integer;
		}
		else if ( get<1>( p )->value_type == Value_BoolCell )
		{
			BoolCell* c = dynamic_cast<BoolCell*>( get<1>( p ) );
			
			if ( c->boolean )
				cout << " = true";
			else
				cout << " = false";
		}
		else if ( get<1>( p )->value_type == Value_IntValue )
		{
			IntValue* c = dynamic_cast<IntValue*>( get<1>( p ) );
			cout << " = " << c->integer;
		}
		else if ( get<1>( p )->value_type == Value_BoolValue )
		{
			BoolValue* c = dynamic_cast<BoolValue*>( get<1>( p ) );
			
			if ( c->boolean )
				cout << " = true";
			else
				cout << " = false";
		}
		else
		{
			ProcValue* c = dynamic_cast<ProcValue*>( get<1>( p ) );
			cout << " = " << c->params.size();
		}
		cout << endl;
		
	}
	cout << "---" << endl;
	//
}



//===----------------------------------------------------------------------===//
// Look up ID in the symbol table's top map
//===----------------------------------------------------------------------===//
Value* SymbolTable::lookUp( string ID, int line, int column )
{
	map<string, Value*>* map = symbol_table.top().second;
	if ( map->find( ID ) != map->end() )
		return map->at( ID );
	return new Value( line, column );
}



//===----------------------------------------------------------------------===//
// Return name of DataType, Op1, Op2, and ValueType
//===----------------------------------------------------------------------===//

string nameOf( DataType dataType )
{
	switch ( dataType )
	{
		case IntType:	return "Int";
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

string nameOf( ValueType valueType )
{
	switch ( valueType )
	{
		case Value_Undefined:	return "an undefined value";
		case Value_IntValue:	return "an integer value";
		case Value_BoolValue:	return "a boolean value";
		case Value_IntCell:		return "an integer cell";
		case Value_BoolCell:	return "a boolean cell";
		case Value_ProcValue:	return "a procedure value";
	}
}