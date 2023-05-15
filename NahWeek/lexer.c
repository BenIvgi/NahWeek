#define _CRT_SECURE_NO_WARNINGS
#include "lexer.h"
#include <string.h>
#include <stdlib.h>

lexer_N* InitLexer(char* pcSrc)
{
	// Allocate memory for lexer.
	lexer_N* lexer = malloc(sizeof(lexer_N));
	// Check memory allocation.
	if (lexer == 0)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}

	// Insert values.
	lexer->pcSrc = pcSrc;
	lexer->nSrcSize = strlen(pcSrc);
	lexer->nIndex = 0;
	lexer->cChar = pcSrc[lexer->nIndex];

	return lexer;
}

void LexerAdvance(lexer_N* lexer)
{
	// As long as its not EOF, advance lexer by 1.
	if (lexer->nIndex < lexer->nSrcSize && lexer->cChar != '\0')
	{
		lexer->nIndex += 1;
		lexer->cChar = lexer->pcSrc[lexer->nIndex];
	}
}

token_N* LexerAdvanceCurrent(lexer_N* lexer, int nType)
{
	// Allocate memory for single letter string.
	char* pcValue = (char*)malloc(sizeof(char)*2);
	// Check memory allocation.
	if (pcValue == 0)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}

	// Insert values.
	pcValue[0] = lexer->cChar;
	pcValue[1] = '\0';

	// Create token for letter.
	token_N* token = InitToken(pcValue, nType);

	// Move to next value.
	LexerAdvance(lexer);
	return token;
}

void LexerSkipSpace(lexer_N* lexer)
{
	// As long as it's one of the empty chars, skip.
	while (lexer->cChar == ' ' || lexer->cChar == '\n' ||
		   lexer->cChar == '\t' || lexer->cChar == '\r')
	{
		LexerAdvance(lexer);
	}
}

token_N* LexerNextToken(lexer_N* lexer)
{
	// As long as it's not EOF.
	while (lexer->cChar != '\0')
	{

		// Skip spaces.
		LexerSkipSpace(lexer);

		// Erase comments.
		while (lexer->cChar == '#')
		{
			while (lexer->cChar != '\n')
			{
				LexerAdvance(lexer);
			}
			LexerSkipSpace(lexer);
		}

		// Case 1 - is number.
		if (lexer->cChar <= '9' && lexer->cChar >= '0')
			return LexerParseNumber(lexer);

		// Case 2 - is char (part of word)
		if (lexer->cChar <= 'z' && lexer->cChar >= 'a' ||
			lexer->cChar <= 'Z' && lexer->cChar >= 'A' ||
			lexer->cChar == '\"' || lexer->cChar == '\'')
			return LexerParseId(lexer);

		// Case 3 - is symbol.
		switch (lexer->cChar)
		{
		case '-':
		{
			if (LexerPeek(lexer, 1) == '>')
			{
				
				return LexerAdvanceWith(lexer, InitToken("->", TOKEN_RARROW));
			}
			else
			{
				return LexerAdvanceCurrent(lexer, TOKEN_MINUS);
			}
			break;
		}
		case '&':
		{
			if (LexerPeek(lexer, 1) == '&')
			{

				return LexerAdvanceWith(lexer, InitToken("&&", TOKEN_AND));
			}
			else
			{
				printf("NahWeek! Error 3 - [NahWeek : Lexer] Unexpected character '%c'.\n", lexer->cChar);
				exit(3);
				break;
			}
			break;
		}
		case '|':
		{
			if (LexerPeek(lexer, 1) == '|')
			{

				return LexerAdvanceWith(lexer, InitToken("||", TOKEN_OR));
			}
			else
			{
				printf("NahWeek! Error 3 - [NahWeek : Lexer] Unexpected character '%c'.\n", lexer->cChar);
				exit(3);
				break;
			}
			break;
		}
		case '=':
		{
			if (LexerPeek(lexer, 1) == '=')
			{

				return LexerAdvanceWith(lexer, InitToken("==", TOKEN_E));
			}
			else
			{
				return LexerAdvanceCurrent(lexer, TOKEN_EQUALS);
			}
			break;

		}
		case '!':
		{
			if (LexerPeek(lexer, 1) == '=')
			{

				return LexerAdvanceWith(lexer, InitToken("!=", TOKEN_NE));
			}
			else
			{
				return LexerAdvanceCurrent(lexer, TOKEN_NOT);
			}
			break;

		}
		case '<':
		{
			if (LexerPeek(lexer, 1) == '=')
			{

				return LexerAdvanceWith(lexer, InitToken("<=", TOKEN_EL));
			}
			else
			{
				return LexerAdvanceCurrent(lexer, TOKEN_LESST);
			}
			break;
		}
		case '>':
		{
			if (LexerPeek(lexer, 1) == '=')
			{

				return LexerAdvanceWith(lexer, InitToken(">=", TOKEN_EG));
			}
			else
			{
				return LexerAdvanceCurrent(lexer, TOKEN_GREATT);
			}
			break;

		}
		case '*': return LexerAdvanceCurrent(lexer, TOKEN_MUL);
		case '/': return LexerAdvanceCurrent(lexer, TOKEN_DIV);
		case '+': return LexerAdvanceCurrent(lexer, TOKEN_PLUS);
		case '(': return LexerAdvanceCurrent(lexer, TOKEN_OPARA);
		case ')': return LexerAdvanceCurrent(lexer, TOKEN_CPARA);
		case '{': return LexerAdvanceCurrent(lexer, TOKEN_OBRAC);
		case '}': return LexerAdvanceCurrent(lexer, TOKEN_CBRAC);
		case ':': return LexerAdvanceCurrent(lexer, TOKEN_COLON);
		case ',': return LexerAdvanceCurrent(lexer, TOKEN_COMMA);
		case ';': return LexerAdvanceCurrent(lexer, TOKEN_SEMIC);
		case '.': return LexerAdvanceCurrent(lexer, TOKEN_DOT);
		case '\0': return LexerAdvanceCurrent(lexer, TOKEN_EOF);

		// Case 4 - char unknown.
		default:
		{
			printf("NahWeek! Error 3 - [NahWeek : Lexer] Unexpected character '%c'.\n", lexer->cChar);
			exit(3);
			break;
		}
		}
	}
	// Case 5 - EOF.
	return InitToken(0, TOKEN_EOF);
}

