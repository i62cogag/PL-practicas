/*!
	\file    ast.cpp
	\brief   Code of funcitons of AST clas
	\author
	\date    2018-12-13
	\version 1.0
*/

#include <iostream>
#include <stdlib.h>
#include <string>
#include <list>

// Para usar la funciones pow y std::abs
#include <cmath>

#include "ast.hpp"

#include "../table/table.hpp"

// warning
#include "../error/error.hpp"

// Macros for the screen
#include "../includes/macros.hpp"

//
#include "../table/numericVariable.hpp"
#include "../table/logicalVariable.hpp"
#include "../table/alfaNumericVariable.hpp"

#include "../table/numericConstant.hpp"
#include "../table/logicalConstant.hpp"

#include "../table/builtinParameter0.hpp"
#include "../table/builtinParameter1.hpp"
#include "../table/builtinParameter2.hpp"

#include "../parser/pie.tab.h"


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

extern lp::Table table; //!< Reference to the Table of Symbols


extern lp::AST *root; //!< Reference to the object at the base of the AST


///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::VariableNode::getType()
{
	// Get the identifier in the table of symbols as Variable
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Return the type of the Variable
	return var->getType();
}


void lp::VariableNode::print()
{
  std::cout << "VariableNode: " << this->_id << std::endl;
  std::cout << "Type: " << this->getType() << std::endl;
}


double lp::VariableNode::evaluateNumber()
{
	double result = 0.0;

	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as NumericVariable
		lp::NumericVariable *var = (lp::NumericVariable *) table.getSymbol(this->_id);

		// Copy the value of the NumericVariable
		result = var->getValue();
	}
	else
	{
		warning("Runtime error in evaluateNumber(): the variable is not numeric",
				   this->_id);
	}

	// Return the value of the NumericVariable
	return result;
}


bool lp::VariableNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		// Get the identifier in the table of symbols as LogicalVariable
		lp::LogicalVariable *var = (lp::LogicalVariable *) table.getSymbol(this->_id);

		// Copy the value of the LogicalVariable
		result = var->getValue();
	}
	else
	{
		warning("Runtime error in evaluateBool(): the variable is not boolean",
				   this->_id);
	}

	// Return the value of the LogicalVariable
	return result;
}

std::string lp::VariableNode::evaluateCadena()
{
	std::string result = "";

	if (this->getType() == CADENA)
	{
		// Get the identifier in the table of symbols as alfanumericVariable
		lp::alfanumericVariable *var = (lp::alfanumericVariable *) table.getSymbol(this->_id);

		// Copy the value of the LogicalVariable
		result = var->getValue();
	}
	else
	{
		warning("Runtime error in evaluateCadena(): the variable is not CADENA",
				   this->_id);
	}

	// Return the value of the alfanumericVariable
	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ConstantNode::print()
{
  std::cout << "ConstantNode: " << this->_id << std::endl;
  std::cout << "Type: " << this->getType() << std::endl;
}

int lp::ConstantNode::getType()
{
	// Get the identifier in the table of symbols as Constant
	lp::Constant *var = (lp::Constant *) table.getSymbol(this->_id);

	// Return the type of the Constant
	return var->getType();
}


double lp::ConstantNode::evaluateNumber()
{
	double result = 0.0;

	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as NumericConstant
		lp::NumericConstant *constant = (lp::NumericConstant *) table.getSymbol(this->_id);

		// Copy the value of the NumericConstant
		result = constant->getValue();
	}
	else
	{
		warning("Runtime error in evaluateNumber(): the constant is not numeric",
				   this->_id);
	}

	// Return the value of the NumericVariable
	return result;
}

bool lp::ConstantNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		// Get the identifier in the table of symbols as LogicalConstant
		lp::LogicalConstant *constant = (lp::LogicalConstant *) table.getSymbol(this->_id);

		// Copy the value of the LogicalConstant
		result = constant->getValue();
	}
	else
	{
		warning("Runtime error in evaluateBool(): the constant is not boolean",
				   this->_id);
	}

	// Return the value of the LogicalVariable
	return result;
}


//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


int lp::NumberNode::getType()
{
	return NUMBER;
}


void lp::NumberNode::print()
{
  std::cout << "NumberNode: " << this->_number << std::endl;
}

