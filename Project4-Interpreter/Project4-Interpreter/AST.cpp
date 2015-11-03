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



//===---------------------------------===//
// Dummy interpreters
//===---------------------------------===//
void Block::interpret() {}
void ConstDecl::interpret() {}
void VarDecl::interpret() {}
void ProcDecl::interpret() {}
void ValParam::interpret() {}
void VarParam::interpret() {}
void Assign::interpret() {}
void Call::interpret() {}

void ValParam::interpret( SymbolTable t ) {}
void VarParam::interpret( SymbolTable t ) {}
void Program::interpret( SymbolTable t ) {}

Value Program::interpretValue( SymbolTable t ) { return Value(); }
Value Block::interpretValue( SymbolTable t ) { return Value(); }
Value ConstDecl::interpretValue( SymbolTable t ) { return Value(); }
Value VarDecl::interpretValue( SymbolTable t ) { return Value(); }
Value ProcDecl::interpretValue( SymbolTable t ) { return Value(); }
Value ValParam::interpretValue( SymbolTable t ) { return Value(); }
Value VarParam::interpretValue( SymbolTable t ) { return Value(); }
Value Assign::interpretValue( SymbolTable t ) { return Value(); }
Value Call::interpretValue( SymbolTable t ) { return Value(); }

//===----------------------------------------------------------------------===//
// Interpret nodes and call functions
//===----------------------------------------------------------------------===//
void Program::interpret()
{
	SymbolTable t = SymbolTable();
	t.enterTable( name, line, column );
	block->interpret( t );
	t.exitTable();
}


void Block::interpret( SymbolTable t )
{
	for ( ConstDecl* c : consts )
		c->interpret( t );
	for ( VarDecl* v : vars )
		v->interpret( t );
	for ( ProcDecl* p : procs )
		p->interpret( t );
	for ( Stmt* b : body )
		b->interpret( t );
}


void ConstDecl::interpret( SymbolTable t )
{
	t.bind( ID, line, column, IntValue( value, line, column ) );
}


void VarDecl::interpret( SymbolTable t )
{
	if ( data_type == IntType )
		t.bind( ID, line, column, IntCell( 0, line, column ) );
	else
		t.bind( ID, line, column, BoolCell( false, line, column ) );
}


void ProcDecl::interpret( SymbolTable t )
{
	t.bind( ID, line, column, ProcValue( params, block, line, column ) );
}


void Assign::interpret( SymbolTable t )
{
	Value lhs = t.lookUp( ID, line, column );
	
	if ( lhs.value_type == Value_IntCell || lhs.value_type == Value_BoolCell )
	{
		Value rhs = expr->interpretValue( t );
		Value* lhs_ptr = &lhs;
		Value* rhs_ptr = &rhs;
		
		if ( lhs.value_type == Value_IntCell && rhs.value_type == Value_IntValue )
		{
			IntCell* cell = dynamic_cast<IntCell*>( lhs_ptr );
			IntValue* value = dynamic_cast<IntValue*>( rhs_ptr );
			cell->set( value->int_value );
		}
		else if ( lhs.value_type == Value_BoolCell && rhs.value_type == Value_BoolValue )
		{
			BoolCell* cell = dynamic_cast<BoolCell*>( lhs_ptr );
			BoolValue* value = dynamic_cast<BoolValue*>( rhs_ptr );
			cell->set( value->bool_value );
		}
		else
		{
			cout << "(!) Cannot assign " << nameOf( rhs.value_type )
				 << " at " << rhs.line << ":" << rhs.column
				 << " to " << ID << " at " << line << ":" << column
				 << ", which is " << nameOf( lhs.value_type ) << endl;
			exit( 1 );
		}
	}
	else
	{
		cout << "(!) Cannot assign to " << ID
			 << " at " << line << ":" << column
			 << ", which is " << nameOf( lhs.value_type ) << endl;
		exit( 1 );
	}
}


