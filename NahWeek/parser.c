#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include <stdlib.h>
#include <string.h>

list_N* vartable;

parser_N* InitParser(lexer_N* lexer)
{
	// Allocate memory for parser.
	parser_N* parser = malloc(sizeof(struct PARSER_STRUCT));
	if (parser == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
		exit(1);
	}
	// init fields.
	parser->lexer = lexer;
	parser->token = LexerNextToken(lexer);

	return parser;
}

token_N* ParserEat(parser_N* parser, int eType)
{
	// if other token faced, throw error.
	if (parser->token->eType != eType)
	{
		printf("NahWeek! Error 4 - [NahWeek : Parser] Unexpected token '%s', '%s' expected.\n",
			TokenToStr(parser->token), TokenTypeToStr(eType));
		exit(4);
	}
	// return former token and andvance lexer.
	token_N* token = parser->token;
	parser->token = LexerNextToken(parser->lexer);
	return token;
}

AST_N* ParserParseExpr(stack_N* s)
{
	// Init ast
	AST_N* ast = InitAst(AST_EXPR);

	// Keep parsing as long as the expr is not over.
	while(!stack_empty(*s))
	{
		// Pop one item from the expr.
		token_N* token = stack_pop(s);
		switch (token->eType)
		{
		case TOKEN_OPARA:
		{
			// if it's a ( -> go to right child with this function.
			ListPush(ast->childern, NULL);
			ListPush(ast->childern, ParserParseExpr(s));
			ast->datatype = DT_UNKNOWN;
			break;
		}
		case TOKEN_PLUS:
		case TOKEN_MINUS:
		case TOKEN_MUL:
		case TOKEN_DIV:
		{
			// if math op -> put op in node, go to left child. 
			ast->name = token->pcVal;
			int op = token->eType;
			ast->childern->ppvItems[0] = ParserParseExpr(s);
			// if both values are literals -> evaluate.
			if (((AST_N*)ast->childern->ppvItems[0])->nVal && ((AST_N*)ast->childern->ppvItems[1])->nVal)
			{
				Evaluate(ast, op);
			}
			else
			{
				ast->datatype = DT_INT;
			}
			break;
		}
		case TOKEN_OR:
		case TOKEN_AND:
		case TOKEN_E:
		case TOKEN_NE:
		case TOKEN_EG:
		case TOKEN_EL:
		case TOKEN_LESST:
		case TOKEN_GREATT:
		{
			// if boolean op, do same as math op, but declare bool type.
			ast->name = token->pcVal;
			int op = token->eType;
			ast->childern->ppvItems[0] = ParserParseExpr(s);
			if (((AST_N*)ast->childern->ppvItems[0])->nVal && ((AST_N*)ast->childern->ppvItems[1])->nVal)
			{
				Evaluate(ast, op);
			}
			else
			{
				ast->datatype = DT_BOOL;
			}
			break;
		}
		case TOKEN_ID:
		{
			// if string literal, place in node.
			if (token->pcVal[0] == '\"' || token->pcVal[0] == '\'')
			{
				ast->nVal = token->pcVal;
				ast->datatype = DT_STRING;
			}
			else
			{
				// if True / False, replace with 1/0 and declare bool type.
				if (!strcmp(token->pcVal, "True") || !strcmp(token->pcVal, "False"))
				{
					ast->nVal = !strcmp(token->pcVal, "True") ? "1" : "0";
					ast->datatype = DT_BOOL;
				}
				else
				{
					if (!stack_empty(*s))
					{
						// if after word comes ( -> function call.
						if (((token_N*)s->top->info)->eType == TOKEN_OPARA)
						{
							char* func = calloc(1, sizeof(char));
							if (func == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
								exit(1);
							}
							ast->name = calloc(strlen(token->pcVal), sizeof(char));
							int paracnt = 0;
							if (ast->name == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
								exit(1);
							}
							strcpy(ast->name, token->pcVal);
							while (!(token->eType == TOKEN_CPARA && paracnt == 0))
							{
								token = stack_pop(s);
								if (token->eType == TOKEN_OPARA) paracnt++;
								if (token->eType == TOKEN_CPARA) paracnt--;
								func = realloc(func, (strlen(func) + strlen(token->pcVal) + 1) * sizeof(char));
								if (func == NULL)
								{
									printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
									exit(1);
								}
								strcat(func, token->pcVal);
							}
							parser_N* lexer = malloc(sizeof(lexer_N*));
							if (lexer == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
								exit(1);
							}
							lexer = InitLexer(func);
							parser_N* parser = malloc(sizeof(parser_N*));
							if (parser == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
								exit(1);
							}
							parser = InitParser(lexer);
							// parse params.
							ast->value = ParserParseList(parser, AST_CALL);
							ast->datatype = FindVarDT(ast->name);
						}
						else
						{
							ast->name = token->pcVal;
							ast->datatype = FindVarDT(ast->name);
						}
					}
					else
					{
						// if variable -> place in node.
						ast->name = token->pcVal;
						ast->datatype = FindVarDT(ast->name);
						if (ast->datatype == -1)
						{
							printf("NahWeek! Error 6 - [NahWeek : Parser] - Variable %s was not declared.\n", ast->name);
							exit(6);
						}
					}
				}
			}
			return ast;
		}
		case TOKEN_INT:
		{
			// if number -> place in node.
			ast->nVal = token->pcVal;
			ast->datatype = DT_INT;
			return ast;
		}
		case TOKEN_CPARA:
		{
			// if ) -> fold to parent. if expr is surronded by ((expr)) -> give up on the outer ones.
			if (!((AST_N*)ast->childern->ppvItems[0]) && !((AST_N*)ast->childern->ppvItems[1])->value)
			{
				ast = ((AST_N*)ast->childern->ppvItems[1]);
			}
			return ast;
		}
		default:
		{
			printf("NahWeek! Error 4 - [NahWeek : Parser] Unexpected token '%s'\n", TokenToStr(token));
			exit(4);
		}
		}
	}
	return ast;
}