token_N* LexerParseId(lexer_N* lexer)
{
	// Allocate memory for word.
	char* pcValue = (char*)malloc(sizeof(char));
	// Check memory allocation.
	if (pcValue == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}

	pcValue[0] = '\0';

	// While char is alphabethical.
	while ((lexer->cChar <= 'z' && lexer->cChar >= 'a') ||
		(lexer->cChar <= 'Z' && lexer->cChar >= 'A') ||
		(lexer->cChar <= '9' && lexer->cChar >= '0'))
	{
		// Append word by 2 (1 for new char, 1 for null Terminator).
		pcValue = realloc(pcValue, sizeof(char) * (strlen(pcValue) + 2));
		// Check memory allocation.
		if (pcValue == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Allocate space for single letter string.
		char* pcChar = (char*)malloc(sizeof(char)*2);
		// Check memory allocation.
		if (pcChar == 0)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Insert value into letter string.
		pcChar[0] = lexer->cChar;
		pcChar[1] = '\0';
		// Add char to word.
		strcat(pcValue, pcChar);
		// Move to next char.
		LexerAdvance(lexer);
	}

	if (lexer->cChar == '\"' || lexer->cChar == '\'')
	{
		char quote = lexer->cChar;
		do
		{
			pcValue = realloc(pcValue, sizeof(char) * (strlen(pcValue) + 2));
			// Check memory allocation.
			if (pcValue == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
				exit(1);
			}

			// Allocate space for single letter string.
			char* pcChar = (char*)malloc(sizeof(char) * 2);
			// Check memory allocation.
			if (pcChar == 0)
			{
				printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
				exit(1);
			}

			// Insert value into letter string.
			pcChar[0] = lexer->cChar;
			pcChar[1] = '\0';
			// Add char to word.
			strcat(pcValue, pcChar);
			// Move to next char.
			LexerAdvance(lexer);
		} while (lexer->cChar != quote);
		pcValue = realloc(pcValue, sizeof(char) * (strlen(pcValue) + 2));
		// Check memory allocation.
		if (pcValue == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Allocate space for single letter string.
		char* pcChar = (char*)malloc(sizeof(char) * 2);
		// Check memory allocation.
		if (pcChar == 0)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Insert value into letter string.
		pcChar[0] = lexer->cChar;
		pcChar[1] = '\0';
		// Add char to word.
		strcat(pcValue, pcChar);
		// Move to next char.
		LexerAdvance(lexer);
	}
	// Return the word as token.
	return InitToken(pcValue, TOKEN_ID);
}

token_N* LexerParseNumber(lexer_N* lexer)
{
	// Allocate memory for number
	char* pcValue = calloc(1, sizeof(char));
	// Check memory allocation.
	if (pcValue == 0)
	{
		printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
		exit(1);
	}
	
	// As long as char is digit.
	while (lexer->cChar <= '9' && lexer->cChar >= '0' || lexer->cChar == '_')
	{
		if (lexer->cChar == '_')
		{
			LexerAdvance(lexer);
			continue;
		}
		// Append numbeer by 2 (1 for new char, 1 for null Terminator).
		pcValue = realloc(pcValue, sizeof(char) * (strlen(pcValue) + 2));
		// Check memory allocation.
		if (pcValue == 0)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Allocate memory for single digit string.
		char* pcChar = (char*)malloc(sizeof(char)*2);
		// Check memory allocation.
		if (pcChar == 0)
		{
			printf("NahWeek! Error 1 - [NahWeek : Lexer] - memory allocation failed.\n");
			exit(1);
		}

		// Insert values to digit string.
		pcChar[0] = lexer->cChar;
		pcChar[1] = '\0';

		// Add digit to number.
		strcat(pcValue, pcChar);
		// Move to next digit.
		LexerAdvance(lexer);
	}
	// Return number as token.
	return InitToken(pcValue, TOKEN_INT);
}

token_N* LexerAdvanceWith(lexer_N* lexer, token_N* token)
{
	// Advance twice, return token given.
	LexerAdvance(lexer);
	LexerAdvance(lexer);
	return token;
}

char LexerPeek(lexer_N* lexer, int offSet)
{
	// If in file range, return next char, else return last char.
	return lexer->pcSrc[(lexer->nIndex + offSet) < lexer->nSrcSize ?
		lexer->nIndex + offSet : lexer->nSrcSize];
}