double lp::NumberNode::evaluateNumber()
{
    return this->_number;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::CadenasNode::getType()
{
	return CADENA;
}

void lp::CadenasNode::print()
{
	std::cout << "CadenasNode: " << this->_cadena << std::endl;
}

std::string lp::CadenasNode::evaluateCadena()
{
	return this->_cadena;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int lp::NumericUnaryOperatorNode::getType()
{
	int result;

	if(this->_exp->getType() == NUMBER)
	{
		result = NUMBER;
	}
	else
	{
		warning("Runtime error: incompatible types for", "Numeric Unary Operator");
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::LogicalUnaryOperatorNode::getType()
{
	int result;

	if(this->_exp->getType() == BOOL)
	{
		result = BOOL;
	}
	else
	{
		warning("Runtime error: incompatible types for", "Logical Unary Operator");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::NumericOperatorNode::getType()
{
	int result = 0;

	if ( (this->_left->getType() == NUMBER) and (this->_right->getType() == NUMBER))
		result = NUMBER;
	else
		warning("Runtime error: incompatible types for", "Numeric Operator");

	return	result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


int lp::RelationalOperatorNode::getType()
{
	int result = 0;

	if ( (this->_left->getType() == NUMBER) and (this->_right->getType() == NUMBER))
		result = BOOL;
	else if ( (this->_left->getType() == BOOL) and (this->_right->getType() == BOOL))
		result = BOOL;
	else if ((this->_left->getType() == CADENA) and (this->_right->getType() == CADENA))
		result = BOOL;
	else
		warning("Runtime error: incompatible types for", "Relational Operator");

	return	result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::LogicalOperatorNode:: getType()
{
	int result = 0;

	if ( (this->_left->getType() == BOOL) and (this->_right->getType() == BOOL))
	{
		//
		result = BOOL;
	}
	else
		warning("Runtime error: incompatible types for", "Logical Operator");

	return	result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::AlphaNumericOperatorNode::getType()
{
	int result = 0;

	if( (this->_left->getType() == CADENA) and (this->_right->getType() == CADENA)){
		result = CADENA;
	}else{
		warning("Runtime error: incompatible types for", "AlphaNumeric Operator");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::UnaryMinusNode::print()
{
  std::cout << "UnaryMinusNode: "  << std::endl;
  std::cout << "-";
  this->_exp->print();
}

double lp::UnaryMinusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		// Minus
		result = - this->_exp->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ", "UnaryMinus");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::UnaryPlusNode::print()
{
  std::cout << "UnaryPlusNode: "  << std::endl;
  this->_exp->print();
}

double lp::UnaryPlusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		result = this->_exp->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","UnaryPlus");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PlusNode::print()
{
  std::cout << "PlusNode: "  << std::endl;
  this->_left->print();
  std::cout << " + ";
  this->_right->print();
}

double lp::PlusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() + this->_right->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ", "Plus");
	}

  return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::MinusNode::print()
{
  std::cout << "MinusNode: "  << std::endl;
  this->_left->print();
  std::cout << " - ";
  this->_right->print();
}

double lp::MinusNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() - this->_right->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ", "Minus");
	}

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////


void lp::MultiplicationNode::print()
{
	std::cout << "MultiplicationNode: "  << std::endl;
	this->_left->print();
	std::cout << " * ";
	this->_right->print();
}

double lp::MultiplicationNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = this->_left->evaluateNumber() * this->_right->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for ","Multiplication");
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::DivisionNode::print()
{
  std::cout << "DivisionNode: " << std::endl;
  this->_left->print();
  std::cout << " / ";
  this->_right->print();
}

double lp::DivisionNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		double leftNumber, rightNumber;

		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();

		// The divisor is not zero
    	if(std::abs(rightNumber) > ERROR_BOUND)
		{
				result = leftNumber / rightNumber;
		}
		else
		{
			warning("Runtime error", "Division by zero");
		}
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for", "Division");
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ModuloNode::print()
{
  std::cout << "ModuloNode: " << std::endl;
  this->_left->print();
  std::cout << " / ";
  this->_right->print();
}

double lp::ModuloNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		double leftNumber, rightNumber;

		leftNumber = this->_left->evaluateNumber();
		rightNumber = this->_right->evaluateNumber();

    	if(std::abs(rightNumber) > ERROR_BOUND)
				result = (int) leftNumber % (int) rightNumber;
		else
			warning("Runtime error", "Division by zero");
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for", "Modulo");
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::PowerNode::print()
{
	std::cout << "PowerNode: "  << std::endl;
	this->_left->print();
	std::cout << " ^ ";
	this->_right->print();
}

double lp::PowerNode::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		result = pow(this->_left->evaluateNumber(), this->_right->evaluateNumber());
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for", "Power");
	}

  return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::QuotientNode::print()
{
	std::cout << "QuotientNode: "  << std::endl;
	this->_left->print();
	std::cout << " quotient ";
	this->_right->print();
}

double lp::QuotientNode::evaluateNumber()
{
	int integerResult = 0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		integerResult = this->_left->evaluateNumber() / this->_right->evaluateNumber();
	}
	else
	{
		warning("Runtime error: the expressions are not numeric for", "quotient");
	}

	double result = integerResult;
 	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
int lp::BuiltinFunctionNode_0::getType()
{
	return	NUMBER;
}


void lp::BuiltinFunctionNode_0::print()
{
	std::cout << "BuiltinFunctionNode_0: "  << std::endl;
	std::cout << this->_id;
	std::cout << " ( ) " ;
}

double lp::BuiltinFunctionNode_0::evaluateNumber()
{
	// Get the identifier in the table of symbols as BuiltinParameter0
	lp::BuiltinParameter0 *f = (lp::BuiltinParameter0 *) table.getSymbol(this->_id);

	// Apply the function and copy the result
   	return f->getFunction()();
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::BuiltinFunctionNode_1::getType()
{
	int result = 0;

	if (this->_exp->getType() == NUMBER)
		result = NUMBER;
	else
		warning("Runtime error: incompatible type for", "BuiltinFunctionNode_1");

	return	result;
}

void lp::BuiltinFunctionNode_1::print()
{
	std::cout << "BuiltinFunctionNode_1: "  << std::endl;
	std::cout << this->_id;
	std::cout << " ( " ;
	this->_exp->print();
	std::cout << " ) " ;
}

double lp::BuiltinFunctionNode_1::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the type of the expression
	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as BuiltinParameter1
		lp::BuiltinParameter1 *f = (lp::BuiltinParameter1 *) table.getSymbol( this->_id);

		// Apply the function to the parameter and copy the result
		result = f->getFunction()(this->_exp->evaluateNumber());
	}
	else
	{
		warning("Runtime error: incompatible type of parameter for ", this->_id);
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int lp::BuiltinFunctionNode_2::getType()
{
	int result = 0;

	if (this->_exp1->getType() == this->_exp2->getType())
		result = this->_exp1->getType();
	else
		warning("Runtime error: incompatible types for", "BuiltinFunctionNode_2");

	return	result;
}


void lp::BuiltinFunctionNode_2::print()
{
	std::cout << "BuiltinFunctionNode_2: "  << std::endl;
	std::cout << this->_id;
	std::cout << " ( " ;
	this->_exp1->print();
	std::cout << " , " ;
	this->_exp2->print();
	std::cout << " ) " ;
}

double lp::BuiltinFunctionNode_2::evaluateNumber()
{
	double result = 0.0;

	// Ckeck the types of the expressions
	if (this->getType() == NUMBER)
	{
		// Get the identifier in the table of symbols as BuiltinParameter2
		lp::BuiltinParameter2 *f = (lp::BuiltinParameter2 *) table.getSymbol(this->_id);

		// Apply the function to the parameters and copy the result
	  	result = f->getFunction()(this->_exp1->evaluateNumber(),this->_exp2->evaluateNumber());
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", this->_id);
	}

  return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::GreaterThanNode::print()
{
  std::cout << "GreaterThanNode: " << std::endl;
  this->_left->print();
  std::cout << " > ";
  this->_right->print();
}

bool lp::GreaterThanNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				result = ( leftNumber > rightNumber);
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr > rightStr);
			}
			break;
			default:
				warning("Runtime error: incompatible types of parameters for ",
								"Greater than operator");
		}
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Greater than");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::GreaterOrEqualNode::print()
{
  std::cout << "GreaterOrEqualNode: " << std::endl;
  this->_left->print();
  std::cout << " >= ";
  this->_right->print();
}

bool lp::GreaterOrEqualNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				result = ( leftNumber >= rightNumber);
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr >= rightStr);
			}
			break;
			default:
				warning("Runtime error: incompatible types of parameters for ",
								"Greater or Equal than operator");
		}
	}else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Greater or Equal than");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::LessThanNode::print()
{
  std::cout << "LessThanNode: " << std::endl;
  this->_left->print();
  std::cout << " < ";
  this->_right->print();
}

bool lp::LessThanNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				result = ( leftNumber < rightNumber);
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr < rightStr);
			}
			break;
			default:
				warning("Runtime error: incompatible types of parameters for ",
								"Less than operator");
		}
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Less than");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::LessOrEqualNode::print()
{
  std::cout << "LessOrEqualNode: " << std::endl;
  this->_left->print();
  std::cout << " <= ";
  this->_right->print();
}

