#include "graph.h"
#include <stdbool.h>
#include <time.h>

/*

Dijkstra's algorithm requires a priority-first search. It contains a graph traversal 
 * algorithm which is similar to those you may have implemented in lab exercise 12, 
except you don't pull from your search list the most recently added (depth-first search) 
 * or the one which has been longest in the list (breadth-first search), 
but the one which is closest in distance to the starting node. Thus, you need to implement 
 * a priority queue. You will also need a priority queue for Lab 12, 
so do not embed it too deeply in the rest of the Dijkstra code.

This file has an additional heuristic, which uses the largest out-degree for making
decisions.
*/

int x;
void insert(List *list, int value)
{
  List *tempList = list;
  
  while (tempList->next != NULL)
    tempList = tempList->next;
  
  tempList->next = (List*)malloc(sizeof(List));
  tempList->next->index = value;
  tempList->next->next = NULL;
}

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
    shortestPath[i] = -1;
  
  shortestPath[source] = 0;
  
  List *list;
  List *tempList;
  
  int min;
  list = (List*)malloc(sizeof(List));
  list->index = 1;
  int size = mygraph.MaxSize;
  
  //Add nodes to list
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
    
    //nearest one in list
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
    
    //exit if we are down to our final node
    if (min == mygraph.MaxSize + 1)
      return shortestPath;
  }
  return shortestPath;
}

bool searchAdjacency(Node *current, Node *target)
{
  //printf("Called search adjacency \n");
  // Go into current node outlist
  List *list = current->outlist;
  if (list != NULL)
  {
    //printf("Search WHILE check is next instruct  \n");
    // While there are elements to search
    while (list->next != NULL)
    {
      // If found
      if (list->index == (int) target->name)
      {
        //printf("Search adjacency finished, returned TRUE \n");
        return true;
      }

      list = list->next;
    }
    //printf("Search adjacency finished, returned FALSE \n");
    return false;    
  }

  //printf("List is null \n");
  return false;
}

void printPath(List* path)
{
  // We use this index to show where in the path we are up to
  int index = 0;

  // While there are nodes in our path to print
  while (path != NULL)
  {
    // Print current node
    printf("At index %d, node index is %d \n", index, path->index);

    // Go to next node in the path
    path = path->next;

    // Increment our index
    index++;
  }
}

void sumList(List* inputList)
{
  // Stores our current total
  int cumulativeTotal = 0;

  // While there are elements not yet summed
  while(inputList != NULL)
  {
    // Add to cumulative total
    cumulativeTotal += inputList->index;

    // Go to next element
    inputList = inputList->next;
  } 
}

int getListSize(List* inputList)
{
  // Stores our current size
  int size = 0;

  // While there are elements not yet counted
  while (inputList != NULL)
  {
    // Increment size
    size++;

    // Go to next element
    inputList = inputList->next;
  }

  // Return size
  return size;
}

int searchValueInList(List *inputList, int toBeFound)
{
  // Used to iterate through our 'inputList'
  List *iterator = inputList;

  // While there are elements not yet counted
  while (iterator != NULL)
  {
    // If found
    if (iterator->index == toBeFound)
    {
      return 1;
    }

    // Go to next element in our iterator
    iterator = iterator->next;
  }

  // If not found
  return 0;  
}

void addToList(List* inputList, int toBeInserted)
{
  // Used for indexing our input list
  List *posNext, *posCurrent;

  // Create structure to insert at appropriate position
  posNext = (List*)malloc(sizeof(inputList));
  posNext->index = toBeInserted;
  posNext->next = NULL;

  // If list is empty
  if(inputList == NULL)
  {
    // Add to 1st index
    inputList = posNext;
  }
  else
  {
    // Go to start of the inputted list
    posCurrent = inputList;

    while(posCurrent->next != NULL)
    {
      posCurrent = posCurrent->next;
    }

    // Add to the end of the list
    posCurrent->next = posNext;
  }
}

int performHeuristic(Graph* myGraph, int source, int target)
{
  // ENTIRE PSEUDO-CODE
  //   CURRENT ← SOURCE;
  // while (TARGET not in CURRENT.OUTLIST) and (CURRENT.OUTLIST not empty)
  // add CURRENT to PATH
  // Let MAXOUT be the unvisited node in CURRENT.OUTLIST with largest out-degree
  // CURRENT ← MAXOUT;
  // endwhile
  // add CURRENT to PATH
  // Look for largest out-degree

  // Store our source node seperately for processing

  //   CURRENT ← SOURCE;
  int current = source;

  // Stores our final path
  List *path = (List*)malloc(sizeof(List));

  // Index of our (possibly next) path to take
  int maxOutDegreeIndex;

  // while (TARGET not in CURRENT.OUTLIST) and (CURRENT.OUTLIST not empty)
  while (!searchValueInList(myGraph->table[current].outlist, target)  
            && myGraph->table[current].outlist != NULL)
  {
    // Add CURRENT to PATH
    addToList(path, current);
    myGraph->table[current].visited = 1;

    // Let MAXOUT be the unvisited node in CURRENT.OUTLIST with largest out-degree
    List *currentOutlist = (List*)malloc(sizeof(List));
    currentOutlist = myGraph->table[current].outlist;
    maxOutDegreeIndex = 0;

    // Get largest out-degree (our heuristic)
    while (currentOutlist != NULL)
    {
      // If we find a higher out-degree AND we have no visited this node before
      if (myGraph->table[currentOutlist->index].outdegree > myGraph->table[maxOutDegreeIndex].outdegree 
       && myGraph->table[currentOutlist->index].visited == 0)
      {
        // Update max degree index
        maxOutDegreeIndex = currentOutlist->index;
      }
      
      // Go to next element
      currentOutlist = currentOutlist->next;
    }

    // CURRENT ← MAXOUT;
    current = maxOutDegreeIndex;
  }

  // We are now 1 hop away from finding our target
  addToList(path, current);

  // Return the size
  return getListSize(path);
}

int main(int argc,char *argv[])
{
  // Pre-processing
  Graph mygraph;
  read_graph(&mygraph,argv[1]);

  // Start the clock
  clock_t clockBegin = clock();

  // Declare / initialise variables
  int max_distance = 0;
  int distance;
  int totalHops = 0;

  for (int targetNode = 1; targetNode < mygraph.MaxSize; targetNode++)
  {
    for (int j = 1; j < mygraph.MaxSize; j++) 
    {
      // Middle argument is 1 because we start from source node 1
      distance = performHeuristic(&mygraph, 1, targetNode);

      // If we find a bigger 'max distance'
      if (distance > max_distance)
      {
        max_distance = distance;
      }
      
      // Add distance to our cumulative count of the 'hops'
      totalHops += distance;
    }
  }
    
  // End clock
  clock_t clockFinish = clock();
  printf("Maximum Distance (Hops): %d\n", max_distance);
  printf("Average Degree: %f\n", (double) totalHops / ((mygraph.MaxSize - 1) * (mygraph.MaxSize - 1)));
  printf("Time Taken: %f\n", (double) (clockFinish - clockBegin) / CLOCKS_PER_SEC);

  return(0);
}