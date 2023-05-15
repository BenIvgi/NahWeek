#define _CRT_SECURE_NO_WARNINGS
#include "asm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include "io.h"
#include "list.h"

// Globals.
// Stack of the functions for closing.
stack_N* scopes;

// If counter to handle nesting lables.
int ifcnt = 0;
int whilecnt = 0;
int forcnt = 0;

// Counter of locals in a function to handle stack allocation.
int locals = 0;

list_N* strpool;

char* AsmGenerate(AST_N* ast)
{
	// Get standard library asm.
	char* sAsmStart = NahReadFile("C:\\Users\\User\\source\\repos\\NahWeek\\NahWeek\\NahWeekTemplate.asm");

	// Allocate and init functions stack.
	scopes = malloc(sizeof(stack_N));
	stack_init(scopes);

	strpool = InitList(sizeof(char*));

	// Add assembly settings to buffer.
	char* sBuffer = calloc(strlen("IDEAL\nMODEL small\nSTACK 800h\n") + 1, sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcpy(sBuffer, "IDEAL\nMODEL small\nSTACK 800h\n");

	// Get nahweek code in assembly.
	char* sAsmCode = AsmSwitch(ast);

	// Add datasegment to buffer.
	char* dataseg = AsmDataSegment();
	sBuffer = realloc(sBuffer, (strlen(dataseg) + strlen(sBuffer) + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, dataseg);

	// Add standard library to buffer.
	sBuffer = realloc(sBuffer, (strlen(sAsmStart) + strlen(sBuffer) + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, sAsmStart);

	// Add nahweek code to buffer.
	sBuffer = realloc(sBuffer, (strlen(sAsmCode) + strlen(sBuffer) + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, sAsmCode);

	// Close the assembly start label.
	sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("\nend start\n") + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, "end start\n");
	free(scopes);
	free(strpool);

	// Return buffer.
	return sBuffer;
}

char* AsmSwitch(AST_N* ast)
{
	// Switch case for creating the right assembly piece of code.
	switch (ast->eType)
	{
	case AST_COMPOUND: return AsmCompound(ast);
	case AST_CALL: return AsmCall(ast);
	case AST_FUNCTION: return AsmFunction(ast);
	case AST_ASSIGNMENT: return AsmAssignment(ast);
	case AST_VAR: return AsmVar(ast);
	default:
	{
		printf("NahWeek! Error 4 - [NahWeek : Assembly] Unexpected AST type '%d'\n", ast->eType);
		exit(4);
	}
	}
}

char* AsmCompound(AST_N* ast)
{
	// Allocate memory for buffer.
	char* sBuffer = calloc(1, sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}

	// For every child of said compound.
	for (int i = 0; i < (int)ast->childern->sSize; i++)
	{
		AST_N* child = (AST_N*)ast->childern->ppvItems[i];

		// Redirect to code generator switch.
		char* sNext = AsmSwitch(child);
		
		// Add next value to buffer.
		sBuffer = realloc(sBuffer, (strlen(sNext) + strlen(sBuffer) + 1) * sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, sNext);
	}

	// if there are any functions that are'nt closed.
	if (!stack_empty(*scopes))
	{
		// if the function is an "if".
		AST_N* recent = stack_pop(scopes);
		if (!strcmp(recent->name, "if"))
		{
			// close the if and generate its skipping lable.
			char* index = recent->nVal;
			sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen(index) + 4) * sizeof(char));
			strcat(sBuffer, "i");
			strcat(sBuffer, index);
			strcat(sBuffer, ":\n");
		}
		else
		{
			// if the function is a "while"
			if (!strcmp(recent->name, "while"))
			{
				char* index = recent->nVal;
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}

				// Add literal / variable name to condition.
				char* sCond = AsmExpr(recent->value->childern->ppvItems[0]);
				sBuffer = realloc(sBuffer, (strlen(sCond) + strlen(sBuffer) + 1) * sizeof(char));
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(sBuffer, sCond);

				// Add the cmp to the code.
				sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("jne w\new:\n") + strlen(index)*2 + strlen("cmp ax, 0\n") + 1) * sizeof(char));
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(sBuffer, "cmp ax, 0\n");
				strcat(sBuffer, "jne w");
				strcat(sBuffer, index);
				strcat(sBuffer, "\new");
				strcat(sBuffer, index);
				strcat(sBuffer, ":\n");
			}
			else
			{
				// if the function is an "iter"
				if (!strcmp(recent->name, "iter"))
				{
					char* index = recent->nVal;
					char* iterate = ((AST_N*)recent->value->childern->ppvItems[2])->name;
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}


					// Add literal / variable name to condition.
					char* sLim = AsmExpr(recent->value->childern->ppvItems[1]);
					sBuffer = realloc(sBuffer, (strlen(sLim) + strlen(sBuffer) + 1) * sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					strcat(sBuffer, sLim);

					// Add the cmp to the code.
					sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("mov ax, limit\nmov bx, \ninc ax\nmov ") + strlen(index)*3 + strlen(iterate)*2 +
						strlen(", bx\ncmp bx, ax\njb f\nef:\n") + 1) * sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					strcat(sBuffer, "mov ax, limit");
					strcat(sBuffer, index);
					strcat(sBuffer, "\nmov bx, ");
					strcat(sBuffer, iterate);
					strcat(sBuffer, "\ninc bx\nmov ");
					strcat(sBuffer, iterate);
					strcat(sBuffer, ", bx\ncmp bx, ax");
					strcat(sBuffer, "\njb f");
					strcat(sBuffer, index);
					strcat(sBuffer, "\nef");
					strcat(sBuffer, index);
					strcat(sBuffer, ":\n");
				}
				else
				{
					// Else return the function to the stack.
					stack_push(scopes, recent);
				}
			}
		}
	}

	// Return the buffer.
	return sBuffer;
}

