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
	for ( ConstDecl* c : consts )
		delete c;
	for ( VarDecl* v : vars )
		delete v;
	for ( ProcDecl* p : procs )
		delete p;
	for ( Stmt* b : body )
		delete b;
}

ProcDecl::~ProcDecl()
{	
	for ( Param* p : params )
		delete p;
	delete block;
}

Assign::~Assign()
{
	delete expr;
}

Call::~Call()
{
	for ( Expr* a : args )
		delete a;
}

Sequence::~Sequence()
{
	for ( Stmt* b : body )
		delete b;
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
	for ( Item* i : items )
		if ( i->node_type == Node_ExprItem )
			delete i;
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
	Value* rhs = expr->interpret( t );
	
	if ( lhs->value_type == Value_IntCell )
		lhs->setValue( rhs->getIntValue( rhs->line, rhs->column ), lhs->line, lhs->column );
	else if ( lhs->value_type == Value_BoolCell )
		lhs->setValue( rhs->getBoolValue( rhs->line, rhs->column ), lhs->line, lhs->column );
	else
	{
		cout << "(!) Cannot perform assignment on " << ID
			 << " at " << line << ":" << column
			 << " because it is " << nameOf( lhs->value_type ) << endl;
		exit( 1 );
	}
	return NULL;
}


Value* Call::interpret( SymbolTable t )
{
	Value* look_up = t.lookUp( ID, line, column );
	
	if ( look_up == NULL )
	{
		cout << "(!) Expected " << nameOf( Value_ProcValue )
		<< " at " << line << ":" << column
		<< " but found " << nameOf( Value_Undefined ) << endl;
		exit( 1 );
	}
	
	if ( look_up->value_type != Value_ProcValue )
	{
		cout << "(!) Expected " << nameOf( Value_ProcValue )
			 << " at " << line << ":" << column
			 << " but found " << nameOf( look_up->value_type ) << endl;
		exit( 1 );
	}
	
	
	ProcValue* value = dynamic_cast<ProcValue*>( look_up );
	list<Value*> arguments;
	
	for ( Expr* arg : args )
	{
		Value* v = arg->interpret( t );
		arguments.push_back( v );
	}
	
	
	if ( value->params.size() != arguments.size() )
	{
		cout << "(!) The number of parameters does not match the number of arguments of "
			 << ID << " at " << line << ":" << column << endl;
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
			
			if ( param->data_type == IntType )
				t.bind( param->ID, param->line, param->column, new IntCell( arg->getIntValue( arg->line, arg->column ), param->line, param->column ) );
			else
				t.bind( param->ID, param->line, param->column, new BoolCell( arg->getBoolValue( arg->line, arg->column ), param->line, param->column ) );
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
				<< " " << ID << " defined at " << arg->line << ":" << arg->column
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
	Value* value = test->interpret( t );
	if ( value->getBoolValue( line, column ) )
		trueClause->interpret( t );
	return NULL;
}


Value* IfThenElse::interpret( SymbolTable t )
{
	Value* value = test->interpret( t );
	if ( value->getBoolValue( line, column ) )
		trueClause->interpret( t );
	else
		falseClause->interpret( t );
	return NULL;
}


Value* While::interpret( SymbolTable t )
{
	Value* value = test->interpret( t );
	while ( value->getBoolValue( line, column ) )
	{
		body->interpret( t );
		value = test->interpret( t );
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
	string input;
	
	cout << message << " ";
	getline( cin, input );
	
	try
	{
		lhs->setValue( stoi( input ), line, column );
	}
	catch ( std::invalid_argument )
	{
		cout << "(!) Input is not an interger" << endl;
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
			Value* value = item->expr->interpret( t );
			cout << value->getIntValue( line, column );
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
	
	switch ( op )
	{
		case And:	return new BoolValue( lhs->getBoolValue( lhs->line, lhs->column ) && rhs->getBoolValue( rhs->line, rhs->column ), line, column );
		case Or:	return new BoolValue( lhs->getBoolValue( lhs->line, lhs->column ) || rhs->getBoolValue( rhs->line, rhs->column ), line, column );
		case EQ:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) == rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case NE:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) != rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case LE:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) <= rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case LT:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) < rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case GE:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) >= rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case GT:	return new BoolValue( lhs->getIntValue( lhs->line, lhs->column ) > rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case Plus:	return new IntValue( lhs->getIntValue( lhs->line, lhs->column ) + rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case Minus:	return new IntValue( lhs->getIntValue( lhs->line, lhs->column ) - rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case Times:	return new IntValue( lhs->getIntValue( lhs->line, lhs->column ) * rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case Div:	return new IntValue( lhs->getIntValue( lhs->line, lhs->column ) / rhs->getIntValue( rhs->line, rhs->column ), line, column );
		case Mod:	return new IntValue( lhs->getIntValue( lhs->line, lhs->column ) % rhs->getIntValue( rhs->line, rhs->column ), line, column );
	}
}


Value* UnOp::interpret( SymbolTable t )
{
	Value* value = expr->interpret( t );
	
	switch ( op )
	{
		case Neg:
			return new IntValue( -value->getIntValue( line, column ), line, column );
		case Not:
			return new BoolValue( !value->getBoolValue( line, column ), line, column );
	}
}


Value* Num::interpret( SymbolTable t )
{
	return new IntValue( value, line, column );
}


Value* Id::interpret( SymbolTable t )
{
	Value* value = t.lookUp( ID, line, column );
	if ( value != NULL )
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



//===----------------------------------------------------------------------===//
// Set, get, and destroy Value
//===----------------------------------------------------------------------===//
int IntValue::getIntValue( int lin, int col )
{
	return integer;
}

bool IntValue::getBoolValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_BoolValue )
		 << " from the integer value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void IntValue::setValue( int i, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_IntValue )
		 << " to the integer value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void IntValue::setValue( bool b, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_BoolValue )
		 << " to the integer value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}



int BoolValue::getIntValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_IntValue )
		 << " from the boolean value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

bool BoolValue::getBoolValue( int lin, int col )
{
	return boolean;
}

void BoolValue::setValue( int i, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_IntValue )
		 << " to the boolean value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void BoolValue::setValue( bool b, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << ": cannot set " << nameOf( Value_BoolValue )
		 << " to the boolean value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}



int IntCell::getIntValue( int lin, int col )
{
	return integer;
}

bool IntCell::getBoolValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_BoolValue )
		 << " from the integer cell defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void IntCell::setValue( int i, int lin, int col )
{
	integer = i;
}

