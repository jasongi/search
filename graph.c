#include "linked_list.h"
#include "graph.h"
#include "bnb.h"
#include "astar.h"
#include "local.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
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
LinkedList* readNodeFile(char* filename)
{
        LinkedList* list = createList();
        FILE* file = fopen(filename, "r");
        char a[10]; 
		char b[10];
        double weight;
		//read the node info file
        while (!feof(file))
        {
            if (fscanf(file,"%s %s %lf\n",a,b,&weight))
            {
                addedge(a,b,weight,list);     
            }
        }
		fclose(file);
		return list;
}

//this frees a graph
void freegraph(LinkedList* list)
{
	LinkedListNode* tmp = list->head;
	while (tmp != NULL)
	{
		//free the list of connected nodes
		freeList(((GraphNode*)(tmp->data))->connectedNodes);
		tmp = tmp->next;
	}
	//free the list of GraphNodes
	freeList(list);
}
	

//this function finds (or creates if they don't exist) the two nodes and then adds each node to each other's ConnectedNodes list
void addedge(char* a,char* b,int weight, LinkedList* list)
{
	//find (or create) node 'a'
    GraphNode* anode = findNode(a, list);
	//find (or create) node 'b'
    GraphNode* bnode = findNode(b, list);
	//add the nodes to each other's list of connected nodes.
	addvert(anode,bnode,weight);

}

//adds each node to the other's connectedNodes list 
int addvert(GraphNode* a, GraphNode* b,int weight)
{
	LinkedListNode* tmpnode = a->connectedNodes->head;
	//see if node b is in a's list of connected node (if so we exit)
	while (tmpnode != NULL)
	{
		if (!strcmp(((GraphNode*)(((ConnectedNode*)(tmpnode->data))->node))->name, b->name))
			return 0;
		tmpnode = tmpnode->next;
	}
	ConnectedNode* conNode = (ConnectedNode*)malloc(sizeof(ConnectedNode));
	conNode->node = b;
	conNode->weight = weight;
	insertLast(a->connectedNodes, (void*)conNode);
	(a->connectedNodes->len)++;
	addvert(b,a,weight);
	static int x = -1;
	return 1;
}

//returns the GraphNode in the list with the name otherwise returns NULL
GraphNode* nodeSearch(char* name, LinkedList* list)
{
    LinkedListNode* currnode = list->head;
	GraphNode* graphnode = NULL;
    while ((currnode != NULL) && (graphnode == NULL))
    {
        if (!strcmp(((GraphNode*)(currnode->data))->name, name))
		{
			graphnode = (GraphNode*)(currnode->data);
		}
        else
            currnode = currnode->next;
    }
	return graphnode;
}

//finds the GraphNode in the list, and if it doesn't exist, creates one with that name
GraphNode* findNode(char* name, LinkedList* list)
{
    LinkedListNode* currnode = list->head;
	GraphNode* graphnode = nodeSearch(name, list);
	//if the node can't be found, allocate a node for it
    if (graphnode == NULL)
    {
        graphnode = makeGraphNode(name);
        insertLast(list, graphnode);
    }
	return graphnode;
}

//allocates memory for a node and sets default values as well as the name
GraphNode* makeGraphNode(char* name)
{
    GraphNode *node = (GraphNode*)malloc(sizeof(GraphNode));
    strcpy(node->name,name);
    node->connectedNodes = createList();
	node->lowestCost = -1;
	node->h = 0;
    return node;
}

//prints the tree (for debug purposes)
void printTree(LinkedList* graph)
{
	LinkedListNode* node = graph->head;
	while (node != NULL)
	{
		printf("%s", ((GraphNode*)(node->data))->name);
		LinkedListNode* conNode = ((LinkedList*)((GraphNode*)(node->data))->connectedNodes)->head;
		while (conNode != NULL)
		{
			printf(" %s(%lf)(%lf)",((ConnectedNode*)(conNode->data))->node->name, ((ConnectedNode*)(conNode->data))->weight,((ConnectedNode*)(conNode->data))->node->h);
			conNode = conNode->next;
		}
		node = node->next;
		printf("\n");
	}
}

//inserts a Partial Path into a list in it's sorted position (sorted by costtonode/f-cost (depending on algorithm)) Insertion sort for each insertion should be O(N).
void insertSorted(LinkedList* list, Solution* sol)
{
	LinkedListNode* node = list->head;
	LinkedListNode* replace; //spot to replace
	int foundspot = 0;
	while ((node != NULL) && (!foundspot))
	{
		if ( ((Solution*)(node->data))->cost > sol->cost)
		{

			foundspot = 1;
			replace = node->prev;
            //if it's the smallest node
			if (replace == NULL)
				insertFirst(list,sol);
			else
			{
            //if it is in the middle somewhere
				LinkedListNode *newLLN = (LinkedListNode*)malloc(sizeof(LinkedListNode));
				newLLN->data = (void*)sol;
				newLLN->next = replace->next;
				newLLN->prev = replace;
				replace->next = newLLN;
				newLLN->next->prev = newLLN;
                (list->len)++;
			}
		}
		node = node->next;
	}
    //if it is the largest node
	if (!foundspot)
		insertLast(list, sol);
}

//prints out a partial solution (or complete solution, it doesn't discriminate)
void printSolution(Solution* sol)
{
	LinkedListNode* node = sol->nodes->head;
	printf("\n");
	while (node != NULL)
	{
		printf("%s", ((GraphNode*)node->data)->name);
		node = node->next;
		if (node != NULL)
			printf("->");
	}
	printf(" cost: %lf", sol->cost);
}

//checks if the solution is at the goal node
int goal(Solution* sol, char* name)
{
	if ((strcmp((((GraphNode*)(sol->nodes->tail->data))->name), name)) == 0)
		return 1;
	return 0;
}
