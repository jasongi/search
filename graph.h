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
//GraphNode represents a single node on a graph.
typedef struct GraphNode{
char name[10];
double lowestCost;
double h;
LinkedList* connectedNodes;
} GraphNode;

//Solution represents a collection of GraphNodes which are connected to each other. Cost is either the sum of link weights up to the last node (if using BnB) or the f-cost if using A*. costtonode is the sum of link weights when A* is being used.
typedef struct Solution{
double cost;
double costtonode;
LinkedList* nodes;
} Solution;

//ConectedNode represents nodes that are connected to a graphnode.
typedef struct ConnectedNode{
GraphNode *node;
double weight;
} ConnectedNode;
//Reads a node file into a graph structure
LinkedList* readNodeFile(char* filename);
//this function finds (or creates if they don't exist) the two nodes and then adds each node to each other's ConnectedNodes list
void addedge(char* a,char* b,int weight, LinkedList* list);

//finds the GraphNode in the list, and if it doesn't exist, creates one with that name
GraphNode* findNode(char* name, LinkedList* list);

//allocates memory for a node and sets default values as well as the name
GraphNode* makeGraphNode(char* name);

//adds each node to the other's connectedNodes list 
int addvert(GraphNode* a, GraphNode* b,int weight);

//prints the tree (for debug purposes)
void printTree(LinkedList* graph);

//inserts a Partial Path into a list in it's sorted position (sorted by costtonode/f-cost (depending on algorithm)) Insertion sort for each insertion should be O(N).
void insertSorted(LinkedList* list, Solution* sol);

//returns the GraphNode in the list with the name otherwise returns NULL
GraphNode* nodeSearch(char* name, LinkedList* list);

//prints out a partial solution (or complete solution, it doesn't discriminate)
void printSolution(Solution* sol);

//checks if the solution is at the goal node
int goal(Solution* sol, char* name);

//frees a graph
void freegraph(LinkedList* list);