void Call::interpret( SymbolTable t )
{
	Value value = t.lookUp( ID, line, column );
	Value* ptr = &value;
	ProcValue* proc_value = dynamic_cast<ProcValue*>( ptr );
	list<Value> arguments;
	
	for ( Expr* arg : args )
	{
		Value v = arg->interpretValue( t );
		arguments.push_back( v );
	}
	
	if ( proc_value->params.size() != arguments.size() )
	{
		cout << "(!) The number of parameters does not match the number of arguments of " << ID
			 << " at " << line << ":" << column << endl;
		exit( 1 );
	}
	
	t.enterTable( ID, line, column );
	call( proc_value->params, proc_value->block, arguments, t );
	t.exitTable();
}


void Call::call( list<Param*> params, Block* block, list<Value> args, SymbolTable t )
{
	if ( params.empty() && args.empty() )
		block->interpret( t );
	else
	{
		Param* param = params.front();
		Value arg = args.front();
		Value* ptr = &arg;
		params.pop_front();
		args.pop_front();
		
		if ( param->node_type == Node_ValParam )
		{
			ValParam* val_param = dynamic_cast<ValParam*>( param );
			
			if ( val_param->data_type == IntType && arg.value_type == Value_IntValue )
			{
				IntValue* intValue = dynamic_cast<IntValue*>( ptr );
				IntCell cell = IntCell( intValue->int_value, arg.line, arg.column );
				t.bind( val_param->ID, arg.line, arg.column, cell );
				call( params, block, args, t );
			}
			else if ( val_param->data_type == IntType )
			{
				cout << "(!) Expected " << nameOf( Value_IntValue )
					 << " at " << arg.line << ":" << arg.column
					 << " but found " << nameOf( arg.value_type ) << endl;
				exit( 1 );
			}
			else if ( val_param->data_type == BoolType && arg.value_type == Value_BoolValue )
			{
				BoolValue* boolValue = dynamic_cast<BoolValue*>( ptr );
				BoolCell cell = BoolCell( boolValue->bool_value, arg.line, arg.column );
				t.bind( val_param->ID, arg.line, arg.column, cell );
				call( params, block, args, t );
			}
			else if ( val_param->data_type == BoolType )
			{
				cout << "(!) Expected " << nameOf( Value_BoolValue )
					 << " at " << arg.line << ":" << arg.column
					 << " but found " << nameOf( arg.value_type ) << endl;
				exit( 1 );
			}
		}
		else
		{
			VarParam* var_param = dynamic_cast<VarParam*>( param );
			
			if ( var_param->data_type == IntType && arg.value_type == Value_IntCell )
			{
				IntCell* cell_ptr = dynamic_cast<IntCell*>( ptr );
				IntCell cell = *cell_ptr;
				t.bind( var_param->ID, arg.line, arg.column, cell );
				call( params, block, args, t );
			}
			else if ( var_param->data_type == IntType )
			{
				cout << "(!) Expected " << nameOf( Value_IntCell )
					 << " at " << arg.line << ":" << arg.column
					 << " but found " << nameOf( arg.value_type ) << endl;
				exit( 1 );
			}
			else if ( var_param->data_type == BoolType && arg.value_type == Value_BoolCell )
			{
				BoolCell* cell_ptr = dynamic_cast<BoolCell*>( ptr );
				BoolCell cell = *cell_ptr;
				t.bind( var_param->ID, arg.line, arg.column, cell );
				call( params, block, args, t );
			}
			else if ( var_param->data_type == BoolType )
			{
				cout << "(!) Expected " << nameOf( Value_BoolCell )
					 << " at " << arg.line << ":" << arg.column
					 << " but found " << nameOf( arg.value_type ) << endl;
				exit( 1 );
			}
		}
	}
}



void Sequence::interpret()
{
	
}

void Sequence::interpret( SymbolTable t )
{
	
}

Value Sequence::interpretValue( SymbolTable t )
{
	
}



void IfThen::interpret()
{
	
}

void IfThen::interpret( SymbolTable t )
{
	
}

Value IfThen::interpretValue( SymbolTable t )
{
	
}



