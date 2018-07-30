#include "graph.h"
#include <stdbool.h>
#include <time.h>

/*
Dijkstra's algorithm for testing the small-world hypothesis, with use of a priority queue.
*/

int x;

/*
Insert a new value into the priority queue
*/
void insert(List *list, int value)
{
  List *tempList = list;
  
  while (tempList->next != NULL)
    tempList = tempList->next;
  
  tempList->next = (List*)malloc(sizeof(List));
  tempList->next->index = value;
  tempList->next->next = NULL;
}

/*
Remove a value from the priority queue
*/
List* removeP(List *list, int value)
{
  List *tempList = list;
  
  if (tempList->index == value)
    return list->next;
  
  while (tempList->next->next != NULL)
  {
    if (tempList->next->index == value)
    {
      tempList->next = tempList->next->next;
      return list;
    }
    tempList = tempList->next;
  }
          
  if (tempList->next->index == value)
    tempList->next = NULL;
  
  return list;
}

/*
Returns a boolean integer indicating whether a value exists in the
priority queue
*/
int inQueueList(List *list, int value)
{
  List *tempList = list;
  
  if (tempList->index == value)
    return 1;
  
  while (tempList != NULL)
  {
    if (tempList->index == value)
      return 1;
    
    tempList = tempList->next;
  }
  
  return 0;
}


int* findTheShortestPath(Graph mygraph, int source)
{
  int *shortestPath = (int*)malloc((mygraph.MaxSize) * sizeof(int));
  
  for (int i = 1; i < mygraph.MaxSize; i++)
  {
    shortestPath[i] = -1;
  }
  
  // The distance from the source node, to the source node, is 0
  shortestPath[source] = 0;
  
  List *list;
  List *tempList;
  list = (List*)malloc(sizeof(List));
  list->index = 1;
  int min;
  int size = mygraph.MaxSize;

  // Add nodes to list
  for (int i = 2; i < size; i++)
     insert(list, i);
  
  size -= 1;
  while (size > 0)
  {
    for (int j = 0; j < mygraph.table[source].outdegree; j++)
    {
      if (j == 0)
        tempList = mygraph.table[source].outlist;
      else
        tempList = tempList->next;
      if (inQueueList(list, tempList->index) == 1)
      {
        if (shortestPath[tempList->index] == -1)
          shortestPath[tempList->index] = shortestPath[source] + 1;
        if (shortestPath[tempList->index] > shortestPath[source] + 1)
          shortestPath[tempList->index] = shortestPath[source] + 1;
      }
    }

    list = removeP(list, source);
    size--;
    
    // Nearest one in list
    tempList = list;
    min = mygraph.MaxSize + 1;
    
    for (int i = 1; i <= size; i++)
    {
      if (shortestPath[tempList->index] < min && shortestPath[tempList->index] > 0)
      {
        source = tempList->index;
        min = shortestPath[tempList->index];
      }
      tempList = tempList->next;
    }
    
    // If we have reached the final node
    if (min == mygraph.MaxSize + 1)
      return shortestPath;
  }
  return shortestPath;
}

int main(int argc,char *argv[])
{
  // Read graph file path given as argument, into memory
  Graph mygraph;
  read_graph(&mygraph,argv[1]);

  // Start the clock
  clock_t clockBegin = clock();
  
  mygraph.MaxSize -= 1;
  int *p;
  int avg = 0;
  int s = 0;
  int max = 0;

  // For each node in the graph
  for (int i = 1; i < mygraph.MaxSize; i++)
  {
    s = 0;
    
    // Find the shortest path
    p = findTheShortestPath(mygraph, i);
    
    // If a new shortest path found, from a given node from the source
    // store it
    for (int j = 1; j < mygraph.MaxSize; j++)
    {
      if (p[j] > max)
      max = p[j];
      s += p[j];
    }
    
    avg += s / (mygraph.MaxSize - 1);
  }

  // End clock
  clock_t clockFinish = clock();

  // Print results to the user
  printf("Maximum degree: %d\n", max);
  printf("Average degree: %f\n", (double)avg / (mygraph.MaxSize - 1));
  printf("Time Taken: %f\n", (double) (clockFinish - clockBegin) / CLOCKS_PER_SEC);

  return(0);
}