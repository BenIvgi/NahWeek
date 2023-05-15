#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "token.h"

typedef enum { FALSE, TRUE, SHALOSH } BOOL;

void stack_init(stack_N* s)
{
	s->top = NULL;
}

int stack_empty(stack_N s)
{
	return(s.top == NULL);
}

int stack_full(stack_N s)
{
	return 0;
}

void stack_push(stack_N* s, stack_item x)
{
	struct node_type* temp = malloc(sizeof(struct node_type));
	if (temp == NULL)
	{
		printf("Error 1 - memory allocation failed.\n");
		exit(1);
	}
	else
	{
		temp->info = x;
		temp->next = s->top;
		s->top = temp;
	}
}

stack_item stack_pop(stack_N* s)
{
	if (!stack_empty(*s))
	{
		struct node_type* temp = s->top;
		stack_item data = s->top->info;
		s->top = temp->next;
		//free(temp);
		return data;
	}
	return NULL;
}

stack_item stack_top(stack_N s)
{
	if (!stack_empty(s))
	{
		stack_item data = (s.top)->info;
		return data;
	}
	return NULL;
}

void printstack(stack_N* s)
{
	token_N* x;
	stack_N s1;
	stack_init(&s1);
	while (!stack_empty(*s))
	{
		x = stack_pop(s);
		printf("%s", x->pcVal);
		stack_push(&s1, x);
	}
	printf("\n");
	turn(s, s1);

}

void turn(stack_N* s, stack_N s1)
{
	while (!stack_empty(s1))
	{
		stack_push(s, stack_pop(&s1));
	}
}

void stack_reverse(stack_N** s)
{
	stack_N* temp1 = malloc(sizeof(stack_N*));
	if (temp1 == NULL)
	{
		printf("Error 1 - memory allocation failed.\n");
		exit(1);
	}
	stack_init(temp1);
	turn(temp1, **s);
	*s = temp1;
	
}