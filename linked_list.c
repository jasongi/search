#include "linked_list.h"
/*  
    Copyright (C) 2014 Jason Giancono (jasongiancono@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
*/

LinkedList *createList()
{
	//allocate a new list and set all the NULL
	LinkedList *newLL = (LinkedList*)malloc(sizeof(LinkedList));
	newLL->head = NULL;
	newLL->tail = NULL;
    newLL->len = 0;
	return newLL;
}

void insertLast(LinkedList *lL, void *inData)
{
	//allocate a new node
	LinkedListNode *newLLN = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	//set the data pointer to point to the item
	newLLN->data = inData;
    newLLN->next = NULL;
	//check if the list is empty
	if (lL->head == NULL)
	{
		lL->head = newLLN;
		lL->tail = newLLN;
        newLLN->prev = NULL;
	}
	else
	{
        lL->tail->next = newLLN;
        newLLN->prev = lL->tail;
		lL->tail = newLLN;        
	}
    (lL->len)++; //increase the couter
}

void insertFirst(LinkedList *lL, void *inData)
{
	//allocate a new node
	LinkedListNode *newLLN = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	//set the data pointer to point to the item
	newLLN->data = inData;
    newLLN->next = lL->head;
    newLLN->prev = NULL;
	//check if the list is empty
	if (lL->tail == NULL)
	{
		lL->head = newLLN;
		lL->tail = newLLN;
	}
	else
	{
        lL->head->prev = newLLN;
		lL->head = newLLN;        
	}
    (lL->len) = lL->len + 1; //increase the couter
}
void* softRemoveLast(LinkedList *lL)
{
	LinkedListNode *tempPtr = lL->head;
	if (tempPtr == NULL)
	    return NULL; //only happens if list is empty. Is bad.
    void* data = tempPtr->data;
	if (tempPtr == lL->tail) 
	{
		//if there is only one element, set both head/tail to null
		free(tempPtr); //free the node but not the data
		lL->head = NULL;
		lL->tail = NULL;
	}
	else
	{
		lL->head = tempPtr->next;
        lL->head->prev = NULL;
		free(tempPtr); //free the node but not the data
	}
    (lL->len)--; //decrement the counter
	return data;
}
void* softRemoveFirst(LinkedList *lL)
{

	LinkedListNode *tempPtr = lL->head;
	if (tempPtr == NULL)
        return NULL; //only happens when the node 
	void* data = tempPtr->data;
	if (tempPtr == lL->tail)
	{

		free(tempPtr);
		lL->head = NULL;
		lL->tail = NULL;
	}
	else
	{

		lL->head = tempPtr->next;
		if (tempPtr->next != NULL)
		    lL->head->prev = NULL;
		free(tempPtr);
	}

    (lL->len)--;
	return data;
}
void removeFirst(LinkedList *lL)
{
	LinkedListNode *tempPtr = lL->head;
	if (tempPtr != NULL) //check if there is anything to remove
	{
		if (tempPtr == lL->tail)
		{
			free(tempPtr->data);
			free(tempPtr);
			lL->head = NULL;
			lL->tail = NULL;
		}
		else
		{
			lL->head = tempPtr->next;
		    lL->head->prev = NULL;
			free(tempPtr->data);
			free(tempPtr);
		}
		(lL->len)--;
	}
}

void removeLast(LinkedList *lL)
{
	LinkedListNode *tempPtr = lL->tail;
	if (tempPtr != NULL) //check if there is anything to remove
	{
		if (tempPtr == lL->head)
		{
			free(tempPtr->data);
			free(tempPtr);
			lL->head = NULL;
			lL->tail = NULL;
		}
		else
		{
			lL->tail = tempPtr->prev;
		    lL->tail->next = NULL;
			free(tempPtr->data);
			free(tempPtr);
		}
		(lL->len)--;
	}
}

void softFreeList(LinkedList *lL)
{
	LinkedListNode *tempPtr;
	tempPtr = lL->head;

	//soft free each element
	while (tempPtr != NULL)
	{
		softRemoveFirst(lL);
		tempPtr= lL->head;
	}

	free(lL); //soft free the list
}


void freeList(LinkedList *lL)
{
	LinkedListNode *tempPtr;
	tempPtr = lL->head;
	//free each element
	while (tempPtr != NULL)
	{
		removeFirst(lL);
		tempPtr= lL->head;
	}
	free(lL); //free list
}