int FindVarDT(char* var)
{
	// search in vartable.
	for (int i = 0; i < vartable->sSize; i++)
	{
		if (!strcmp(var, ((AST_N*)vartable->ppvItems[i])->name))
		{
			return ((AST_N*)vartable->ppvItems[i])->datatype;
		}
	}
	return -1;
}

void Evaluate(AST_N* ast, int eType)
{
	// set a, b to be operands -> solve equation and place in ast nVal.
	AST_N* a = ((AST_N*)ast->childern->ppvItems[1]);
	AST_N* b = ((AST_N*)ast->childern->ppvItems[0]);
	switch (eType)
	{
	case TOKEN_PLUS:
	{
		// concat strings.
		if (a->datatype == DT_STRING || b->datatype == DT_STRING)
		{
			char let[2] = { 0,0 };
			ast->nVal = calloc(strlen(a->nVal) + strlen(b->nVal) + 1, sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			if (a->nVal[0] != '\"' && a->nVal[0] != '\'')
			{
				let[0] = b->nVal[0];
				strcat(ast->nVal, let);
			}
			strcat(ast->nVal, a->nVal);
			ast->nVal[strlen(ast->nVal) - 1] = '\0';
			if (b->nVal[0] == '\"' || b->nVal[0] == '\'')
			{
				strcat(ast->nVal, ++(b->nVal));
			}
			else
			{
				strcat(ast->nVal, b->nVal);
				let[0] = ast->nVal[0];
				strcat(ast->nVal, let);
			}
			ast->datatype = DT_STRING;
			ast->name = NULL;
			ast->childern->sSize = 0;
		}
		else
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) + atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_INT;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		break;
	}
	case TOKEN_MINUS:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) - atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1),sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_INT;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_MUL:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) * atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_INT;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_DIV:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) / atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_INT;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_GREATT:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) > atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_LESST:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) < atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_AND:
	{
		if (a->datatype == DT_BOOL && b->datatype == DT_BOOL)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) && atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_OR:
	{
		if (a->datatype == DT_BOOL && b->datatype == DT_BOOL)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) || atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_E:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) == atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	case TOKEN_NE:
	{
		if (a->datatype == DT_INT && b->datatype == DT_INT)
		{
			char label[100] = { 0 };
			snprintf(label, 100, "%d", atoi(a->nVal) != atoi(b->nVal));
			ast->nVal = calloc((strlen(label) + 1), sizeof(char));
			if (ast->nVal == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(ast->nVal, label);
			ast->datatype = DT_BOOL;
			ast->name = NULL;
			ast->childern->sSize = 0;
			break;
		}
		else
		{
			printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
			exit(4);
		}
	}
	default:
	{
		printf("NahWeek! Error 5 - [NahWeek : Parser] Invalid Operation.\n");
		exit(4);
	}
	}
	free(a);
	free(b);
}