void IfThenElse::interpret()
{
	
}

void IfThenElse::interpret( SymbolTable t )
{
	
}

Value IfThenElse::interpretValue( SymbolTable t )
{
	
}



void While::interpret()
{
	
}

void While::interpret( SymbolTable t )
{
	
}

Value While::interpretValue( SymbolTable t )
{
	
}



void Prompt::interpret()
{
	
}

void Prompt::interpret( SymbolTable t )
{
	
}

Value Prompt::interpretValue( SymbolTable t )
{
	
}



void Prompt2::interpret()
{
	
}

void Prompt2::interpret( SymbolTable t )
{
	
}

Value Prompt2::interpretValue( SymbolTable t )
{
	
}



void Print::interpret()
{
	
}

void Print::interpret( SymbolTable t )
{
	
}

Value Print::interpretValue( SymbolTable t )
{
	
}



void ExprItem::interpret()
{
	
}

void ExprItem::interpret( SymbolTable t )
{
	
}

Value ExprItem::interpretValue( SymbolTable t )
{
	
}



void StringItem::interpret()
{
	
}

void StringItem::interpret( SymbolTable t )
{
	
}

Value StringItem::interpretValue( SymbolTable t )
{
	
}



void BinOp::interpret()
{
	
}

void BinOp::interpret( SymbolTable t )
{
	
}

Value BinOp::interpretValue( SymbolTable t )
{
	
}



void UnOp::interpret()
{
	
}

void UnOp::interpret( SymbolTable t )
{
	
}

Value UnOp::interpretValue( SymbolTable t )
{
	
}



void Num::interpret()
{
	
}

void Num::interpret( SymbolTable t )
{
	
}

Value Num::interpretValue( SymbolTable t )
{
	
}



void Id::interpret()
{
	
}

void Id::interpret( SymbolTable t )
{
	
}

Value Id::interpretValue( SymbolTable t )
{
	
}



void True::interpret()
{
	
}

void True::interpret( SymbolTable t )
{
	
}

Value True::interpretValue( SymbolTable t )
{
	
}



void False::interpret()
{
	
}

void False::interpret( SymbolTable t )
{
	
}

Value False::interpretValue( SymbolTable t )
{
	
}



//===----------------------------------------------------------------------===//
// Set and destroy Value
//===----------------------------------------------------------------------===//
void IntCell::set( int i )
{
	int_value = i;
}

void BoolCell::set( bool b )
{
	bool_value = b;
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
	pair<string, map<string, Value> > symbol = make_pair( ID, map<string, Value>() );
	symbol_table.push( symbol );
}



//===----------------------------------------------------------------------===//
// Pop the top element of the symbol table
//===----------------------------------------------------------------------===//
void SymbolTable::exitTable()
{
	symbol_table.pop();
}



//===----------------------------------------------------------------------===//
// Create a new pair of value decleration and value
// and push it to the symbol table's top map
//===----------------------------------------------------------------------===//
void SymbolTable::bind( string ID, int line, int column, Value v )
{
	Value value = lookUp( ID, line, column );
	if ( value.value_type == Value_Undefined )
	{
		pair<string, Value> new_value = make_pair( ID, v );
		get<1>(symbol_table.top()).insert( new_value );
	}
	else
	{
		cout << "(!) " << ID << " is defined at " << value.line << ":" << value.column << " and should not be re-defined at " << line << ":" << column << endl;
		exit( 1 );
	}
	
}



//===----------------------------------------------------------------------===//
// Look up ID in the symbol table's top map
//===----------------------------------------------------------------------===//
Value SymbolTable::lookUp( string ID, int line, int column )
{
	map<string, Value> map = get<1>(symbol_table.top());
	if ( map.find( ID ) != map.end() )
		return map.at( ID );
	return Value( line, column );
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
		default:		return "";
	}
}

string nameOf( Op1 op1 )
{
	switch ( op1 )
	{
		case Neg:	return "Neg";
		case Not:	return "Not";
		default:	return "";
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
		default:	return "";
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