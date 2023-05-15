#include "AST.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

AST_N* InitAst(int eType)
{
	// allocate space for ast, init fields.
	AST_N* ast = calloc(1, sizeof(struct AST_STRUCT));
	if (ast == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : AST] - memory allocation failed.\n");
		exit(1);
	}
	ast->eType = eType;

	if (eType == AST_COMPOUND || eType == AST_EXPR)
	{
		ast->childern = InitList(sizeof(struct AST_STRUCT*));
	}
	return ast;
}

void PrintAST(AST_N* ast, int depth)
{
	printf("[NahWeek : Parser]\nDisplaying AST\nfilename: %s\n", __argv[1]);
	// print every child of global scope.
	for (int i = 0; i < ast->childern->sSize; i++)
	{
		printf("\n");
		PrintChild(ast->childern->ppvItems[i], depth);
	}
	printf("(C) Ben David Ivgi, 2023\n");
}

int DatatypeToInt(char* name)
{
	if (!strcmp(name, "number")) return DT_INT;
	if (!strcmp(name, "bool")) return DT_BOOL;
	if (!strcmp(name, "string")) return DT_STRING;
	if (!strcmp(name, "void")) return DT_VOID;
	return DT_PTR;
}

void PrintIndentation(int depth)
{
	for (int i = 0; i < depth; i++) printf("|---");
}

void PrintChild(AST_N* child, int depth)
{
	switch (child->eType)
	{
	case AST_CALL:
	{
		PrintIndentation(depth);
		printf("[\n");
		PrintIndentation(depth + 1);
		printf("Name: %s\n", child->name);
		PrintIndentation(depth + 1);
		printf("Arguments:\n");
		PrintChild(child->value, depth + 1);
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	case AST_ASSIGNMENT:
	{
		PrintIndentation(depth);
		printf("[\n");
		PrintIndentation(depth + 1);
		printf("Name: %s\n", child->name);
		PrintIndentation(depth + 1);
		PrintChild(child->value, depth + 1);
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	case AST_EXPR:
	{
		PrintIndentation(depth);
		printf("[\n");
		PrintIndentation(depth + 1);
		printf("Type: %d\n", child->datatype);
		PrintIndentation(depth + 1);
		printf("Value: ");
		PrintExpr(child);
		printf("\n");
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	case AST_VAR:
	{
		PrintIndentation(depth);
		printf("[\n");
		PrintIndentation(depth + 1);
		printf("Name: %s\n", child->name);
		PrintIndentation(depth + 1);
		printf("Type: %d\n", child->datatype);
		if (child->value)
		{
			PrintChild(child->value, depth + 1);
		}
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	case AST_COMPOUND:
	{
		PrintIndentation(depth);
		printf("[\n");
		for (int i = 0; i < child->childern->sSize; i++)
		{
			AST_N* comp = child->childern->ppvItems[i];
			PrintIndentation(depth);
			printf("Item No. %d: \n//\n", i);
			PrintChild(comp, depth + 1);
		}
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	case AST_FUNCTION:
	{
		PrintIndentation(depth);
		printf("[\n");
		PrintIndentation(depth + 1);
		printf("Name: %s\n", child->name);
		PrintIndentation(depth + 1);
		printf("Return Value Type: %d\n", child->datatype);
		PrintIndentation(depth + 1);
		printf("Parameters:\n");
		PrintChild(child->value, depth + 1);
		PrintIndentation(depth);
		printf("]\n");
		break;
	}
	}
}

void PrintExpr(AST_N* child)
{
	// as long as not leaf, go inside.
	if (child->childern->sSize)
	{
		printf("(");
		PrintExpr(child->childern->ppvItems[1]);
		printf("%s", child->name);
		if (child->childern->ppvItems[0])
		{
			PrintExpr(child->childern->ppvItems[0]);
		}
		printf(")");
	}
	else
	{
		// print operand.
		printf("%s", child->name ? child->name : child->nVal);
		if (child->value)
		{
			// case call -> print args.
			printf("(");
			for (int i = 0; i < child->value->childern->sSize; i++)
			{
				AST_N* comp = child->value->childern->ppvItems[i];
				PrintExpr(comp);
				if (child->value->childern->sSize - i != 1)
				{
					printf(", ");
				}
			}
			printf(")");
		}
	}
}