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

#include "linked_list.h"
#include "local.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
	srand(time(NULL));

    if (argc == 2)
	{
		int x,y,z;
        FILE* file = fopen(argv[1], "r");
		LocalNode* corner = NULL;
        while (!feof(file)) //read graph points into a graph
        {
            if (fscanf(file,"%d %d %d",&x,&y,&z))
            {
            	LocalNode* node = makeLocalNode(x,y,z);
				if (corner == NULL){
					corner = node;
				}
				else	{
					TwoDInsertSorted(corner, node);
				}
				//move to next line
				fscanf(file,"\n");
				corner = getCorner(corner);
            }
			else printf("READ ERROR");
        }
		fclose(file);
		linkUp(corner); //attach nodes to thier neighbours
		char l[] = "seq.txt";
    	FILE* f = fopen(l, "w"); //open the log file
		int xx,yy,zz;
		char yn = ' ';
		printf("Do you want to specify start point (default is southwest corner) y/n: ");
		while ((yn != 'y') && (yn != 'n')) //validate input
			scanf("%c", &yn);
		if (yn == 'y')
		{
			printf("input coordinates in the form <coord> <coord> (eg: \"23 11\"): ");
			scanf("%d %d", &xx,&yy);
			corner = findCoor(xx,yy,corner);
		}
		int max_shoulder = 0;
		printf("\nEnter max number of Shoulder Move: ");
		scanf("%d", &max_shoulder);
		LocalNode* max = findMax(corner, f, max_shoulder);
		printf("MAX Z VALUE: %d %d %d\n", max->x,max->y,max->z);
		fclose(f);
	}
    else
    {
        printf("\nUSAGE:\nstochastic-hill <mapfile>\n\n");
    }
}
//inserts a node into sorted position into a grid, start should be the south most, west most node
void TwoDInsertSorted(LocalNode *start, LocalNode *toInsert)
{
	LocalNode* iter = start;
	LocalNode* iterlast;
	//find the node's X position
	while((iter != NULL) && (iter->x < toInsert->x))
	{
		iterlast = iter;
		iter = iter->e; 
	}
	if(iter == NULL) //is the nodes x value larger than all other nodes x values?
	{
		iterlast->e = toInsert;
	}
	else if (iter->x == toInsert->x) //we've found the corrent x position, now look for the y position
	{
		if (iter->y > toInsert->y) //is the node the south most node for that x value?
		{
			toInsert->w = iter->w;	
			toInsert->e = iter->e;
			if (iter->w != NULL)
				iter->w->e = toInsert;
			if (iter->e !=NULL)
				iter->e->w = toInsert;
			iter->w = NULL;
			iter->e = NULL;
			toInsert->n = iter;
			iter->s = toInsert;
		}
		else
		{
			while ((iter !=NULL) && (iter->y <= toInsert->y))//find the node's Y postion
			{
				iterlast = iter;
				iter = iter->n;
			}
			if(iter == NULL) //is it's y bigger than all other nodes?
			{
				iterlast->n = toInsert;
				toInsert->s = iterlast;
			}
			else if (iter->y == toInsert->y) //two nodes shouldn't occupy the same xy, bad map file
			{
				//this is bad
				printf("Nodes share same x and y value. Can't make path a path outta that!");
			}
			else //y is between two other nodes of the same x
			{		
				toInsert->n = iter;
				toInsert->s = iter->s;
				iter->s->n = toInsert;
				iter->s = toInsert;
			}
		}
	}
	else //this node has the smallest x
	{		
		toInsert->e = iter;
		toInsert->w = iter->w;
		if(iter->w != NULL)
			iter->w->e = toInsert;
		iter->w = toInsert;
	}
}

//attaches nodes to their east/west node
void linkUp(LocalNode* start)
{
	LocalNode* left = start;
	LocalNode* right = start->e;
	while (right != NULL)
	{
		LocalNode* leftcol = left->n;
		LocalNode* rightcol = right->n;
		while ((leftcol != NULL) && (rightcol != NULL))
		{
			leftcol->e = rightcol;
			rightcol->w = leftcol;
			leftcol = leftcol->n;
			rightcol = rightcol->n;
		}
		left = right;
		right = right->e;
	}
}

//allocates a Node
LocalNode* makeLocalNode(int x, int y, int z)
{
	LocalNode* node = (LocalNode*)malloc(sizeof(LocalNode));
	node->x = x;
	node->y = y;
	node->z = z;
	node->w = NULL;
	node->s = NULL;
	node->n = NULL;
	node->e = NULL;
	return node;
}

//prints the entire graph in a pretty unreadable format
void printLocal(LocalNode* corner)
{
	while (corner != NULL)
	{
		LocalNode* curr = corner;
		while(curr != NULL)
		{
			printf("[%d,%d,%d], ", curr->x,curr->y,curr->z);	
			curr = curr->e;	
		}
		printf("\n\n");
		corner = corner->n;
	}
}

//prints a single node
void printnode(LocalNode* n)
{
	if (n !=NULL)
		printf("[%d %d %d]", n->x,n->y,n->z);
	else printf("[NULL]");
}

//find max will try to find the highest z value using stochastic hill climbing
LocalNode* findMax(LocalNode* node, FILE* log, int maxShoulders)
{
	int shoulder = 0;
	while(1)
	{
		//write the node to seq.txt
		writelog(node, log);
		fprintf(log,"\n");
		//put all the neighbours in an array (order doesn't matter)
		LocalNode* neighbours[4] = {node->e,node->s,node->w,node->n};
		int found = 0;
		int len = 4;	

		while ((len > 0) && !(found)) //if there is any nodes left in the array
		{
			int roll = rand()%(len);//pick a neighbour at random
			LocalNode* tmp = neighbours[roll];
			if (roll < len-1)//fill in the empty space left in the array
			{
				LocalNode* t = neighbours[len-1];
				neighbours[roll] = t;
			}
			neighbours[len-1] = NULL;
			len--;
			if (tmp != NULL)//is there a neighbour in that spot?
			{
				if(tmp->z > node->z) //is the neighbour bigger than the current z?
				{
					node = tmp;
					found = 1;
				}
				else if((tmp->z == node->z) && (shoulder < maxShoulders))//does the neighbour have the same z?
				{	
					node = tmp;	
					found = 1;
					shoulder++;
				}
			}
			if ((!found) && (len < 1)) //did we find a higher node? if not return.
				return node;
		}
	}
}

//writes a node to the log file
void writelog(LocalNode* n, FILE* log)
{
	fprintf(log,"%d %d %d",n->x,n->y,n->z);
	printf("%d %d %d\n", n->x,n->y,n->z);
	fflush(stdout);
}

//finds a code at a certain xy coordinate.
LocalNode* findCoor(int x, int y,LocalNode* node)
{
	while(node->x < x) //navigate to x coordinate
	{
		node = node->e;
	}
	if (node->x == x)
	{
		while(node->y < y)
		{
			node = node->n;
		}
		if (node->y == y)
			return node;
	}
	return NULL;
}
LocalNode* getCorner(LocalNode* node)
{
	while (node->s != NULL)
	{
		node = node->s;
	}
	while (node->w != NULL)
	{
		node = node->w;
	}
	return node;
}
