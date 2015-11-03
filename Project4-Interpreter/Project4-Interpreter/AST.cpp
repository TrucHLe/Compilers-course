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
			delete cell;
			delete value;
		}
		else if ( lhs->value_type == Value_BoolCell && rhs->value_type == Value_BoolValue )
		{
			BoolCell* cell = dynamic_cast<BoolCell*>( lhs );
			BoolValue* value = dynamic_cast<BoolValue*>( rhs );
			cell->set( value->boolean );
			delete cell;
			delete value;
		}
		else
		{
			cout << "(!) Cannot assign " << nameOf( rhs->value_type )
				 << " at " << rhs->line << ":" << rhs->column
				 << " to " << ID << " at " << line << ":" << column
				 << ", which is " << nameOf( lhs->value_type ) << endl;
			exit( 1 );
		}
		delete rhs;
	}
	else
	{
		cout << "(!) Cannot assign to " << ID
			 << " at " << line << ":" << column
			 << ", which is " << nameOf( lhs->value_type ) << endl;
		exit( 1 );
	}
	delete lhs;
	return NULL;
}


Value* Call::interpret( SymbolTable t )
{
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
	
	delete value;
	return NULL;
}


void Call::call( list<Param*> params, Block* block, list<Value*> args, SymbolTable t )
{
	if ( params.empty() && args.empty() )
		block->interpret( t );
	else
	{
		Param* param = params.front();
		Value* arg = args.front();
		params.pop_front();
		args.pop_front();
		
		if ( param->node_type == Node_ValParam )
		{
			ValParam* val_param = dynamic_cast<ValParam*>( param );
			
			if ( val_param->data_type == IntType && arg->value_type == Value_IntValue )
			{
				IntValue* value = dynamic_cast<IntValue*>( arg );
				IntCell* cell = new IntCell( value->integer, arg->line, arg->column );
				t.bind( val_param->ID, arg->line, arg->column, cell );
				call( params, block, args, t );
				delete value;
				delete cell;
			}
			else if ( val_param->data_type == IntType )
			{
				cout << "(!) Expected " << nameOf( Value_IntValue )
					 << " at " << arg->line << ":" << arg->column
					 << " but found " << nameOf( arg->value_type ) << endl;
				exit( 1 );
			}
			else if ( val_param->data_type == BoolType && arg->value_type == Value_BoolValue )
			{
				BoolValue* value = dynamic_cast<BoolValue*>( arg );
				BoolCell* cell = new BoolCell( value->boolean, arg->line, arg->column );
				t.bind( val_param->ID, arg->line, arg->column, cell );
				call( params, block, args, t );
				delete value;
				delete cell;
			}
			else if ( val_param->data_type == BoolType )
			{
				cout << "(!) Expected " << nameOf( Value_BoolValue )
					 << " at " << arg->line << ":" << arg->column
					 << " but found " << nameOf( arg->value_type ) << endl;
				exit( 1 );
			}
			delete val_param;
		}
		else
		{
			VarParam* var_param = dynamic_cast<VarParam*>( param );
			
			if ( var_param->data_type == IntType && arg->value_type == Value_IntCell )
			{
				IntCell* cell = dynamic_cast<IntCell*>( arg );
				t.bind( var_param->ID, arg->line, arg->column, cell );
				call( params, block, args, t );
				delete cell;
			}
			else if ( var_param->data_type == IntType )
			{
				cout << "(!) Expected " << nameOf( Value_IntCell )
					 << " at " << arg->line << ":" << arg->column
					 << " but found " << nameOf( arg->value_type ) << endl;
				exit( 1 );
			}
			else if ( var_param->data_type == BoolType && arg->value_type == Value_BoolCell )
			{
				BoolCell* cell = dynamic_cast<BoolCell*>( arg );
				t.bind( var_param->ID, arg->line, arg->column, cell );
				call( params, block, args, t );
				delete cell;
			}
			else if ( var_param->data_type == BoolType )
			{
				cout << "(!) Expected " << nameOf( Value_BoolCell )
					 << " at " << arg->line << ":" << arg->column
					 << " but found " << nameOf( arg->value_type ) << endl;
				exit( 1 );
			}
			delete var_param;
		}
		delete param;
		delete arg;
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
	delete value;
	return NULL;
}


Value* IfThenElse::interpret( SymbolTable t )
{
	BoolValue* value = dynamic_cast<BoolValue*>( test->interpret( t ) );
	if ( value->boolean )
		trueClause->interpret( t );
	else
		falseClause->interpret( t );
	delete value;
	return NULL;
}


Value* While::interpret( SymbolTable t )
{
	BoolValue* value = dynamic_cast<BoolValue*>( test->interpret( t ) );
	while ( value->boolean )
	{
		body->interpret( t );
		value = dynamic_cast<BoolValue*>( test->interpret( t ) );
	}
	delete value;
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
		IntCell* cell = dynamic_cast<IntCell*>( lhs );
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
		
		cout << cell->integer << endl;
		cell->set( input );
		delete cell;
	}
	else
	{
		cout << "(!) Expected " << nameOf( Value_IntCell )
			 << " at " << line << ":" << column
			 << " but found " << nameOf( lhs->value_type ) << endl;
		exit( 1 );
	}
	delete lhs;
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
			delete value;
		}
		else
		{
			StringItem* item = dynamic_cast<StringItem*>( i );
			cout << item->message;
			delete item;
		}
	}
	cout << endl;
	return NULL;
}


