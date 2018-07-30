#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// Create structure
typedef struct node {
   char object[50];
   char question[50];
   struct node *yes_ptr;
   struct node *no_ptr;
} node_t;

int isObject(node_t *node);
node_t* treeRead(FILE *fp);
node_t* beginTreeRead();
void saveTree(node_t *ptr, FILE *fp);
void beginTreeSave(node_t *rootNode);

int isObject(node_t *node)
{
  return (node->yes_ptr == NULL && node->no_ptr == NULL);
}

node_t* treeRead(FILE *fp) 
{
  // Create buffers
  char buffer[255];
  char secondBuffer[255];

  // Allocate memory for new node
  node_t *nodeRead = malloc(sizeof(node_t));

  // Get next line
  if (fgets(buffer, 50, fp) != NULL)
  {
    // Get first character (Q or O)
    char type = buffer[0];

    // If a question
    if (type == 'Q') 
    {
      // Add question
      sscanf(buffer, "%c:%[^\n]", &type, secondBuffer);
      strncpy(nodeRead->question,secondBuffer,50);

      // Read YES and NO sub-trees
      nodeRead->yes_ptr = treeRead(fp);
      nodeRead->no_ptr  = treeRead(fp);
    } 
    // If a object
    if (type == 'O') 
    {
      // Add object
      sscanf(buffer, "%c:%[^\n]", &type, secondBuffer);
      strncpy(nodeRead->object,secondBuffer,50);

      nodeRead->yes_ptr = NULL;
      nodeRead->no_ptr  = NULL;
    }  
  }

  return nodeRead;
}

node_t* beginTreeRead()
{
  // Open data for reading
  FILE *fp = fopen("data.dat", "r");
  node_t *rootNode = NULL;

  // Get root node pointer
  rootNode = treeRead(fp);

  fclose(fp);

  return rootNode;
}

void saveTree(node_t *ptr, FILE *fp) 
{
  if (ptr == NULL)
  {
    return;
  }
  // If we reach a question
  else if (!isObject(ptr))
  {
      fprintf(fp, "Q:%s\n", ptr->question);

      // Save left and right sub-trees
      saveTree(ptr->yes_ptr, fp);
      saveTree(ptr->no_ptr, fp);

      // Free memory
      free(ptr);
  } 
  // If we reach a object
  else
  {   // Write object
      fprintf(fp, "O:%s\n", ptr->object);

      // Free memory
      free(ptr);
  }
}

void beginTreeSave(node_t *rootNode)
{
  // Wipe previous contents
  fclose(fopen("data.dat", "w"));

  // Open data for writing
  FILE *fp = fopen("data.dat", "w");

  // Save tree
  saveTree(rootNode, fp);

  fclose(fp);
}

int main(int argc, char **argv) 
{
  // Load tree
  node_t *root = beginTreeRead();

  printf("Welcome to the Genie! \n");

  // Set current node to root
  node_t *currentNode = root;
  node_t *previousNode = root;

  bool notFinished = true;

  while(notFinished)
  {
    char userResponse[20];

    // If we reach an object node
    if (currentNode->yes_ptr == NULL && currentNode->no_ptr == NULL)
    {
      printf("Are you thinking of a %s? ", currentNode->object);

      // Get user response
      fgets(userResponse,20,stdin);

      // Computer guesses right
      if (strcmp(userResponse,"Yes\n") == 0)
      {
        printf("Computer has won!\n");
        notFinished = false;

        beginTreeSave(root);
      }
      // Computer guesses wrong
      else if (strcmp(userResponse,"No\n") == 0)
      {
        char userObject[50];
        char userQuestion[50];

        printf("You beat the program! Please add a new object-name and question\n");

        // Get new object
        printf("What were you thinking of? ");
        fgets(userObject,20,stdin);

        // Get new question
        printf("What question could differentiate between a %s and a %s? ", userObject, currentNode->object);
        fgets(userQuestion,20,stdin);

        // Remove newline
        userQuestion[strcspn(userQuestion, "\n")] = 0;
        userObject[strcspn(userObject, "\n")] = 0;

        // Check for ending '?' for question
        if (userQuestion[strlen(userQuestion)] != '?')
        {
          // 2 extra characters for trailing 0, and new character itself
          char newQuestion[50];

          newQuestion[strlen(userQuestion)] = '?';
          strcpy(newQuestion, userQuestion);
        }    

        // If Q starting letter is LOWER CASE
        if (userQuestion[0] >= 'a' && userQuestion[0] <= 'z')
        {
          userQuestion[0] = toupper(userQuestion[0]);
          printf("Changed starting letter, new string is %s", userQuestion);
        }

        // Create new object
        node_t *newObj =  malloc(sizeof(node_t));
        strncpy(newObj->object, userObject, 50);
        newObj->yes_ptr = NULL;
        newObj->no_ptr = NULL;

        // Create new question
        node_t *newQuestion = malloc(sizeof(node_t));
        strncpy(newQuestion->question, userQuestion, 50);

        // Replace old object with new question
        // If we previously said 'yes' to get to current node
        if (previousNode->yes_ptr == currentNode)
        { 
          previousNode->yes_ptr = newQuestion;
        }
        // If we previously said 'no' to get to current node
        else
        {
          previousNode->no_ptr = newQuestion;          
        }

        // Yes pointer for new question
        newQuestion->yes_ptr = newObj;

        // No pointer for new question (set to old object)
        newQuestion->no_ptr = currentNode;

        notFinished = false;

        // Save tree to file
        beginTreeSave(root);
      }
      else
      {
        printf("What are you saying? Please type 'yes' or 'no'\n");
      }
    }
    else
    {
      printf("%s ", currentNode->question);

      // Get user response
      fgets(userResponse,20,stdin);

      // Parse user response
      if (strcmp(userResponse,"Yes\n") == 0)
      {
        // Go to left sub-tree
        previousNode = currentNode;
        currentNode = currentNode->yes_ptr;
      }
      else if (strcmp(userResponse,"No\n") == 0)
      {
        // Go to right sub-tree
        previousNode = currentNode;
        currentNode = currentNode->no_ptr;
      }
      else
      {
        printf("What are you saying? Please type 'Yes' or 'No' \n");
      }
    }
  }
}

void nodePrint(node_t* node)
{
  // Print node information
  printf("Object: %s \n", node->object);
  printf("Question: %s \n", node->question);
  printf("Yes: %s \n", node->yes_ptr->object);
  printf("No: %s \n", node->no_ptr->object);
}

void treePrint(node_t *ptr) 
{
  if (ptr == NULL)
  {
    return;
  }
  // If we reach a question
  else if (isObject(ptr) == 0)
  {
      printf("Question: \"%s\" \n", ptr->question);

      // Print the YES and NO sub-trees
      treePrint(ptr->yes_ptr);
      treePrint(ptr->no_ptr);
  } 
  // If we reach an object
  else 
  {   
      printf("Object: %s \n", ptr->object);
  }
}
