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
typedef struct LocalNode{
int x;
int y;
int z;
struct LocalNode* n;
struct LocalNode* s;
struct LocalNode* e;
struct LocalNode* w;
} LocalNode;

//find max will try to find the highest z value using stochastic hill climbing
LocalNode* findMax(LocalNode* node, FILE* log, int maxShoulders);

//allocates a Node
LocalNode* makeLocalNode(int x, int y, int z);

//attaches nodes to their east/west node
void linkUp(LocalNode* start);

//inserts a node into sorted position into a grid, start should be the south most, west most node
void TwoDInsertSorted(LocalNode *start, LocalNode *toInsert);

//Prints the xyz values of a LocalNode
void printLocal(LocalNode* corner);

//Finds the corner of a graph
LocalNode* getCorner(LocalNode* node);

//writes a node to the log file
void writelog(LocalNode* n, FILE* log);

//finds a code at a certain xy coordinate.
LocalNode* findCoor(int x, int y,LocalNode* node);