stack_N* ParserToStack(parser_N* parser)
{
	// Create and init stack.
	int paracnt = 0;
	stack_N* s = malloc(sizeof(stack_N*));
	if (s == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
		exit(1);
	}
	stack_init(s);

	// as long as in expr boundery, push.
	while (parser->token->eType != TOKEN_SEMIC && !((parser->token->eType == TOKEN_COMMA ||
		parser->token->eType == TOKEN_CPARA) && paracnt == 0))
	{
		if (parser->token->eType == TOKEN_OPARA) paracnt++;
		if (parser->token->eType == TOKEN_CPARA) paracnt--;
		stack_push(s, ParserEat(parser, parser->token->eType));
	}
	// send to formatting and return.
	return FormatExpr(s);
}

stack_N* FormatExpr(stack_N* s)
{
	// Allocate temporary stack.
	stack_N* temp = malloc(sizeof(stack_N));
	if (temp == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
		exit(1);
	}
	stack_init(temp);
	// reverse to temp.
	while (!stack_empty(*s))
	{
		stack_push(temp, stack_pop(s));
	}
	// slowly reverse to s. now checking high priority operators *, /, ==, !=, <=, >=, <, >.
	while (!stack_empty(*temp))
	{
		token_N* token = stack_pop(temp);
		// if operator
		if (token->eType == TOKEN_MUL || token->eType == TOKEN_DIV ||
			token->eType == TOKEN_E || token->eType == TOKEN_NE || 
			token->eType == TOKEN_EL || token->eType == TOKEN_EG || 
			token->eType == TOKEN_LESST || token->eType == TOKEN_GREATT)
		{
			// push op.
			stack_push(temp, token);

			// create subexpression stack.
			stack_N* subexpr = malloc(sizeof(stack_N));
			if (subexpr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
				exit(1);
			}
			stack_init(subexpr);

			// get next item.
			token = stack_pop(s);
			//check if left side of the equation is a sub expression.
			// if so, push it to subexpr stack.
			if (token->eType == TOKEN_CPARA)
			{
				int paracnt = 1;
				while (!(token->eType == TOKEN_OPARA && paracnt == 0))
				{
					stack_push(subexpr, token);
					token = stack_pop(s);
					if (token->eType == TOKEN_OPARA)
					{
						paracnt--;
					}
					else
					{
						if (token->eType == TOKEN_CPARA)
						{
							paracnt++;
						}
					}
				}
				stack_push(subexpr, token);
				if (s->top)
				{
					//check if left side is acutually a function call.
					if (((token_N*)s->top->info)->eType == TOKEN_ID)
					{
						stack_push(subexpr, stack_pop(s));
					}
				}
				stack_push(s, InitToken("(", TOKEN_OPARA));
				while (!stack_empty(*subexpr))
				{
					stack_push(s, stack_pop(subexpr));
				}
			}
			else
			{
				// surrould left side with (
				stack_push(s, InitToken("(", TOKEN_OPARA));
				stack_push(s, token);
				
			}
			// do same for right side.
			stack_push(s, stack_pop(temp));
			token = stack_pop(temp);
			if (token->eType == TOKEN_OPARA)
			{
				stack_N* subexpr = malloc(sizeof(stack_N));
				int paracnt = 1;
				if (subexpr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
					exit(1);
				}
				stack_init(subexpr);
				while (!(token->eType == TOKEN_CPARA && paracnt == 0))
				{
					stack_push(subexpr, token);
					token = stack_pop(temp);
					if (token->eType == TOKEN_OPARA)
					{
						paracnt++;
					}
					else
					{
						if (token->eType == TOKEN_CPARA)
						{
							paracnt--;
						}
					}
				}
				stack_push(subexpr, token);
				stack_push(temp, InitToken(")", TOKEN_CPARA));
				FormatExpr(subexpr);
				while (!stack_empty(*subexpr))
				{
					stack_push(temp, stack_pop(subexpr));
				}
			}
			else
			{
				stack_push(s, token);
				int paracnt = 1;
				if (temp->top)
				{
					if (((token_N*)temp->top->info)->eType == TOKEN_OPARA)
					{
						token = stack_pop(temp);
						while (!(token->eType == TOKEN_CPARA && paracnt == 0))
						{
							stack_push(s, token);
							token = stack_pop(temp);
							if (token->eType == TOKEN_OPARA)
							{
								paracnt++;
							}
							else
							{
								if (token->eType == TOKEN_CPARA)
								{
									paracnt--;
								}
							}
						}
						stack_push(s, token);
					}
				}
				stack_push(s, InitToken(")", TOKEN_CPARA));
			}
		}
		else
		{
			//if not operator, ignore.
			stack_push(s, token);
		}
	}
	// reverse to temp.
	while (!stack_empty(*s))
	{
		stack_push(temp, stack_pop(s));
	}
	// do the same as before but with 2nd set of operators.
	while (!stack_empty(*temp))
	{
		token_N* token = stack_pop(temp);
		if (token->eType == TOKEN_PLUS || token->eType == TOKEN_MINUS || token->eType == TOKEN_AND ||
			token->eType == TOKEN_OR)
		{
			stack_push(temp, token);
			token = stack_pop(s);
			if (token->eType == TOKEN_CPARA)
			{
				stack_N* subexpr = malloc(sizeof(stack_N));
				int paracnt = 1;
				if (subexpr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
					exit(1);
				}
				stack_init(subexpr);
				while (!(token->eType == TOKEN_OPARA && paracnt == 0))
				{
					stack_push(subexpr, token);
					token = stack_pop(s);
					if (token->eType == TOKEN_OPARA)
					{
						paracnt--;
					}
					else
					{
						if (token->eType == TOKEN_CPARA)
						{
							paracnt++;
						}
					}
				}
				stack_push(subexpr, token);
				if (s->top)
				{
					if (((token_N*)s->top->info)->eType == TOKEN_ID)
					{
						stack_push(subexpr, stack_pop(s));
					}
				}
				stack_push(s, InitToken("(", TOKEN_OPARA));
				while (!stack_empty(*subexpr))
				{
					stack_push(s, stack_pop(subexpr));
				}
			}
			else
			{
				stack_push(s, InitToken("(", TOKEN_OPARA));
				stack_push(s, token);
			}
			stack_push(s, stack_pop(temp));
			token = stack_pop(temp);
			if (token->eType == TOKEN_OPARA)
			{
				stack_N* subexpr = malloc(sizeof(stack_N));
				int paracnt = 1;
				if (subexpr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
					exit(1);
				}
				stack_init(subexpr);
				while (!(token->eType == TOKEN_CPARA && paracnt == 0))
				{
					stack_push(subexpr, token);
					token = stack_pop(temp);
					if (token->eType == TOKEN_OPARA)
					{
						paracnt++;
					}
					else
					{
						if (token->eType == TOKEN_CPARA)
						{
							paracnt--;
						}
					}
				}
				stack_push(subexpr, token);
				stack_push(temp, InitToken(")", TOKEN_CPARA));
				while (!stack_empty(*subexpr))
				{
					stack_push(temp, stack_pop(subexpr));
				}
			}
			else
			{
				stack_push(s, token);
				int paracnt = 1;
				if (temp->top)
				{
					if (((token_N*)temp->top->info)->eType == TOKEN_OPARA)
					{
						token = stack_pop(temp);
						while (!(token->eType == TOKEN_CPARA && paracnt == 0))
						{
							stack_push(s, token);
							token = stack_pop(temp);
							if (token->eType == TOKEN_OPARA)
							{
								paracnt++;
							}
							else
							{
								if (token->eType == TOKEN_CPARA)
								{
									paracnt--;
								}
							}
						}
						stack_push(s, token);
					}
				}
				stack_push(s, InitToken(")", TOKEN_CPARA));
			}
		}
		else
		{
			stack_push(s, token);
		}
	}
	// reverse stack.
	stack_reverse(&s);
	return s;
}

