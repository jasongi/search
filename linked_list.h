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
#ifndef IO
#include <stdio.h>
#define IO
#endif

#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

typedef struct LinkedListNode{
void *data;
struct LinkedListNode *next; 
struct LinkedListNode *prev;
} LinkedListNode;

typedef struct LinkedList{
int len; //number of items in the list
LinkedListNode *head;
LinkedListNode *tail; 
} LinkedList;

//mallocs a new list
LinkedList *createList();

//inserts item at the end of a list
void insertLast(LinkedList *lL, void *data);

//inserts item at the start of a list
void insertFirst(LinkedList *lL, void *inData);
//removes and frees the first item of a list
void removeFirst(LinkedList *lL);

//removes and frees the last item of a list
void removeLast(LinkedList *lL);

//removes but does not free first item of a list
void* softRemoveFirst(LinkedList *lL);

//returns N element from the list & frees the node
void *returnNElement(LinkedList *lL, int n);

//frees the List and the data pointer (watch out for mem leaks)
void freeList(LinkedList *lL);


