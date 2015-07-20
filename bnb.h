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

//solves a graph using the branch and bound method
Solution* branchandbound(char* startnode,char* endnode,LinkedList* list);

//helper function for branch and bound which expands the neighbours
void checkneighbours(Solution* solution,LinkedList* solList);
