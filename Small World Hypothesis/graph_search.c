#include "graph.h"

void DFS(Graph *graph, int i)
{
	// Go to node specified by index
  List *listPointer;
  listPointer = graph->table[i].outlist;

  printf("\n Node %d \n", i);

  // Store that we have visited this node now
  graph->table[i].visited = 1;

  // If outlist is not null
  while(listPointer != NULL)
  {
    i = listPointer->index;
     
    // If we haven't yet visited
    if(graph->table[i].visited == 0)
    {
      // Recursive call
      DFS(graph, i);	
    }

    // Go to next in list
    listPointer = listPointer->next;
  }
}

// PUT BFS BELOW