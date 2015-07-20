#include "linked_list.h"
#include "graph.h"
#include "astar.h"
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
void readHeuristics(char* filename, LinkedList* list)
{
		char a[10];
		double weight = 0;
		FILE* file = fopen(filename,"r");
		fscanf(file,"Node_ID Distance_to_Goal\n\n");
        while (!feof(file))
        {
            if (fscanf(file,"%s %lf",a,&weight))
            {
				//find the node whose heuristic is listed
            	GraphNode* node = nodeSearch(a,list);
				//set the heuristic
				node->h = weight;
				//move to next line
				fscanf(file,"\n");
            }
        }
		fclose(file);
}
int main(int argc, char** argv)
{
	srand(time(NULL));
    if (argc == 3)
    {
		LinkedList* list = readNodeFile(argv[1]);
		//read the heuristic file
		readHeuristics(argv[2],list);


		char aa[10];
		char bb[10];
		printf("Enter Start Node: ");
		scanf("%s", aa);
		printf("Enter End Node: ");
		scanf("%s", bb);
		Solution* sol;
		sol = astar(aa,bb,list);
		freegraph(list);
		softFreeList(sol->nodes);
		free(sol);
    }
    else
    {
        printf("\nUSAGE:\nastar-search <node file> <heuristic file>\n\n");
    }

}

Solution* astar(char* startnode,char* endnode,LinkedList* list)
{
    GraphNode* start = findNode(startnode, list);
    //check to see if the start node actually exists
    if(start != NULL)
    {
        //create a partial path for the start node
        LinkedList* solList = createList();
        Solution* startsol = (Solution*)malloc(sizeof(Solution));
        startsol->cost = start->h;
        startsol->costtonode = 0;
        startsol->nodes = createList();
        insertLast(startsol->nodes,start);
        insertSorted(solList,startsol);
        Solution* lastGoalPath = NULL; //this will be the final solution
        //explore neighbours
        while(1)
        {
            //take the path with the fcost off the top of the list
            Solution* lowest = (Solution*)(softRemoveFirst(solList));
            //if it's null that means we've exhausted the possible paths, so we quit. This shouldn't happen because this A* implementation revisits past nodes unlike the Branch and Bound one.
            if (lowest == NULL)
            {
                printf("\nGraph fully explored\n");
                freeList(solList);
                return lastGoalPath;
            }
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
			//add each of it's neighbours to a new partial path and add to solution list
            astar_checkneighbours(lowest,solList);
			//free the partial path
            softFreeList(lowest->nodes);
            free(lowest);
        }
    }
}

void astar_checkneighbours(Solution* solution, LinkedList* solList)
{
	//get the first neighbour
    LinkedListNode* neighbour = ((GraphNode*)(solution->nodes->tail->data))->connectedNodes->head;
    while (neighbour != NULL)
    {
        Solution* sol = (Solution*)malloc(sizeof(Solution));
        //costtonode is the weight of the links up until this node, cost is the costtonode + heuristic
        sol->costtonode = solution->costtonode + ((ConnectedNode*)neighbour->data)->weight;
        sol->cost = sol->costtonode + ((GraphNode*)((ConnectedNode*)neighbour->data)->node)->h ;
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


