#pragma once
#pragma once
#ifndef NAH_STACK_H
#define NAH_STACK_H

typedef void* stack_item;
//-----------------------------------------------------------------------------
// node_type
// -------------------
//
// General : structure resembles a node in a stack.
//
// Properties :
// info - can be changed by typdef in line 1.
// next - a pointer to a node_type.
//
//-----------------------------------------------------------------------------
struct node_type
{
	stack_item info;
	struct node_type* next;
};
//-----------------------------------------------------------------------------
// stack
// -------------------
//
// General : structure resembles a stack.
//
// Properties :
// top - a pointer to a node.
//
//-----------------------------------------------------------------------------
typedef struct
{
	struct node_type* top;
}stack_N;
//-----------------------------------------------------------------------------
// Construct a Stack
// -------------------------------------------------
//
// General : the function sets the top value as null.
//
// Parameters :
// s - a pointer to a stack.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void stack_init(stack_N* s);
//-----------------------------------------------------------------------------
// Is the Stack Empty?
// -------------------------------------------------
//
// General : the function checks if the stack is empty.
//
// Parameters :
// s - a stack
//
// Return Value : 1 if empty, 0 otherwise. 
//
//-----------------------------------------------------------------------------
int stack_empty(stack_N s);
//-----------------------------------------------------------------------------
// Is The Stack Full?
// -------------------------------------------------
//
// General : the function checks if the stack is full.
//
// Parameters :
// s - a stack
//
// Return Value : 1 if full, 0 otherwise. 
//
//-----------------------------------------------------------------------------
int stack_full(stack_N s);
//-----------------------------------------------------------------------------
// Push item to the stack
// -------------------------------------------------
//
// General : pushes a item into a stack.
//
// Parameters :
// s - a pointer to a stack.
// x - a stack item.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void stack_push(stack_N* s, stack_item x);
//-----------------------------------------------------------------------------
// Pop the Top Item.
// -------------------------------------------------
//
// General : the function extracts the top item from the stack.
//
// Parameters :
// s - a pointer to a stack.
//
// Return Value : The item's value.
//
//-----------------------------------------------------------------------------
stack_item stack_pop(stack_N* s);
//-----------------------------------------------------------------------------
// Turn The Stack Upside Down.
// -------------------------------------------------
//
// General : the function puts the content of one stack in another, upside down.
//
// Parameters :
// s - a pointer to a stack.
// s1 - a stack.
//
// Return Value : 
//
//-----------------------------------------------------------------------------
void turn(stack_N* s, stack_N s1);
//-----------------------------------------------------------------------------
// Print The Stack
// -------------------------------------------------
//
// General : the function prints the content of the stack.
//
// Parameters :
// s - a pointer to a stack.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void printstack(stack_N* s);

//-----------------------------------------------------------------------------
// Reverse a Stack.
// -------------------------------------------------
//
// General : the function reverses a stack's content.
//
// Parameters :
// s - a pointer to a prointer to a stack.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void stack_reverse(stack_N** s);
#endif