AST_N* ParserParseID(parser_N* parser)
{
	// allocate space for id token value.
	char* name;
	name = calloc(strlen(parser->token->pcVal) + 1, sizeof(char));
	if (name == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
		exit(1);
	}

	strcpy(name, parser->token->pcVal);
	// move to next token.
	free(ParserEat(parser, TOKEN_ID));
	switch (parser->token->eType)
	{
	// case '=' -> assignment.
	case TOKEN_EQUALS:
	{
		free(ParserEat(parser, TOKEN_EQUALS));
		AST_N* ast = InitAst(AST_ASSIGNMENT);
		ast->name = name;
		ast->datatype = FindVarDT(ast->name);
		ast->value = ParserParseExpr(ParserToStack(parser));
		return ast;
	}
	// case '(' -> call function.
	case TOKEN_OPARA:
	{
		AST_N* ast = InitAst(AST_CALL);
		ast->name = name;
		ast->value = ParserParseList(parser, ast->eType);
		return ast;
	}
	// case ':' -> name was actually type, now comes name.
	case TOKEN_COLON:
	{
		free(ParserEat(parser, TOKEN_COLON));
		AST_N* ast = InitAst(AST_VAR);
		char* datatype = name;

		name = calloc(strlen(parser->token->pcVal) + 1, sizeof(char));
		if (name == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Parser] - memory allocation failed.\n");
			exit(1);
		}
		strcpy(name, parser->token->pcVal);
		ast->name = name;
		ast->datatype = DatatypeToInt(datatype);
		ListPush(vartable, ast);
		free(ParserEat(parser, TOKEN_ID));
		// check again after declaration
		switch (parser->token->eType)
		{
		// case '(' -> function declaration.
		case TOKEN_OPARA:
		{
			ast->eType = AST_FUNCTION;
			ast->value = ParserParseList(parser, ast->eType);
			if(parser->token->eType == TOKEN_SEMIC)
			{
				return NULL;
			}
			return ast;
		}
		// case '=' -> dec & assign.
		case TOKEN_EQUALS:
		{
			free(ParserEat(parser, TOKEN_EQUALS));
			ast->value = ParserParseExpr(ParserToStack(parser));
			return ast;
		}
		default:
			return ast;
		}

		
	}
	default:
	{
		// if just word -> dynamically typed language, i guess?
		AST_N* ast = InitAst(AST_VAR);
		ast->name = name;
		ListPush(vartable, ast);

		return ast;
	}
	}
}