bool lp::LessOrEqualNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				result = ( leftNumber <= rightNumber);
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr <= rightStr);
			}
			break;
			default:
				warning("Runtime error: incompatible types of parameters for ",
								"Less or Equal than operator");
		}
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Less or equal than");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::EqualNode::print()
{
  std::cout << "EqualNode: " << std::endl;
  this->_left->print();
  std::cout << " == ";
  this->_right->print();
}

bool lp::EqualNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				// ERROR_BOUND to control the precision of real numbers
				result = ( std::abs(leftNumber - rightNumber) < ERROR_BOUND );
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr == rightStr);
			}
			break;
			case BOOL:
			{
				bool leftBoolean, rightBoolean;
				leftBoolean = this->_left->evaluateBool();
				rightBoolean = this->_right->evaluateBool();

				//
				result = (leftBoolean == rightBoolean);
			}
			break;
		  	default:
				warning("Runtime error: incompatible types of parameters for ",
								"Equal operator");
		}
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ",
						"Equal operator");
	}

	return result;
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::NotEqualNode::print()
{
  std::cout << "NotEqualNode: " << std::endl;
  this->_left->print();
  std::cout << " != ";
  this->_right->print();
}

bool lp::NotEqualNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		switch(this->_left->getType()){
			case NUMBER:
			{
				double leftNumber, rightNumber;
				leftNumber = this->_left->evaluateNumber();
				rightNumber = this->_right->evaluateNumber();

				// ERROR_BOUND to control the precision of real numbers
				result = ( std::abs(leftNumber - rightNumber) >= ERROR_BOUND );
			}
			break;
			case CADENA:
			{
				std::string leftStr,rightStr;
				leftStr = this->_left->evaluateCadena();
				rightStr = this->_right->evaluateCadena();
				result = (leftStr != rightStr);
			}
			break;
			case BOOL:
			{
				bool leftBoolean, rightBoolean;
				leftBoolean = this->_left->evaluateBool();
				rightBoolean = this->_right->evaluateBool();

				//
				result = (leftBoolean != rightBoolean);
			}
			break;
		  default:
				warning("Runtime error: incompatible types of parameters for ",
								"Not Equal operator");
		}
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "Not Equal operator");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AndNode::print()
{
  std::cout << "AndNode: " << std::endl;
  this->_left->print();
  std::cout << " && ";
  this->_right->print();
}