char* AsmCall(AST_N* ast)
{
	// if the function is a "send".
	if (!strcmp(ast->name, "send"))
	{
		// Declare variable.
		char label[100] = { 0 };
		snprintf(label, 100, "%d", locals*2);
		char* sBuffer = calloc(1, sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}

		// Add return value: return value if stated, 0 otherwise.
		char* sReturnValue = ast->value->childern->sSize ? AsmExpr(ast->value->childern->ppvItems[0]) : "mov ax, 0\n";
		sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen(sReturnValue) + 1)* sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, sReturnValue);

		// Add local deallocation code to buffer.
		sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("pop bp\nadd sp, ") +
			strlen(label) + 1) * sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, "pop bp\nadd sp, ");
		strcat(sBuffer, label);

		// Init locals count.
		locals = 0;

		// Add the process closing code to the buffer.
		AST_N* func = stack_pop(scopes);
		if (!ast->value->childern->sSize)
		{
			if (func->datatype != DT_VOID)
			{
				printf("NahWeek! Error 10 - [NahWeek : Assembly] - function missing return value.\n");
				exit(9);
			}
		}
		else
		{
			if (func->datatype != ((AST_N*)ast->value->childern->ppvItems[0])->datatype && func->datatype)
			{
				printf("NahWeek! Error 9 - [NahWeek : Assembly] - function datatype is incompatible with its return value.\n");
				exit(9);
			}
		}
		sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("\nret ") + strlen(func->name) + strlen("\nendp ") + 2)*sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, "\nret ");

		// If the function had parameters, free them.
		if (func->value->childern->sSize)
		{
			snprintf(label, 100, "%d", func->value->childern->sSize * 2);
			sBuffer = realloc(sBuffer, strlen(sBuffer) + strlen(label) + 2, sizeof(char));
			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(sBuffer, label);
		}
		strcat(sBuffer, "\nendp ");
		strcat(sBuffer, func->name);
		strcat(sBuffer, "\n");

		// Return the buffer.
		return sBuffer;
	}
	else
	{
		// If the function is an "if"
		if (!strcmp(ast->name, "if"))
		{
			// Declare variables.
			char* label = calloc(4, sizeof(char));
			if (label == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			snprintf(label, 3, "%d", ifcnt);
			char* sBuffer = calloc(1, sizeof(char));
			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}

			// Add literal / variable name to condition.
			char* sCond = AsmExpr(ast->value->childern->ppvItems[0]);
			
			sBuffer = realloc(sBuffer, (strlen(sCond) + strlen(sBuffer) + 1) * sizeof(char));
			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(sBuffer, sCond);
			// Add the cmp to the code.
			sBuffer = realloc(sBuffer, (strlen(sBuffer) + +strlen("je \n") + strlen(label) +
				strlen("cmp ax, 0\n") + 1) * sizeof(char));

			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(sBuffer, "cmp ax, 0\n");

			strcat(sBuffer, "je i");
			strcat(sBuffer, label);
			strcat(sBuffer, "\n");

			// Increase if count.
			ast->nVal = label;
			stack_push(scopes, ast);
			ifcnt++;
			// Return the buffer.
			return sBuffer;

		}
		else
		{
			if (!strcmp(ast->name, "while"))
			{
				char* label  = calloc(4, sizeof(char));
				if (label == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				snprintf(label, 3, "%d", whilecnt);
				char* sBuffer = calloc(1, sizeof(char));
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				// Add literal / variable name to condition.
				char* sCond = AsmExpr(ast->value->childern->ppvItems[0]);
				sBuffer = realloc(sBuffer, (strlen(sCond) + strlen(sBuffer) + 1) * sizeof(char));
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(sBuffer, sCond);
				// Add the cmp to the code.
				sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("je w\new:\n") + strlen(label) * 2 + strlen("cmp ax, 0\n") + 1) * sizeof(char));
				if (sBuffer == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(sBuffer, "cmp ax, 0\n");
				strcat(sBuffer, "je ew");
				strcat(sBuffer, label);
				strcat(sBuffer, "\nw");
				strcat(sBuffer, label);
				strcat(sBuffer, ":\n");
				ast->nVal = label;
				stack_push(scopes, ast);
				whilecnt++;
				return sBuffer;
			}
			else
			{
				// if the function is an "iter"
				if (!strcmp(ast->name, "iter"))
				{
					// get the for count.
					char* label = calloc(4, sizeof(char));
					if (label == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					snprintf(label, 3, "%d", forcnt);
					char* sBuffer = calloc(1, sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					// create following: iterator as variable, high limit as variable, low limit.
					AST_N* iterate = InitAst(AST_VAR);
					AST_N* Limit = InitAst(AST_VAR);
					AST_N* lowLim = InitAst(AST_EXPR);
					lowLim->nVal = "0";
					iterate->datatype = DT_INT;
					Limit->datatype = DT_INT;
					char* index = calloc(2, sizeof(char));
					if (index == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					// if no iterator was specified, iterator starts at 0, otherwise, starts at low limit.
					char* inititerate = ast->value->childern->sSize == 1 ? "mov ax, 0\n" : AsmExpr((AST_N*)ast->value->childern->ppvItems[0]);
					// upper limit is first param if no others are present, otherwise second.
					char* upLim = ast->value->childern->sSize == 1 ? AsmExpr((AST_N*)ast->value->childern->ppvItems[0]) :
						AsmExpr((AST_N*)ast->value->childern->ppvItems[1]);
					// auto index is char for ascii('i' + number of fors)
					*index = 'i' + forcnt;
					// if iterator name was specified than use it, otherwise auto index.
					iterate->name = ast->value->childern->sSize >= 3 ? ((AST_N*)ast->value->childern->ppvItems[2])->name : index;
					// save limit in variable.
					Limit->name = calloc(strlen("limit") + strlen(label) + 1, sizeof(char));
					Limit->value = (AST_N*)ast->value->childern->ppvItems[0];
					if (Limit == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					strcat(Limit->name, "limit");
					strcat(Limit->name, label);

					// add all together.
					char* createLimit = AsmVar(Limit);
					char* createIterate = AsmVar(iterate);
					sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen(Limit->name) + strlen(createLimit) + strlen("mov ax, \ncmp ax, bx\n je ef\nf:\n") + strlen(inititerate) + strlen(createIterate) + strlen(label) +
						strlen("mov , ax\nmov bx, ax\n") + strlen(iterate->name) + 2
						) * sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					strcat(sBuffer, createLimit);
					strcat(sBuffer, createIterate);
					strcat(sBuffer, inititerate);
					strcat(sBuffer, "mov ");
					strcat(sBuffer, iterate->name);
					strcat(sBuffer,", ax\nmov bx, ax\n");
					strcat(sBuffer, "mov ax, ");
					strcat(sBuffer, Limit->name);
					strcat(sBuffer, "\ncmp ax, bx\nje ef");
					strcat(sBuffer, label);
					strcat(sBuffer, "\nf");
					strcat(sBuffer, label);
					strcat(sBuffer, ":\n");
					if (ast->value->childern->sSize == 1)
					{
						ListPush(ast->value->childern, lowLim);
						ListPush(ast->value->childern, iterate);
					}
					else
					{
						if (ast->value->childern->sSize == 2)
						{
							ListPush(ast->value->childern, iterate);
						}
					}
					// nval of iter is it's number.
					ast->nVal = label;
					// add to scopes.
					stack_push(scopes, ast);
					forcnt++;
					return sBuffer;
				}
				else
				{

					// Declare variables.
					char* sBuffer = calloc(1, sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}

					// For each argument sent.
					for (int i = 0; i < ast->value->childern->sSize; i++)
					{
						char* param = AsmExpr(ast->value->childern->ppvItems[i]);
						
						// add the stack pushing code to buffer.
						sBuffer = realloc(sBuffer, (strlen(sBuffer) +
							strlen("push ax\n") +
							strlen(param) + 1) * sizeof(char));
						if (sBuffer == NULL)
						{
							printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
							exit(1);
						}
						strcat(sBuffer, param);
						strcat(sBuffer, "push ax\n");
					}

					// Add the function calling code to buffer.
					sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("call ") + strlen(ast->name) + 2) * sizeof(char));
					if (sBuffer == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					strcat(sBuffer, "call ");
					strcat(sBuffer, ast->name);
					strcat(sBuffer, "\n");

					// Return the buffer.
					return sBuffer;
				}
			}
		}
	}
}

char* AsmFunction(AST_N* ast)
{
	// Allocate space for variables.
	char* sBuffer = calloc(1, sizeof(char));
	char label[100] = { 0 };
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}

	// Add function start to buffer.
	sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("proc ") + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcpy(sBuffer, "proc ");

	// Add function name to buffer.
	sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen(ast->name) + strlen("\npush bp\n") + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, ast->name);
	strcat(sBuffer, "\npush bp\n");

	// Push function to stack.
	stack_push(scopes, ast);
	
	for (int i = 0; i < ast->value->childern->sSize; i++)
	{
		snprintf(label, 100, "%d", ((int)(ast->value->childern->sSize - (i + 1)) + 2) * 2);
		sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen(((AST_N*)ast->value->childern->ppvItems[i])->name)
			+ strlen(" equ [bp + ") + strlen(label) + 3) * sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, ((AST_N*)ast->value->childern->ppvItems[i])->name);
		strcat(sBuffer, " equ [bp + ");
		strcat(sBuffer, label);
		strcat(sBuffer, "]\n");
	}
	// Return the buffer.
	return sBuffer;
}

