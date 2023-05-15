#pragma once
#ifndef NAH_LIST_H
#define NAH_LIST_H
#include <stdlib.h>

//-----------------------------------------------------------------------------
// List
// -----------------------------------------------------------------------------
//
// General : structure resembles a 
// 
// 
// 
// .
//
// Properties :
// ppvItems - a pointer to the items pointers array.
// sSize - the number of items.
// sItemSize - the size of each item.
//
//-----------------------------------------------------------------------------
typedef struct LIST_STRUCT
{
	void** ppvItems;
	size_t sSize;
	size_t sItemSize;
} list_N;

//-----------------------------------------------------------------------------
// Initialize list.
//-----------------------------------------------------------------------------
//
// General : allocate memory and init the list's components.
//
// Parameters :
// ItemSize - the size of each item in the list.
//
// Return Value : a pointer to a list.
//
//-----------------------------------------------------------------------------
list_N* InitList(size_t ItemSize);

//-----------------------------------------------------------------------------
// Push an item to the list.
//-----------------------------------------------------------------------------
//
// General : add an item to the list.
//
// Parameters :
// list - a pointer to a list.
// item - a pointer to the item.
//
// Return Value : None.
//
//-----------------------------------------------------------------------------
void ListPush(list_N* list, void* item);
#endif
