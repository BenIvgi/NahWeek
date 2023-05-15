#pragma once
#ifndef NAH_TOKEN_H
#define NAH_TOKEN_H
//-----------------------------------------------------------------------------
// Token
// -----------------------------------------------------------------------------
//
// General : resembles a token of information.
//
// Properties :
// pcVal - a pointer to a char.
// eType - Type Enum
// 
// 0.TOKEN_ID char  7.TOKEN_COMMA ,		14. TOKEN_PLUS +	21. TOKEN_E
// 1.TOKEN_EQUALS = 8.TOKEN_LESST <		15. TOKEN_MINUS -	22. TOKEN_NE
// 2.TOKEN_OPARA (  9.TOKEN_GREATT >	16. TOKEN_MUL *		23. TOKEN_EL
// 3.TOKEN_CPARA )  10.TOKEN_RARROW ->	17. TOKEN_DIV /		24. TOKEN_EG
// 4.TOKEN_OBRAC {  11.TOKEN_LARROW <-	18. TOKEN_NOT !		25. TOKEN_DOT
// 5.TOKEN_CBRAC }  12.TOKEN_INT number	19. TOKEN_AND &&	26.TOKEN_EOF
// 6.TOKEN_COLON :  13.TOKEN_SEMIC ;	20. TOKEN_OR ||		
//					
// 
//
//-----------------------------------------------------------------------------
typedef struct TOKEN_STRUCT
{
	char* pcVal;
	enum
	{
		TOKEN_ID,
		TOKEN_EQUALS,
		TOKEN_OPARA,
		TOKEN_CPARA,
		TOKEN_OBRAC,
		TOKEN_CBRAC,
		TOKEN_COLON,
		TOKEN_COMMA,
		TOKEN_LESST,
		TOKEN_GREATT,
		TOKEN_RARROW,
		TOKEN_LARROW,
		TOKEN_INT,
		TOKEN_SEMIC,
		TOKEN_PLUS,
		TOKEN_MINUS,
		TOKEN_MUL,
		TOKEN_DIV,
		TOKEN_NOT,
		TOKEN_AND,
		TOKEN_OR,
		TOKEN_E,
		TOKEN_NE,
		TOKEN_EL,
		TOKEN_EG,
		TOKEN_DOT,
		TOKEN_EOF,
	} eType;
}token_N;

//-----------------------------------------------------------------------------
// Initialize Token
//-----------------------------------------------------------------------------
//
// General : function allocates memory for token,
// gets val and type into struct properties.
//
// Parameters :
// pcVal - a pointer to a char.
// nType - an integer.
//
// Return Value : a pointer to a token.
//
//-----------------------------------------------------------------------------
token_N* InitToken(char* pcVal, int nType);

//-----------------------------------------------------------------------------
// Return Type String Literal
//-----------------------------------------------------------------------------
//
// General : function returns the name of an enum category in string format.
//
// Parameters :
// eType - enum value.
//
// Return Value : String name of enum value.
//
//-----------------------------------------------------------------------------
const char* TokenTypeToStr(int eType);

//-----------------------------------------------------------------------------
// Return Token Description
//-----------------------------------------------------------------------------
//
// General : function retruns token value and type in string format.
//
// Parameters :
// token - token struct.
//
// Return Value : String description of token.
//
//-----------------------------------------------------------------------------
char* TokenToStr(token_N* token);

#endif