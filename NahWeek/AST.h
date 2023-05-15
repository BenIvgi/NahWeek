#pragma once
#ifndef NAH_AST_H
#define NAH_AST_H
#include "list.h"

//-----------------------------------------------------------------------------
// AST
// -----------------------------------------------------------------------------
//
// General : sturct resembles an abstract syntax tree (AST).
//
// Properties :
// eType - determines the ast compound type.
// childern - a list of a nodes children.
// value - the value compund of a node.
// nVal - the literal value of a node.
// datatype - the node's datatype.
//
//-----------------------------------------------------------------------------
typedef struct AST_STRUCT
{
	enum
	{
		AST_COMPOUND,
		AST_FUNCTION,
		AST_VAR,
		AST_CALL,
		AST_ASSIGNMENT,
		AST_NOOP,
		AST_EXPR
	}eType;

	list_N* childern;
	char* name;
	struct AST_STRUCT* value;
	char* nVal;
	enum
	{
		DT_INT,
		DT_STRING,
		DT_BOOL,
		DT_PTR,	
		DT_UNKNOWN,
		DT_VOID
	}datatype;

}AST_N;

//-----------------------------------------------------------------------------
// Initialize AST
//-----------------------------------------------------------------------------
//
// General : allocate memory for ast and init it's properties.
//
// Parameters :
// eType - determines the ast's type.
//
// Return Value : a pointer to an AST struct.
//
//-----------------------------------------------------------------------------
AST_N* InitAst(int eType);

//-----------------------------------------------------------------------------
// Print AST
//-----------------------------------------------------------------------------
//
// General : visually display the tree.
//
// Parameters :
// ast - the AST to print
// depth - determines the number of indentations.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void PrintAST(AST_N* ast, int depth);

//-----------------------------------------------------------------------------
// Print AST Child
//-----------------------------------------------------------------------------
//
// General : visually display an AST's child by type.
//
// Parameters :
// child - the AST child to print
// depth - determines the number of indentations.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void PrintChild(AST_N* child, int depth);

//-----------------------------------------------------------------------------
// Return Type's Enum Value
//-----------------------------------------------------------------------------
//
// General : check what is the type, and return the according enum label.
//
// Parameters :
// ast - the AST to print
// depth - determines the number of indentations.
//
// Return Value : the enum label.
//
//-----------------------------------------------------------------------------
int DatatypeToInt(char* name);

//-----------------------------------------------------------------------------
// Print Indentation
//-----------------------------------------------------------------------------
//
// General : visually display the difference between layers.
//
// Parameters :
// depth - determines the number of indentations.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void PrintIndentation(int depth);

//-----------------------------------------------------------------------------
// Print Unsolved expression.
//-----------------------------------------------------------------------------
//
// General : display the expression that the variable stores.
//
// Parameters :
// child - the AST child to print.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void PrintExpr(AST_N* child);
#endif