Value* BinOp::interpret( SymbolTable t )
{
	Value* lhs = left->interpret( t );
	Value* rhs = right->interpret( t );
	
	
	// Using switch might be longer than using nested if,
	// but it is the most modular and straight-fwd
	switch ( op )
	{
		{case And:
			if ( lhs->value_type == Value_BoolCell && rhs->value_type == Value_BoolCell )
			{
				BoolCell* l_cell = dynamic_cast<BoolCell*>( lhs );
				BoolCell* r_cell = dynamic_cast<BoolCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->boolean && r_cell->boolean, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform operation " << nameOf( And )
					 << " on " << nameOf( lhs->value_type )
					 << " at " << lhs->line << ":" << lhs->column
					 << " and " << nameOf( rhs->value_type )
					 << " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case Or:
			if ( lhs->value_type == Value_BoolCell && rhs->value_type == Value_BoolCell )
			{
				BoolCell* l_cell = dynamic_cast<BoolCell*>( lhs );
				BoolCell* r_cell = dynamic_cast<BoolCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->boolean || r_cell->boolean, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform operation " << nameOf( Or )
				<< " on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case EQ:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer == r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '==' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case NE:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer != r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '<>' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
		
			
		{case LE:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer <= r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '<=' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case LT:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer < r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '<' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case GE:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer >= r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '>=' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case GT:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				BoolValue* value = new BoolValue( l_cell->integer > r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '>' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case Plus:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				IntValue* value = new IntValue( l_cell->integer + r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '+' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case Minus:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				IntValue* value = new IntValue( l_cell->integer - r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '-' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case Times:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				IntValue* value = new IntValue( l_cell->integer * r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform '*' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
			
			
		{case Div:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				IntValue* value = new IntValue( l_cell->integer / r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform 'div' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
		
		
		{case Mod:
			if ( lhs->value_type == Value_IntCell && rhs->value_type == Value_IntCell )
			{
				IntCell* l_cell = dynamic_cast<IntCell*>( lhs );
				IntCell* r_cell = dynamic_cast<IntCell*>( rhs );
				IntValue* value = new IntValue( l_cell->integer % r_cell->integer, line, column );
				delete l_cell;
				delete r_cell;
				delete lhs;
				delete rhs;
				return value;
			}
			else
			{
				cout << "(!) Cannot perform 'mod' on " << nameOf( lhs->value_type )
				<< " at " << lhs->line << ":" << lhs->column
				<< " and " << nameOf( rhs->value_type )
				<< " at " << rhs->line << ":" << rhs->column << endl;
				exit( 1 );
			}
		}
	}
}


Value* UnOp::interpret( SymbolTable t )
{
	Value* v = expr->interpret( t );
	
	switch ( op )
	{
		{case Neg:
			if ( v->value_type == Value_IntCell )
			{
				IntCell* cell = dynamic_cast<IntCell*>( v );
				IntValue* value = new IntValue( -cell->integer, line, column );
				delete cell;
				delete v;
				return value;
			}
			else
			{
				cout << "(!) Expected a " << nameOf( Value_IntCell )
					 << " at " << v->line << ":" << v->column
					 << " but found " << nameOf( v->value_type ) << endl;
				exit( 1 );
			}
		}
			
			
		{case Not:
			if ( v->value_type == Value_BoolCell )
			{
				BoolCell* cell = dynamic_cast<BoolCell*>( v );
				BoolValue* value = new BoolValue( !cell->boolean, line, column );
				delete cell;
				delete v;
				return value;
			}
			else
			{
				cout << "(!) Expected a " << nameOf( Value_BoolCell )
				<< " at " << v->line << ":" << v->column
				<< " but found " << nameOf( v->value_type ) << endl;
				exit( 1 );

			}
		}
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
	pair<string, map<string, Value>* > symbol = make_pair( ID, new map<string, Value>() );
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
		symbol_table.top().second->insert( map<string, Value>::value_type( ID, *v ) );
	else
	{
		cout << "(!) " << ID << " is defined at " << value->line << ":" << value->column << " and should not be re-defined at " << line << ":" << column << endl;
		exit( 1 );
	}
	
	// Debugging: print stack's top map
	for ( pair<string, Value> p : *symbol_table.top().second )
	{
		cout << get<0>( p ) << ": " << nameOf( get<1>( p ).value_type ) << endl;
		if ( get<1>( p ).value_type == Value_IntCell )
		{
			IntCell* c = dynamic_cast<IntCell*>( &get<1>( p ) );
			cout << c->integer << endl; //sth wrong here
		}
	}
	cout << "---" << endl;
	//
}



//===----------------------------------------------------------------------===//
// Look up ID in the symbol table's top map
//===----------------------------------------------------------------------===//
Value* SymbolTable::lookUp( string ID, int line, int column )
{
	map<string, Value>* map = symbol_table.top().second;
	if ( map->find( ID ) != map->end() )
		return &map->at( ID );
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