AST_N* ParserParseList(parser_N* parser, int eType)
{
	// eat (
	free(ParserEat(parser, TOKEN_OPARA));
	AST_N* compound = InitAst(AST_COMPOUND);
	if (parser->token->eType != TOKEN_CPARA)
	{
		// if function, parse child normally, if call, parse as expr to eval arg value.
		AST_N* child = eType == AST_FUNCTION ? ParserParseSwitch(parser) : ParserParseExpr(ParserToStack(parser));
		ListPush(compound->childern, child);

		// do again for all childs.
		while (parser->token->eType == TOKEN_COMMA)
		{
			free(ParserEat(parser, TOKEN_COMMA));
			child = eType == AST_FUNCTION ? ParserParseSwitch(parser) : ParserParseExpr(ParserToStack(parser));
			ListPush(compound->childern, child);
		}
	}
	// eat )
	free(ParserEat(parser, TOKEN_CPARA));
	return compound;
}

AST_N* ParserParseBlock(parser_N* parser)
{
	// eat {
	free(ParserEat(parser, TOKEN_OBRAC));
	AST_N* compound = InitAst(AST_COMPOUND);
	AST_N* child;
	// parse all child exprs and push to list.
	while (parser->token->eType != TOKEN_CBRAC)
	{
		child = ParserParseSwitch(parser);
		ListPush(compound->childern, child);
		if (parser->token->eType == TOKEN_SEMIC)
		{
			free(ParserEat(parser, TOKEN_SEMIC));
		}
	}
	// eat }
	free(ParserEat(parser, TOKEN_CBRAC));
	return compound;
}

AST_N* ParserParseSwitch(parser_N* parser) 
{
	// direct to parsing function.
	switch (parser->token->eType)
	{
	case TOKEN_ID:
	{
		return ParserParseID(parser);
	}
	case TOKEN_OPARA:
	{
		return ParserParseList(parser, NULL);
	}
	case TOKEN_OBRAC:
	{
		return ParserParseBlock(parser);
	}
	default:
	{
		printf("NahWeek! Error 4 - [NahWeek : Parser] Unexpected token '%s'\n", TokenToStr(parser->token));
		exit(4);
	}
	}
}

AST_N* ParserParseCompound(parser_N* parser)
{
	AST_N* compound = InitAst(AST_COMPOUND);
	// parse children.
	while (parser->token->eType != TOKEN_EOF)
	{
		AST_N* child = ParserParseSwitch(parser);
		if (child)
		{
			ListPush(compound->childern, child);
		}
		if (parser->token->eType == TOKEN_SEMIC)
		{
			free(ParserEat(parser, TOKEN_SEMIC));
		}
	}

	return compound;
}

AST_N* ParserParse(parser_N* parser)
{
	// init vartable, call parsing of global scope.
	vartable = InitList(sizeof(AST_N*));
	return ParserParseCompound(parser);
}