#pragma once
#ifndef NAH_LEXER_H
#define NAH_LEXER_H
#include <stdio.h>
#include "token.h"
#include "io.h"

//-----------------------------------------------------------------------------
// Lexer
// -----------------------------------------------------------------------------
//
// General : resembles a lexer.
//
// Properties :
// pcSrc - source, current word.
// cChar - current char.
// nIndex - index in word.
//
//-----------------------------------------------------------------------------
typedef struct LEXER_STRUCT
{
	char* pcSrc;
	size_t nSrcSize;
	char cChar;
	unsigned int nIndex;

}lexer_N;

//-----------------------------------------------------------------------------
// Initialize Lexer
//-----------------------------------------------------------------------------
//
// General : function initializes lexer values. 
//
// Parameters :
// pcSrc - Source, a pointer to a char.
//
// Return Value : a pointer to a lexer.
//
//-----------------------------------------------------------------------------
lexer_N* InitLexer(char* pcSrc);

//-----------------------------------------------------------------------------
// Advance Lexer
//-----------------------------------------------------------------------------
//
// General : function advances lexer index to next word. 
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void LexerAdvance(lexer_N* lexer);

//-----------------------------------------------------------------------------
// Advance Current Lexer with Token
//-----------------------------------------------------------------------------
//
// General : function advances lexer, and creates token from char.
//
// Parameters :
// lexer - a pointer to a lexer.
// nType - an integer symbolizing the type.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* LexerAdvanceCurrent(lexer_N* lexer, int nType);

//-----------------------------------------------------------------------------
// Advance Lexer
//-----------------------------------------------------------------------------
//
// General : function advances lexer index but returns current token. 
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* LexerNextToken(lexer_N* lexer);

//-----------------------------------------------------------------------------
// Skip Spaces
//-----------------------------------------------------------------------------
//
// General : function ignores space chars. 
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void LexerSkipSpace(lexer_N* lexer);

//-----------------------------------------------------------------------------
// Parse Source Word
//-----------------------------------------------------------------------------
//
// General : function parses chars into token. 
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* LexerParseId(lexer_N* lexer);

//-----------------------------------------------------------------------------
// Advance Current Lexer with Token
//-----------------------------------------------------------------------------
//
// General : function advances lexer, and creates token from chars.
//
// Parameters :
// lexer - a pointer to a lexer.
// token - a pointer to a token.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* LexerAdvanceWith(lexer_N* lexer, token_N* token);

//-----------------------------------------------------------------------------
// Peek Into Char
//-----------------------------------------------------------------------------
//
// General : check next char in source.
//
// Parameters :
// lexer - a pointer to a lexer.
// offset - an integer.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
char LexerPeek(lexer_N* lexer, int offSet);

//-----------------------------------------------------------------------------
// Parse Source Number
//-----------------------------------------------------------------------------
//
// General : function parses chars into token. 
//
// Parameters :
// lexer - a pointer to a lexer.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* LexerParseNumber(lexer_N* lexer);

#endif