char* AsmAssignment(AST_N* ast)
{
	// Allocate memory.
	char* sAssignVal = AsmExpr(ast->value);

	char* sBuffer = calloc(strlen(sAssignVal) + 1, sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	// add the assigned value calculation.
	strcpy(sBuffer, sAssignVal);

	switch (ast->datatype)
	{
	case DT_INT:
	{
		// to add string to int, call toNumber.
		if (ast->value->datatype == DT_STRING)
		{
			sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("push ax\ncall toNumber\n") + 1) * sizeof(char));
			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(sBuffer, "push ax\ncall toNumber\n");
		}
		break;
	}
	case DT_STRING:
	{
		// to add bool to string, call boolToString.
		if (ast->value->datatype == DT_BOOL)
		{
			sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("push ax\ncall boolToStr\n") + 1) * sizeof(char));
			if (sBuffer == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(sBuffer, "push ax\ncall boolToStr\n");
		}
		break;
	}
	case DT_BOOL:
	{
		break;
	}
	default:
		break;
	}
	

	// Add the variable name.
	char* sVarName = ast->name;
	sBuffer = (char*)realloc(sBuffer, (strlen(sBuffer) + strlen(sVarName) + strlen("mov , ax\n") + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, "mov ");
	strcat(sBuffer, sVarName);
	strcat(sBuffer, ", ax\n");

	// Return buffer.
	return sBuffer;
}