void IntCell::setValue( bool b, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_BoolValue )
		 << " to the integer cell defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}



int BoolCell::getIntValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_IntValue )
		 << " from the boolean cell defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

bool BoolCell::getBoolValue( int lin, int col )
{
	return boolean;
}

void BoolCell::setValue( int i, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_IntValue )
		 << " to the boolean cell defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void BoolCell::setValue( bool b, int lin, int col )
{
	boolean = b;
}



int ProcValue::getIntValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_IntValue )
		 << " from the procedure value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

bool ProcValue::getBoolValue( int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot get " << nameOf( Value_BoolValue )
		 << " from the procedure value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void ProcValue::setValue( int i, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_IntValue )
		 << " to the procedure value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

void ProcValue::setValue( bool b, int lin, int col )
{
	cout << "(!) At " << lin << ":" << col
		 << " : cannot set " << nameOf( Value_BoolValue )
		 << " to the procedure value defined at "
		 << line << ":" << column << endl;
	exit( 1 );
}

ProcValue::~ProcValue()
{
	for ( Param* p : params )
		delete p;
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
	symbol_table.push_back( symbol );
}



//===----------------------------------------------------------------------===//
// Pop the top element of the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::exitTable()
{
	delete symbol_table.back().second;
	symbol_table.pop_back();
}



//===----------------------------------------------------------------------===//
// Create a new pair of value decleration and value
// and push it to the symbol table's top map
//===----------------------------------------------------------------------===//
void SymbolTable::bind( string ID, int line, int column, Value* v )
{
	map<string, Value*>* current_map = symbol_table.back().second;
	if ( current_map->find( ID ) == current_map->end() )
		current_map->insert( map<string, Value*>::value_type( ID, v ) );
	else
	{
		cout << "(!) " << ID << " is defined at " << v->line << ":" << v->column
			 << " and should not be re-defined at " << line << ":" << column << endl;
		exit( 1 );
	}
	
	//printFrontMap();
}



//===----------------------------------------------------------------------===//
// Print the map of the symbol table's front element for debugging
//===----------------------------------------------------------------------===//
void SymbolTable::printFrontMap()
{
	cout << "---" << symbol_table.back().first << "---" << endl;
	for ( pair<string, Value*> p : *symbol_table.back().second )
	{
		cout << p.first; //<< " :" << nameOf( p.second->value_type );
		
		if ( p.second->value_type == Value_IntValue || p.second->value_type == Value_IntCell )
			cout << " = " << p.second->getIntValue( p.second->line, p.second->column );
		else if ( p.second->value_type == Value_BoolValue || p.second->value_type == Value_BoolCell )
		{
			if ( p.second->getBoolValue( p.second->line, p.second->column ) )
				cout << " = true";
			else
				cout << " = false";
		}
		else
		{
			ProcValue* v = dynamic_cast<ProcValue*>( p.second );
			cout << " = " << v->params.size() << " parameters";
		}
		cout << endl;
	}
	cout << endl;
}


//===----------------------------------------------------------------------===//
// Look up ID in the symbol table's top map
//===----------------------------------------------------------------------===//
Value* SymbolTable::lookUp( string ID, int line, int column )
{
	for ( int i = (int) symbol_table.size() - 1; i > -1; --i )
	{
		map<string, Value*>* map = symbol_table.at( i ).second;
		if ( map->find( ID ) != map->end() )
			return map->at( ID );
	}
	return NULL;
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