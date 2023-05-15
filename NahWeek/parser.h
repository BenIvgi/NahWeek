#pragma once
#ifndef NAH_PARSER_H
#define NAH_PARSER_H
#include "AST.h"
#include "lexer.h"
#include "stack.h"

//-----------------------------------------------------------------------------
// Parser
// -------------------
//
// General : structure resembles a parser.
//
// Properties :
// lexer - the token generator.
// token - the current token being parsed.
//
//-----------------------------------------------------------------------------
typedef struct PARSER_STRUCT
{
	token_N* token;
	lexer_N* lexer;
}parser_N;


//-----------------------------------------------------------------------------
// Initialize Parser
//-----------------------------------------------------------------------------
//
// General : function allocates memory for parser,
// puts lexer in property and first token.
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : a pointer to a parser.
//
//-----------------------------------------------------------------------------
parser_N* InitParser(lexer_N* lexer);

//-----------------------------------------------------------------------------
// Eat a Token.
//-----------------------------------------------------------------------------
//
// General : function "eats" a token and moves to the next token.
//
// Parameters :
// parser - a pointer to a parser.
// eType - the token type expected to be eaten.
//
// Return Value : a pointer to the token that was eaten.
//
//-----------------------------------------------------------------------------
token_N* ParserEat(parser_N* parser, int eType);

//-----------------------------------------------------------------------------
// Parse Compound.
//-----------------------------------------------------------------------------
//
// General : function is used as the main parsing function.
// pushes all sub components to the AST's list.
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseCompound(parser_N* parser);

//-----------------------------------------------------------------------------
// Parse Block.
//-----------------------------------------------------------------------------
//
// General : parse a block of code.
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseBlock(parser_N* parser);

//-----------------------------------------------------------------------------
// Parse (duh..)
//-----------------------------------------------------------------------------
//
// General : parsing shell function.
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParse(parser_N* parser);

//-----------------------------------------------------------------------------
// Parse an Expression.
//-----------------------------------------------------------------------------
//
// General : function is used as a switch to direct an ast to the right parsing.
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseSwitch(parser_N* parser);

//-----------------------------------------------------------------------------
// Parse Compound.
//-----------------------------------------------------------------------------
//
// General : parse a word (leads to many sub - cases).
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseID(parser_N* parser);

//-----------------------------------------------------------------------------
// Parse Compound.
//-----------------------------------------------------------------------------
//
// General : parse a list of values.
//
// Parameters :
// parser - a pointer to a parser.
// eType - indicates if this is a list of params or args.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseList(parser_N* parser, int eType);

//-----------------------------------------------------------------------------
// Parse Expression
//-----------------------------------------------------------------------------
//
// General : parse an expression.
//
// Parameters :
// s - a stack containing the expr items.
//
// Return Value : a pointer to an AST.
//
//-----------------------------------------------------------------------------
AST_N* ParserParseExpr(stack_N* s);

//-----------------------------------------------------------------------------
// Format Expression
//-----------------------------------------------------------------------------
//
// General : add parantheses to expression.
//
// Parameters :
// s - a stack containing the expr items.
//
// Return Value : a pointer to a stack.
//
//-----------------------------------------------------------------------------
stack_N* FormatExpr(stack_N* s);

//-----------------------------------------------------------------------------
// Create The Expression Stack.
//-----------------------------------------------------------------------------
//
// General : eat tokens from the parser to create an expr stack.
//
// Parameters :
// parser - a pointer to a parser.
//
// Return Value : stack containing the expr items.
//
//-----------------------------------------------------------------------------
stack_N* ParserToStack(parser_N* parser);


//-----------------------------------------------------------------------------
// Evaluate an Expression.
//-----------------------------------------------------------------------------
//
// General : preform constant folding and string concating.
//
// Parameters :
// ast - a pointer to an ast.
// eType - the expression's type.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void Evaluate(AST_N* ast, int eType);
#endif