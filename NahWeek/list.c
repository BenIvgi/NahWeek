#include "list.h"

list_N* InitList(size_t ItemSize)
{
	// Allocate memory.
	list_N* list = calloc(1, sizeof(struct LIST_STRUCT));
	if (list == NULL)
	{
		printf("NahWeek! Error 1 - [NahWeek : List] - memory allocation failed.\n");
		exit(1);
	}

	// Init properties.
	list->sItemSize = ItemSize;
	list->sSize = 0;
	list->ppvItems = NULL;

	// Return list.
	return list;
}

void ListPush(list_N* list, void* item)
{
	// Increase list counter.
	list->sSize += 1;

	// Init list if empty.
	if (!list->ppvItems)
	{
		list->ppvItems = calloc(1, list->sItemSize);
	}
	else
	{
		// if not empty, make spcae and add item.
		list->ppvItems = (void**)realloc(list->ppvItems, list->sSize * list->sItemSize);
		if (list->ppvItems == NULL)
		{
			printf("NahWeek! Error 1 - [NahWeek : List] - memory allocation failed.\n");
			exit(1);
		}
	}
	list->ppvItems[list->sSize - 1] = item;
}