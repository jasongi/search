all: astar branch local
		
astar: graph.o linked_list.o astar.o
		gcc -o astar-search astar.o graph.o linked_list.o
branch: graph.o linked_list.o bnb.o
		gcc -o dyn-branch-search bnb.o graph.o linked_list.o
local: graph.o linked_list.o local.o
		gcc -o stochastic-hill local.o graph.o linked_list.o
graph.o:	graph.c graph.h linked_list.h
		gcc -c graph.c
bnb.o:	bnb.c bnb.h graph.h linked_list.h
		gcc -c bnb.c
local.o:	local.c local.h linked_list.h
		gcc -c local.c
astar.o:	astar.c astar.h graph.h linked_list.h
		gcc -c astar.c
linked_list.o:  linked_list.c linked_list.h
		gcc -c linked_list.c
clean:
		rm *.o astar-search dyn-branch-search stochastic-hill *~
