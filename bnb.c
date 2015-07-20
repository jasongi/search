#include "linked_list.h"
#include "graph.h"
#include "bnb.h"
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

int main(int argc, char** argv)
{
	srand(time(NULL));
    if (argc == 2)
    {
		LinkedList* list = readNodeFile(argv[1]);
		char aa[10];
		char bb[10];
		printf("Enter Start Node: ");
		scanf("%s", aa);
		printf("Enter End Node: ");
		scanf("%s", bb);
		Solution* sol = branchandbound(aa,bb,list);
		freegraph(list);
		softFreeList(sol->nodes);
		free(sol);
    }
    else
    {
        printf("\nUSAGE:\ndyn-branch-search <node file>\n\n");
    }
}

Solution* branchandbound(char* startnode,char* endnode,LinkedList* list)
{
	GraphNode* start = findNode(startnode, list);
    //check to see if the start node actually exists
	if(start != NULL)
	{
        //create a partial path for the start node
		LinkedList* solList = createList();
		Solution* startsol = (Solution*)malloc(sizeof(Solution));
		startsol->cost = 0;
		startsol->nodes = createList();
		insertLast(startsol->nodes,start);
		insertSorted(solList,startsol);
        Solution* lastGoalPath = NULL; //this will be the final solution
        //explore neighbours
		while(1)
		{
			//take the path with the lowest cost off the top of the list
			Solution* lowest = (Solution*)(softRemoveFirst(solList));
			//if it's null that means we've exhausted the possible paths, so we quit
            if (lowest == NULL)
            {
                printf("\nGraph fully explored\n");
				freeList(solList);
                return lastGoalPath;
            }
			//grab the end node of the path (the one we will expand)
			GraphNode* curnode = ((GraphNode*)(lowest->nodes->tail->data));
            printSolution(lowest);
			//if it is the goal, ask to continue or quit
			if (goal(lowest,endnode))
            {
				lastGoalPath = lowest;
                char option = 'A';
                printf("\n\nQ to exit, C to find alt path: ");
                fflush(stdin);
                while((option != 'C') && (option !='Q'))
                    scanf("%c", &option);
                if (option == 'Q')
				{
					LinkedListNode* tmp = solList->head;
					while(tmp != NULL)
					{
						softFreeList(((Solution*)(tmp->data))->nodes);
						tmp=tmp->next;
					}
					freeList(solList);
                    return lastGoalPath;
				}
            }
			//check to see if node has been visited before
			if ((curnode->lowestCost > lowest->cost) || (curnode->lowestCost == -1))
			{
				curnode->lowestCost = lowest->cost;
				//add each of it's neighbours to a new partial path and add to solution list
				checkneighbours(lowest, solList);
			}
			else
				printf(" Path Discarded");
			//free the partial path
			softFreeList(lowest->nodes);
			free(lowest);
		}
	}
}
//this function adds creates a new path for each node connected to the end node of a partial solution
void checkneighbours(Solution* solution, LinkedList* solList)
{
	//get the first neighbour
	LinkedListNode* neighbour = ((GraphNode*)(solution->nodes->tail->data))->connectedNodes->head;
	while (neighbour != NULL)
	{
		Solution* sol = (Solution*)malloc(sizeof(Solution));
		sol->cost = solution->cost + ((ConnectedNode*)neighbour->data)->weight;
		sol->nodes = createList();
		//copy the partial path into the new one		
		LinkedListNode* tmpnode = solution->nodes->head;
		while(tmpnode != NULL)
		{
				insertLast(sol->nodes,((GraphNode*)tmpnode->data));
				tmpnode = tmpnode->next;
		}
		//insert the neighbour node at the end of the partial path
		insertLast(sol->nodes,((ConnectedNode*)neighbour->data)->node);
		//insert into sorted position
		insertSorted(solList,sol);
		neighbour = neighbour->next;
	}
}


