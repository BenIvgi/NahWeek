#define _CRT_SECURE_NO_WARNINGS
#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MSG_LEN 230

token_N* InitToken(char* pcVal, int nType)
{
	// Declare Variables and Allocate Memory.
	token_N* token = malloc(sizeof(struct TOKEN_STRUCT));
	// Check memory allocation.
	if (token == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}

	// Insert values given into token.
	token->pcVal = pcVal;
	token->eType = nType;

	// Return token.
	return token;
}

const char* TokenTypeToStr(int eType)
{
	switch (eType)
	{
	case TOKEN_ID: return "TOKEN_ID    ";
	case TOKEN_EQUALS: return "TOKEN_EQUALS";
	case TOKEN_OPARA: return "TOKEN_OPARA ";
	case TOKEN_CPARA: return "TOKEN_CPARA ";
	case TOKEN_OBRAC: return "TOKEN_OBRAC ";
	case TOKEN_CBRAC: return "TOKEN_CBRAC ";
	case TOKEN_COLON: return "TOKEN_COLON ";
	case TOKEN_COMMA: return "TOKEN_COMMA ";
	case TOKEN_LESST: return "TOKEN_LESST ";
	case TOKEN_GREATT: return "TOKEN_GREATT";
	case TOKEN_RARROW: return "TOKEN_RARROW";
	case TOKEN_LARROW: return "TOKEN_LARROW";
	case TOKEN_INT: return "TOKEN_INT   ";
	case TOKEN_SEMIC: return "TOKEN_SEMIC";
	case TOKEN_PLUS: return "TOKEN_PLUS ";
	case TOKEN_MINUS: return "TOKEN_MINUS ";
	case TOKEN_MUL: return "TOKEN_MUL ";
	case TOKEN_DIV: return "TOKEN_DIV ";
	case TOKEN_NOT: return "TOKEN_NOT ";
	case TOKEN_AND: return "TOKEN_AND ";
	case TOKEN_OR: return "TOKEN_OR ";
	case TOKEN_EOF: return "TOKEN_EOF   ";
	}
	return "TOKEN_UNKNOWN";
}

char* TokenToStr(token_N* token)
{
	// Allocate Space for msg.
	char* arrcType = calloc(sizeof(char), MSG_LEN);
	if (arrcType == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}
	// Add type, name, value, value
	strcpy(arrcType, "[Type: ");
	strcat(arrcType, TokenTypeToStr(token->eType));
	strcat(arrcType, "; Value: \"");
	strcat(arrcType, token->pcVal);
	strcat(arrcType, "\"]\n\0");
	return arrcType;
}