char* AsmExpr(AST_N* ast)
{
	// as long as not a leaf.
	if (ast->childern->sSize)
	{
		// scan in "inorder"
		char* term1 = AsmExpr(ast->childern->ppvItems[0]);
		char* expr = calloc(strlen(term1) + 1, sizeof(char));
		if (expr == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcpy(expr, term1);
		// save operand in stack.
		expr = (char*)realloc(expr, (strlen(expr) + strlen("push ax\n") + 1) * sizeof(char));
		if (expr == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(expr, "push ax\n");
		char* term2 = AsmExpr(ast->childern->ppvItems[1]);
		expr = (char*)realloc(expr, (strlen(expr) + strlen(term2) + 1) * sizeof(char));
		if (expr == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(expr, term2);
		expr = realloc(expr, (strlen(expr) + strlen("pop bx\n") + 1) * sizeof(char));
		if (expr == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(expr, "pop bx\n");
		// preform the assembly operation.
		if (!strcmp(ast->name, "+"))
		{
			if (((AST_N*)ast->childern->ppvItems[0])->datatype == DT_STRING &&
				((AST_N*)ast->childern->ppvItems[1])->datatype == DT_STRING)
			{
				expr = realloc(expr, (strlen(expr) + strlen("push ax\npush bx\ncall strAdd\n") + 1) * sizeof(char));
				if (expr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(expr, "push ax\npush bx\ncall strAdd\n");
				ast->datatype = DT_STRING;
			}
			else
			{
				if (((AST_N*)ast->childern->ppvItems[0])->datatype == DT_STRING)
				{
					if (((AST_N*)ast->childern->ppvItems[1])->datatype == DT_BOOL)
					{
						expr = realloc(expr, (strlen(expr) + strlen("push ax\ncall boolToStr\npush ax\npush bx\ncall strAdd\n") + 1) * sizeof(char));
						if (expr == NULL)
						{
							printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
							exit(1);
						}
						strcat(expr, "push ax\ncall boolToStr\npush ax\npush bx\ncall strAdd\n");
						ast->datatype = DT_STRING;
					}
					else
					{
						expr = realloc(expr, (strlen(expr) + strlen("push ax\ncall toString\npush ax\npush bx\ncall strAdd\n") + 1) * sizeof(char));
						if (expr == NULL)
						{
							printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
							exit(1);
						}
						strcat(expr, "push ax\ncall toString\npush ax\npush bx\ncall strAdd\n");
						ast->datatype = DT_STRING;
					}
				}
				else
				{
					if (((AST_N*)ast->childern->ppvItems[1])->datatype == DT_STRING)
					{
						if (((AST_N*)ast->childern->ppvItems[0])->datatype == DT_BOOL)
						{
							expr = realloc(expr, (strlen(expr) + strlen("push ax\npush bx\ncall boolToStr\npush ax\ncall strAdd\n") + 1) * sizeof(char));
							if (expr == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
								exit(1);
							}
							strcat(expr, "push ax\npush bx\ncall boolToStr\npush ax\ncall strAdd\n");
							ast->datatype = DT_STRING;
						}
						else
						{
							expr = realloc(expr, (strlen(expr) + strlen("push ax\npush bx\ncall toString\npush ax\ncall strAdd\n") + 1) * sizeof(char));
							if (expr == NULL)
							{
								printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
								exit(1);
							}
							strcat(expr, "push ax\npush bx\ncall toString\npush ax\ncall strAdd\n");
							ast->datatype = DT_STRING;
						}
						
					}
					else
					{
						expr = realloc(expr, (strlen(expr) + strlen("add ax, bx\n") + 1) * sizeof(char));
						if (expr == NULL)
						{
							printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
							exit(1);
						}
						strcat(expr, "add ax, bx\n");
						ast->datatype = DT_INT;
					}
				}
				
			}
			
		}
		if (!strcmp(ast->name, "&&"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("and ax, bx\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "and ax, bx\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "||"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("or ax, bx\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "or ax, bx\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "=="))
		{
			if (((AST_N*)ast->childern->ppvItems[0])->datatype == DT_STRING &&
				((AST_N*)ast->childern->ppvItems[1])->datatype == DT_STRING)
			{
				expr = realloc(expr, (strlen(expr) + strlen("push ax\npush bx\ncall strCmp\n") + 1) * sizeof(char));
				if (expr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(expr, "push ax\npush bx\ncall strCmp\n");
			}
			else
			{
				expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\nje $ + 7\nmov ax, 0\njmp $ + 5\nmov ax, 1\n") + 1) * sizeof(char));
				if (expr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(expr, "cmp ax, bx\nje $ + 7\nmov ax, 0\njmp $ + 5\nmov ax, 1\n");
			}
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "!="))
		{
			if (((AST_N*)ast->childern->ppvItems[0])->datatype == DT_STRING &&
				((AST_N*)ast->childern->ppvItems[1])->datatype == DT_STRING)
			{
				expr = realloc(expr, (strlen(expr) + strlen("push ax\npush bx\ncall strCmp\nsub ax, 1\nneg ax\n") + 1) * sizeof(char));
				if (expr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(expr, "push ax\npush bx\ncall strCmp\nsub ax, 1\nneg ax\n");
			}
			else
			{
				expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\nje $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n") + 1) * sizeof(char));
				if (expr == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcat(expr, "cmp ax, bx\nje $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n");
			}
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "<="))
		{
			expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\njge $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "cmp ax, bx\njg $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, ">="))
		{
			expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\njle $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "cmp ax, bx\njl $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, ">"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\njl $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "cmp ax, bx\njle $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "<"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("cmp ax, bx\njg $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "cmp ax, bx\njge $ + 7\nmov ax, 1\njmp $ + 5\nmov ax, 0\n");
			ast->datatype = DT_BOOL;
		}
		if (!strcmp(ast->name, "-"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("sub ax, bx\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "sub ax, bx\n");
			ast->datatype = DT_INT;
		}
		if (!strcmp(ast->name, "*"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("mul bx\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "mul bx\n");
			ast->datatype = DT_INT;
		}
		if (!strcmp(ast->name, "/"))
		{
			expr = realloc(expr, (strlen(expr) + strlen("idiv bx\n") + 1) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, "idiv bx\n");
			ast->datatype = DT_INT;
		}
		return expr;
	}
	else
	{
		// if a fucntion call, assemble it.
		if (ast->value)
		{
			return AsmCall(ast);
		}
		else
		{
			// move leaf value to ax.
			char* expr = calloc(strlen("mov ax, ") + 1, sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcpy(expr, "mov ax, ");
			char* val;
			if (ast->datatype == DT_STRING && ast->nVal)
			{
				char label[100] = { 0 };
				snprintf(label, 100, "%d", (int)strpool->sSize);
				val = calloc(strlen("offset str") + strlen(label) + 1, sizeof(char));
				if (val == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				strcpy(val, "offset str");
				strcat(val, label);
				char* string = calloc(1, sizeof(char));
				if (string == NULL)
				{
					printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
					exit(1);
				}
				for (int i = 1; i < strlen(ast->nVal) - 1; i++)
				{
					char label[100] = { 0 };
					snprintf(label, 100, "%d, ", ast->nVal[i]);
					string = realloc(string, (strlen(string) + strlen(label) + 3) * sizeof(char));
					if (string == NULL)
					{
						printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
						exit(1);
					}
					
					strcat(string, label);
				}
					ListPush(strpool, string);
			}
			else
			{
				val = ast->name ? ast->name : ast->nVal;
			}
			expr = (char*)realloc(expr, (strlen(expr) + strlen(val) + 3) * sizeof(char));
			if (expr == NULL)
			{
				printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
				exit(1);
			}
			strcat(expr, val);
			strcat(expr, "\n");
			return expr;
		}
	}
}

char* AsmVar(AST_N* ast)
{
	// Allocate memory.
	char label[100] = { 0 };
	char* sBuffer;

	// if there are no locals, init stack allocating code.
	if (!locals)
	{
		sBuffer = calloc(strlen("mov bp, sp\n") + 1, sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcpy(sBuffer, "mov bp, sp\n");
	}

	// Allocate space on the stack for the variable.
	sBuffer = calloc(strlen(ast->name) + strlen("sub sp, 2\n") + 1, sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcpy(sBuffer, "sub sp, 2\n");
	strcat(sBuffer, ast->name);
	locals++;

	// Create alias.
	char* bp = "[bp - ";
	snprintf(label, 100, "%d", (locals)*2);
	sBuffer = realloc(sBuffer, (strlen(" equ ") + strlen(sBuffer) + strlen(bp) + strlen(label) + 3)* sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, " equ ");
	strcat(sBuffer, bp);
	strcat(sBuffer, label);
	strcat(sBuffer, "]\n");

	// Assign value if exists.
	char* sValue = ast->value ? AsmAssignment(ast) : "\0";
	sBuffer = (char*)realloc(sBuffer, (strlen(sValue) + strlen(sBuffer) + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, sValue);
	
	// Return buffer.
	return sBuffer;
}

char* AsmDataSegment()
{
	// lable to add numbers to strings.
	char label[100] = { 0 };
	char* sBuffer = calloc(strlen("DATASEG\n") + 1, sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcpy(sBuffer, "DATASEG\n");
	// for each string in strpool save name as str+i, and value as the ascii char values ended with null terminator.
	for (int i = 0; i < strpool->sSize; i++)
	{
		snprintf(label, 100, "%d", i);
		sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("str") + strlen(" db ") + strlen("0\n") +
			strlen((char*)(strpool->ppvItems[i])) + strlen(label) + 1) * sizeof(char));
		if (sBuffer == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
			exit(1);
		}
		strcat(sBuffer, "str");
		strcat(sBuffer, label);
		strcat(sBuffer, " db ");
		strcat(sBuffer, (char*)(strpool->ppvItems[i]));
		strcat(sBuffer, "0\n");
	}
	// save end of data segment in variable.
	sBuffer = realloc(sBuffer, (strlen(sBuffer) + strlen("eods dw $ + 2\n") + 1) * sizeof(char));
	if (sBuffer == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : Assembly] - memory allocation failed.\n");
		exit(1);
	}
	strcat(sBuffer, "eods dw $ + 2\n");
	return sBuffer;
}