bool lp::AndNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		bool leftBool, rightBool;

		leftBool = this->_left->evaluateBool();
		rightBool = this->_right->evaluateBool();

		result = leftBool and rightBool;
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator And");
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::UnaryPlusStmt::print()
{
  std::cout << "UnaryPlusStmt: " << std::endl;
  std::cout << this->_id << std::endl;
}

void lp::UnaryPlusStmt::evaluate()
{
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);
	if (firstVar->getType() == NUMBER)
	{
	  	// Get the identifier in the table of symbols as NumericVariable
		lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

		// Assignment the value to the identifier in the table of symbols
		v->setValue(v->getValue()+1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::UnaryMinusStmt::print()
{
  std::cout << "UnaryMinusStmt: " << std::endl;
  std::cout << this->_id << std::endl;
}

void lp::UnaryMinusStmt::evaluate()
{
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);
	if (firstVar->getType() == NUMBER)
	{
	  	// Get the identifier in the table of symbols as NumericVariable
		lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

		// Assignment the value to the identifier in the table of symbols
		v->setValue(v->getValue()-1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::OrNode::print()
{
  std::cout << "OrNode: " << std::endl;
  this->_left->print();
  std::cout << " || ";
  this->_right->print();
}

bool lp::OrNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		bool leftBool, rightBool;

		leftBool = this->_left->evaluateBool();
		rightBool = this->_right->evaluateBool();

		result = leftBool or rightBool;
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Or");
	}

	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::NotNode::print()
{
  std::cout << "NotNode: " << std::endl;
  std::cout << " ! ";
  this->_exp->print();
}

bool lp::NotNode::evaluateBool()
{
	bool result = false;

	if (this->getType() == BOOL)
	{
		result = not this->_exp->evaluateBool();
	}
	else
	{
		warning("Runtime error: incompatible types of parameters for ", "operator Not");
	}

	return result;
}




///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AssignmentStmt::print()
{
  std::cout << "assignment_node: "  << std::endl;
  std::cout << this->_id << " = ";
  this->_exp->print();
  std::cout << std::endl;
}

void lp::AssignmentStmt::evaluate()
{
	/* Get the identifier in the table of symbols as Variable */
	/*
		a = 2;
		a = b = 2;

		a: firstVar
		b: secondVar
	*/
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		// Check the type of the expression of the asgn
		switch(this->_exp->getType())
		{
			case NUMBER:
			{
				double value;
				// evaluate the expression as NUMBER
			 	value = this->_exp->evaluateNumber();

				// Check the type of the first varible
				if (firstVar->getType() == NUMBER)
				{
				  	// Get the identifier in the table of symbols as NumericVariable
					lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable
					// with the type NUMBER and the value
					lp::NumericVariable *v = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,value);
					table.installSymbol(v);
				}
			}
			break;

			case BOOL:
			{
				bool value;
				// evaluate the expression as BOOL
			 	value = this->_exp->evaluateBool();

				if (firstVar->getType() == BOOL)
				{
				  	// Get the identifier in the table of symbols as LogicalVariable
					lp::LogicalVariable *v = (lp::LogicalVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				// The type of variable is not BOOL
				else
				{
					// Delete the variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable
					// with the type BOOL and the value
					lp::LogicalVariable *v = new lp::LogicalVariable(this->_id,
											VARIABLE,BOOL,value);
					table.installSymbol(v);
				}
			}
			break;
			case CADENA:
			{
				std::string value;

				value = this->_exp->evaluateCadena();

				if(firstVar->getType() == CADENA)
				{
				  	// Get the identifier in the table of symbols as LogicalVariable

					lp::alfanumericVariable * v = (lp::alfanumericVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value);
				}
				else
				{
					// Delete the variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable
					// with the type BOOL and the value

					lp::alfanumericVariable * v = new lp::alfanumericVariable(this->_id,VARIABLE,CADENA,value);
					table.installSymbol(v);
				}
			}break;

			default:
				warning("Runtime error: incompatible type of expression for ", "Assigment");
		}

	}

	//////////////////////////////////////////////
	// Allow multiple assigment -> a = b = c = 2;

	else // this->_asgn is not NULL
	{
		// IMPORTANT
		//  evaluate the assigment child
		this->_asgn->evaluate();


		/* Get the identifier of the previous asgn in the table of symbols as Variable */
		lp::Variable *secondVar = (lp::Variable *) table.getSymbol(this->_asgn->_id);

		// Get the type of the variable of the previous asgn
		switch(secondVar->getType())
		{
			case NUMBER:
			{
				/* Get the identifier of the previous asgn in the table of symbols as NumericVariable */
				lp::NumericVariable *secondVar = (lp::NumericVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == NUMBER)
				{
				/* Get the identifier of the first variable in the table of symbols as NumericVariable */
				lp::NumericVariable *firstVar = (lp::NumericVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
//					lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());

				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the first variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable
					// with the type NUMBER and the value of the previous variable
					lp::NumericVariable *firstVar = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,secondVar->getValue());
					table.installSymbol(firstVar);
				}
			}
			break;

			case BOOL:
			{
				/* Get the identifier of the previous asgn in the table of symbols as LogicalVariable */
				lp::LogicalVariable *secondVar = (lp::LogicalVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == BOOL)
				{
				/* Get the identifier of the first variable in the table of symbols as LogicalVariable */
				lp::LogicalVariable *firstVar = (lp::LogicalVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
//					lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());

				}
				// The type of variable is not BOOL
				else
				{
					// Delete the first variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable
					// with the type BOOL and the value of the previous variable
					lp::LogicalVariable *firstVar = new lp::LogicalVariable(this->_id,
											VARIABLE,BOOL,secondVar->getValue());
					table.installSymbol(firstVar);
				}
			}
			break;
			case CADENA:
			{
				/* Get the identifier of the previous asgn in the table of symbols as LogicalVariable */
				lp::alfanumericVariable * secondVar = (lp::alfanumericVariable *) table.getSymbol(this->_asgn->_id);
				// Check the type of the first variable
				if (firstVar->getType() == CADENA)
				{
					/* Get the identifier of the first variable in the table of symbols as LogicalVariable */
					lp::alfanumericVariable * firstVar = (lp::alfanumericVariable *) table.getSymbol(this->_id);
				  	// Get the identifier o f the in the table of symbols as NumericVariable
//					lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value of the second variable to the first variable
					firstVar->setValue(secondVar->getValue());

				}
				// The type of variable is not CADENA
				else
				{
					// Delete the first variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the first variable in the table of symbols as NumericVariable
					// with the type CADENA and the value of the previous variable

					lp::alfanumericVariable *firstVar = new lp::alfanumericVariable(this->_id,VARIABLE,CADENA,secondVar->getValue());

					table.installSymbol(firstVar);
				}
			}
			break;
			default:
				warning("Runtime error: incompatible type of expression for ", "Assigment");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AssignmentAdditionStmt::print()
{
  std::cout << "assignmentAddition_node: "  << std::endl;
  std::cout << this->_id << " +:= ";
  this->_exp->print();
  std::cout << std::endl;
}

void lp::AssignmentAdditionStmt::evaluate()
{
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		// Check the type of the expression of the asgn
		switch(this->_exp->getType())
		{
			case NUMBER:
			{
				double value;
				// evaluate the expression as NUMBER
			 	value = this->_exp->evaluateNumber();

				// Check the type of the first varible
				if (firstVar->getType() == NUMBER)
				{
				  	// Get the identifier in the table of symbols as NumericVariable
					lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(value+v->getValue());
				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable
					// with the type NUMBER and the value
					lp::NumericVariable *v = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,value);
					table.installSymbol(v);
				}
			}
			break;

			default:
				warning("Runtime error: incompatible type of expression for ", "Assigment");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AssignmentSubstractionStmt::print()
{
  std::cout << "assignmentSubstraction_node: "  << std::endl;
  std::cout << this->_id << " -:= ";
  this->_exp->print();
  std::cout << std::endl;
}

void lp::AssignmentSubstractionStmt::evaluate()
{
	lp::Variable *firstVar = (lp::Variable *) table.getSymbol(this->_id);

	// Check the expression
	if (this->_exp != NULL)
	{
		// Check the type of the expression of the asgn
		switch(this->_exp->getType())
		{
			case NUMBER:
			{
				double value;
				// evaluate the expression as NUMBER
			 	value = this->_exp->evaluateNumber();

				// Check the type of the first varible
				if (firstVar->getType() == NUMBER)
				{
				  	// Get the identifier in the table of symbols as NumericVariable
					lp::NumericVariable *v = (lp::NumericVariable *) table.getSymbol(this->_id);

					// Assignment the value to the identifier in the table of symbols
					v->setValue(v->getValue()-value);
				}
				// The type of variable is not NUMBER
				else
				{
					// Delete the variable from the table of symbols
					table.eraseSymbol(this->_id);

					// Insert the variable in the table of symbols as NumericVariable
					// with the type NUMBER and the value
					lp::NumericVariable *v = new lp::NumericVariable(this->_id,
											VARIABLE,NUMBER,value);
					table.installSymbol(v);
				}
			}
			break;

			default:
				warning("Runtime error: incompatible type of expression for ", "Assigment");
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::WriteStmt::print() {
  std::cout << "PrintStmt: "  << std::endl;
  std::cout << " print ";
  this->_exp->print();
  std::cout << std::endl;
}


void lp::WriteStmt::evaluate()
{
	std::cout << BIYELLOW;
	std::cout << "write: ";
	std::cout << RESET;

	switch(this->_exp->getType())
	{
		case NUMBER:
				std::cout << this->_exp->evaluateNumber() << std::endl;
				break;
		case BOOL:
			if (this->_exp->evaluateBool())
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			break;

		default:
			warning("Runtime error: incompatible type for ", "write");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::WriteStringStmt::print()
{
  std::cout << "WriteStmt: "  << std::endl;
  std::cout << " print ";
  std::cout << this->_id << std::endl;
  std::cout << std::endl;
}


void lp::WriteStringStmt::evaluate()
{
	std::cout << BIYELLOW;
	std::cout << "Write: ";
	std::cout << RESET;

	if(this->_exp == NULL)
	{
		lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

		if(var->getType() == CADENA){
			lp::alfanumericVariable * n = (lp::alfanumericVariable *) table.getSymbol(this->_id);
			std::cout << n->getValue() << std::endl;
		}
	}
	else
	{
		switch(this->_exp->getType())
		{
			case CADENA:
					std::cout << this->_exp->evaluateCadena() << std::endl;
					break;
			default:
				warning("Runtime error: incompatible type for ", "writeString");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::UntilStmt::print()
{
  std::cout << "UntilStmt: "  << std::endl;
  // Condition
  this->_cond->print();

  std::list<Statement *>::iterator stmtIter;
  // Consequent
  for (stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }

  std::cout << std::endl;
}


void lp::UntilStmt::evaluate()
{
  std::list<Statement *>::iterator stmtIter;
  // While the condition is false. the body is run
  int infinite = 0;
  do{
	for(stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
		(*stmtIter)->evaluate();
	infinite++;
  }while (this->_cond->evaluateBool() == false and infinite < 10000000);

  if (infinite >= 10000000)
		warning("Runtime error: infinite for loop condition ", "untilStmt");
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::ForStmt::print()
{
  std::cout << "ForStmt: "  << std::endl;
  // Condition
  std::cout << "For variable: "<<this->_id;
  std::cout << std::endl;
  std::cout << "From: ";
  this->_from->print();
  std::cout << std::endl;
  std::cout << "To: ";
  this->_to->print();
  std::cout << std::endl;
  std::cout << "Step: ";
  this->_from->print();
  std::cout << std::endl;
  std::cout << "Do: ";
  std::list<Statement *>::iterator stmtIter;
  // Consequent
  for (stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }
  std::cout << std::endl;
}


void lp::ForStmt::evaluate()
{

	double from, to, step;
	from = this->_from->evaluateNumber();
	to = this->_to->evaluateNumber();
	if(this->_step == NULL)
		step = 1;
	else
		step = this->_step->evaluateNumber();

	/* Get the identifier in the table of symbols as NumericVariable */
	table.eraseSymbol(this->_id);
	//lp::NumericVariable * n = (lp::NumericVariable *) table.getSymbol(this->_id);
	//n->setValue(from);

	lp::NumericVariable *n = new lp::NumericVariable(this->_id,	VARIABLE, NUMBER, from);
	table.installSymbol(n);

	if (step > 0 and from > to)
		warning("Runtime error: infinite for loop condition ", "forStmt");
	if (step < 0 and from < to)
		warning("Runtime error: infinite for loop condition ", "forStmt");		

	int i = from;
	while(i<=to)
	{
		std::list<Statement *>::iterator stmtIter;
		for(stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
			(*stmtIter)->evaluate();

		i = i + step;
		n->setValue(i);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ReadStmt::print()
{
  std::cout << "ReadStmt: "  << std::endl;
  std::cout << " read (" << this->_id << ")";
  std::cout << std::endl;
}


void lp::ReadStmt::evaluate()
{
	double value;
	std::cout << BIYELLOW;
	std::cout << "Insert a numeric value --> " ;
	std::cout << RESET;
	std::cin >> value;

	/* Get the identifier in the table of symbols as Variable */
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Check if the type of the variable is NUMBER
	if (var->getType() == NUMBER)
	{
		/* Get the identifier in the table of symbols as NumericVariable */
		lp::NumericVariable *n = (lp::NumericVariable *) table.getSymbol(this->_id);

		/* Assignment the read value to the identifier */
		n->setValue(value);
	}
	// The type of variable is not NUMBER
	else
	{
		// Delete $1 from the table of symbols as Variable
		table.eraseSymbol(this->_id);

			// Insert $1 in the table of symbols as NumericVariable
		// with the type NUMBER and the read value
		lp::NumericVariable *n = new lp::NumericVariable(this->_id,
									  VARIABLE,NUMBER,value);

		table.installSymbol(n);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ReadStringStmt::print()
{
  std::cout << "ReadStringStmt: "  << std::endl;
  std::cout << " read (" << this->_id << ")";
  std::cout << std::endl;
}


void lp::ReadStringStmt::evaluate()
{
	std::string value, read, temp;
	char aux;
	std::cout << BIYELLOW;
	std::cout << "Insert a string value --> " ;
	std::cout << RESET;
	aux = getchar();
	temp += aux;
	std::getline(std::cin, read);

	if ( aux != '\n')
		value = temp + read;
	else
		value = read;		

	/* Get the identifier in the table of symbols as Variable */
	lp::Variable *var = (lp::Variable *) table.getSymbol(this->_id);

	// Check if the type of the variable is CADENA
	if (var->getType() == CADENA)
	{
		/* Get the identifier in the table of symbols as NumericVariable */
		lp::alfanumericVariable * n = (lp::alfanumericVariable *) table.getSymbol(this->_id);

		/* Assignment the read value to the identifier */
		n->setValue(value);
	}
	// The type of variable is not CADENA
	else
	{
		// Delete $1 from the table of symbols as Variable
		table.eraseSymbol(this->_id);

		// Insert $1 in the table of symbols as NumericVariable
		// with the type CADENA and the read value
		lp::alfanumericVariable *n = new lp::alfanumericVariable(this->_id,VARIABLE,CADENA,value);

		table.installSymbol(n);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::EmptyStmt::print()
{
  std::cout << "EmptyStmt "  << std::endl;
}

void lp::EmptyStmt::evaluate()
{
  // Empty
}


void lp::ClearStmt::print()
{
	std::cout << "ClearStmt" << std::endl;
}

void lp::ClearStmt::evaluate()
{
	std::cout << CLEAR_SCREEN;
}

void lp::PlaceStmt::print()
{
	std::cout << "PlaceStmt" << std::endl;
	std::cout << "x -> " << this->_posX << std::endl;
  	std::cout << "y -> " << this->_posY << std::endl;
}

void lp::PlaceStmt::evaluate()
{

	PLACE((int)this->_posX, (int)this->_posY);
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::IfStmt::print()
{
  std::cout << "IfStmt: "  << std::endl;
  // Condition
  this->_cond->print();

  std::list<Statement *>::iterator stmtIter;
  // Consequent
  for (stmtIter = this->_stmt1->begin(); stmtIter != this->_stmt2->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }
  // The alternative is printed if exists
  if (this->_stmt2 != NULL)
	  for (stmtIter = this->_stmt2->begin(); stmtIter != this->_stmt2->end(); stmtIter++)
	     (*stmtIter)->print();


  std::cout << std::endl;
}

void lp::IfStmt::evaluate()
{

  	std::list<Statement *>::iterator stmtIter;

   // If the condition is true,
	if (this->_cond->evaluateBool() == true )
		for(stmtIter = this->_stmt1->begin(); stmtIter != this->_stmt1->end(); stmtIter++)
			(*stmtIter)->evaluate();
    // Otherwise, the alternative is run if exists
	else if (this->_stmt2 != NULL)
		for(stmtIter = this->_stmt2->begin(); stmtIter != this->_stmt2->end(); stmtIter++)
			(*stmtIter)->evaluate();
}


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::WhileStmt::print()
{
  std::cout << "WhileStmt: "  << std::endl;
  // Condition
  this->_cond->print();


  std::list<Statement *>::iterator stmtIter;
  // Consequent
  for (stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }

  std::cout << std::endl;
}


void lp::WhileStmt::evaluate()
{
  std::list<Statement *>::iterator stmtIter;
  // While the condition is true. the body is run
  while (this->_cond->evaluateBool() == true)
  {
	for(stmtIter = this->_stmt->begin(); stmtIter != this->_stmt->end(); stmtIter++)
		(*stmtIter)->evaluate();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// NEW in example 17

void lp::BlockStmt::print()
{
  std::list<Statement *>::iterator stmtIter;

  std::cout << "BlockStmt: "  << std::endl;

  for (stmtIter = this->_stmts->begin(); stmtIter != this->_stmts->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }
}


void lp::BlockStmt::evaluate()
{
  std::list<Statement *>::iterator stmtIter;

  for (stmtIter = this->_stmts->begin(); stmtIter != this->_stmts->end(); stmtIter++)
  {
    (*stmtIter)->evaluate();
  }
}





///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::AST::print()
{
  std::list<Statement *>::iterator stmtIter;

  for (stmtIter = stmts->begin(); stmtIter != stmts->end(); stmtIter++)
  {
     (*stmtIter)->print();
  }
}



void lp::AST::evaluate()
{
  std::list<Statement *>::iterator stmtIter;

  for (stmtIter = stmts->begin(); stmtIter != stmts->end(); stmtIter++)
  {
    (*stmtIter)->evaluate();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void lp::ConcatenationNode::print()
{
  std::cout << "Concatenation: " << std::endl;
  this->_left->print();
  std::cout << " || ";
  this->_right->print();
}

std::string lp::ConcatenationNode::evaluateCadena()
{
	std::string result;

	// Ckeck the types of the expressions
	if (this->getType() == CADENA)
	{
		std::string left, right;

		left = this->_left->evaluateCadena();
		right = this->_right->evaluateCadena();
		result = left + right;
	}
	else
	{
		warning("Runtime error: the expressions are not string for", "Concatenation");
	}

  return result;
}
