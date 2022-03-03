/*Written by: Dave Dominique */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

#define MAXLEN 30

typedef struct itemNode
{
 char name[MAXLEN];
 int count;
 struct itemNode *left, *right;
}itemNode;

typedef struct treeNameNode
{
 char treeName[MAXLEN];
 struct treeNameNode *left, *right;
 itemNode *theTree;
}treeNameNode;

//phase1 BUILD TREENAMENODES
  

    treeNameNode* createTreeNameNode(char treeName[])
    {
      treeNameNode* nameNode = malloc(sizeof(treeNameNode));
      strcpy(nameNode->treeName, treeName);
      nameNode->left = NULL;
      nameNode->right = NULL;
      nameNode->theTree = NULL;

      return nameNode;
    }

    treeNameNode* buildNameTree(FILE* infile, treeNameNode* root, char treeName[])
    {
      if (root == NULL)
      {
        root = createTreeNameNode(treeName);
      } 
      
      else
      {
        if (strcmp(root->treeName, treeName) < 0) // we want to go right
        {
          if(root->right == NULL)
          {
            root->right = createTreeNameNode(treeName);
          }
          else
            buildNameTree(infile, root->right, treeName);
        }

        else //we want to go left
        {
          if(root->left == NULL)
          {
            root->left = createTreeNameNode(treeName);
          }
          else
            buildNameTree(infile, root->left, treeName);
        }
      }
      
      return root; 
    }




//phase 2 BUILD ITEMTREES
    itemNode* createItemNode(char name[], int count)
    {
      itemNode* item = malloc(sizeof(itemNode));
      strcpy(item->name, name);
      item->count = count;

      return item;
    }


    itemNode* addItem(FILE* infile, itemNode* root, char name[], int count)
    {
      if(root == NULL)
          {
            root = createItemNode(name, count);
          
          }
          
          else // if its not null we want to go left or right based on the itemnodes name;
          {

            if(strcmp(root->name, name) < 0) // go right
            {
              if(root->right == NULL)
              {
                root->right = createItemNode(name, count);
              }
              
              else
              {
                addItem(infile, root->right, name, count);
              }
            }

            else // go left
            {
            if(root->left == NULL)
              {
                root->left = createItemNode(name, count);
              }
              
              else
              {
                addItem(infile, root->left, name, count);
              }
            }
          }

      return root;
    }

    treeNameNode* addtoTree(FILE* infile, treeNameNode* root, char theTree[], char name[], int count)
    {
      if(root == NULL)
        return NULL;

      if (strcmp(root->treeName, theTree) == 0) // there is a match in names
        {
          root->theTree = addItem(infile, root->theTree, name, count); 
        }
      

      else //there isnt a match in names
      {
        if (strcmp(root->treeName, theTree) < 0) // we want to go right
        {
          addtoTree(infile, root->right, theTree, name, count);
        }

        else //we want to go left
        {
          addtoTree(infile, root->left, theTree, name, count);
        }
      }

      return root;
    }


    void printNamesTreeinorder(FILE* outfile, treeNameNode*root)
    {
      //check case for NULL
      if(root ==  NULL)
      {
        return;
      }
      else
      {
        printNamesTreeinorder(outfile, root->left);
        
        printf("%s ", root->treeName);
        fprintf(outfile, "%s ", root->treeName );
            
        printNamesTreeinorder(outfile, root->right);
      }
    }

    void printItems(FILE* outfile,itemNode*root)
    {
      //check case for NULL
      if(root ==  NULL)
      {
        return;
      }

      else
      {
        printItems(outfile,root->left);
        
        printf("%s ", root->name);
        fprintf(outfile, "%s ", root->name );
            
        printItems(outfile,root->right);
      }
    }

    void printFromNames(FILE* outfile, treeNameNode* root)
    {
      if (root == NULL)
        return;
      
      else
      {
        
        printFromNames(outfile, root->left);

        printf("\n===%s===\n", root->treeName);
        fprintf(outfile, "\n===%s===\n", root->treeName );

        printItems(outfile, root->theTree);

        printFromNames(outfile, root->right);
      }  
      
    }

    void traverse_in_traverse(FILE* outfile, treeNameNode *root)
    {
      printNamesTreeinorder(outfile, root);
      printFromNames(outfile, root);
    }

    treeNameNode * searchNameNode(treeNameNode * root, char treeName[50])
    {
      if(root != NULL)
      {
        if (strcmp(root->treeName, treeName) == 0) //found a match
        {
          return root;
        }

        if (strcmp(root->treeName, treeName) < 0) // we want to go right)
        {
          return searchNameNode(root->right, treeName);
        }

        else //we want to go left
        {
          return searchNameNode(root->left, treeName);
        }

      }
      
      return NULL;
    }




//phase 3 BUILD QUERIES
  //SEARCH
    void searchItem(FILE* outfile, treeNameNode* ptr , itemNode* root, char item[])
    {
      if(root != NULL)
      {
        if (strcmp(root->name, item) == 0) //found a match
        {
          printf("%d %s found in %s \n",root->count, root->name, ptr->treeName);
          fprintf(outfile, "%d %s found in %s \n",root->count, root->name, ptr->treeName );
          return;
        }

        if (strcmp(root->name, item) < 0) // we want to go right)
        {
          return searchItem(outfile, ptr, root->right, item);
        }

        else //we want to go left
        {
          return searchItem(outfile, ptr, root->left, item);
        }
      }

      printf("%s not found in %s \n", item, ptr->treeName);
      fprintf(outfile,"%s not found in %s \n", item, ptr->treeName  );
      return;
    }


    void search(FILE* outfile, treeNameNode* root, char treeName[50], char item[])
    {
      root = searchNameNode(root, treeName);

      if(root != NULL)
      {
        searchItem(outfile, root, root->theTree, item);
      }

      else
       {
         printf("%s does not exist\n", treeName);
        fprintf(outfile, "%s does not exist\n", treeName);
       } 
    }





  
  //ITEM_BEFORE
    void beforeOrder(FILE* outfile, treeNameNode* ptr , itemNode* root, char item[], int move)
    { 
        move++; 
        //check case for NULL
        if(root ==  NULL)
        {
          return;
        }

        else 
        {
          beforeOrder(outfile, ptr, root->left, item, move); 
          
          beforeOrder(outfile, ptr, root->right, item, move);
          

          if(strcmp(root->name, item)==0)
            {
              move++;
              printf("%d \n", move);
              fprintf(outfile, "%d \n", move );
              return; 
            }
        }
        
      return;
    }

    void item_before(FILE* outfile, treeNameNode* root, char treeName[50], char item[])
    {
      int move = 0;
      
      root = searchNameNode(root, treeName);

      printf("item before %s: ", item);
      fprintf(outfile,"item before %s: ", item );

      beforeOrder(outfile, root, root->theTree, item, move);

    }

  
  //HEIGHT_BALANCE
  
      int defineheight(itemNode *root){
        //define right height
        //define leftheight
        if(root == NULL){
          return 0;
        }
        //bottom of left or right
        int leftheight = defineheight(root->left) + 1;
        int rightheight = defineheight(root->right) + 1;

        if(leftheight >= rightheight){
          return leftheight;
        }

        return rightheight;
      }

      void height_balance(FILE* outfile, treeNameNode*root, char treeName[])
      {
        root = searchNameNode(root, treeName);
        int left = defineheight(root->theTree->left)-1;
        int right = defineheight(root->theTree->right)-1;

        int diff = abs( (defineheight(root->theTree->left)) - (defineheight(root->theTree->right)) );
        
        printf("%s: left height %d, right height %d, ", root->treeName, left, right);
        fprintf(outfile,"%s: left height %d, right height %d, ", root->treeName, left, right );


        printf("difference %d, ", diff);
        fprintf(outfile,"difference %d, ", diff );

        if(diff > 1)
        {
          printf("not balanced\n");
          fprintf(outfile, "not balanced\n");
        }
          
        else
        {
          printf("balanced\n");
          fprintf(outfile,"balanced\n" );
        }
          
      }

  
    
  //COUNT
    int sum(itemNode*root)
    { 
      int val; 

      if (root == NULL)
        return 0; 

      val = root->count; 

      if (val > 0) 
        return val + sum(root->left) + sum(root->right); 

      else 
        return val + sum(root->left) + sum(root->right); 
    } 

    void count(FILE* outfile, treeNameNode* root, char treeName[50])
    {
      root = searchNameNode(root, treeName);

      int total = sum(root->theTree);

      printf("%s count %d\n", treeName, total);
      fprintf(outfile, "%s count %d\n", treeName, total );
    }


  
  
  

  

  //DELETE
  
    itemNode* deleteItem(treeNameNode*ptr, itemNode* root, char item[])
    {
      // if the tree is empty
      // if we reach an open spot aka a null child
      if (root == NULL)
      {
        return root;
      }

      //printf("CURRENTLY @ %s",root->name);
      // check if the value is smaller than the current node
      if ((strcmp(root->name, item) < 0))
      {
        //printf("Went Right\n");
        // we need to go to the left
        root->right = deleteItem(ptr, root->right, item);
      }

      else if ((strcmp(root->name, item) > 0))
      {
      // printf("Went left\n");
        // we need to go the right
        root->left = deleteItem(ptr, root->left, item);
      }

      if((strcmp(root->name, item) == 0))
      {
        //printf("FOUND IT\n");

        itemNode* temp;
        // this is where we will write the code to remove the node
        // check if the node has 0 children
        if (root->left == NULL && root->right == NULL)
        {
          // free the node
          //printf("freed, %s\n", root->name);
          free(root);
          // return NULL in its place
          return NULL;
        }

        // check if only a right child
        if (root->right != NULL && root->left == NULL)
        {
          // store the child in a temp variable
          temp = root->right;
          // free the node
        // printf("freed, %s\n", root->name);
          free(root);
          // return the child in place of it parent
          return temp;
        }

        // check if only a left child
        if (root->left != NULL && root->right == NULL)
        {
          temp = root->left;
        // printf("freed, %s\n", root->name);
          free(root);
          return temp;
        }


        // if we get to this point it is assumed we have 2 children
        
        // find the largest value on the left
        // store the left child in a temp variable
        temp = root->left;

        // move to the right of that left child until we can't anymore
        while(temp->right != NULL)
        {
          temp = temp->right;
        }

        // temp should be storing the largest value on the left side of our main node

        // replace the root node with the value we just found
      
      // printf("current  %s \n", root->name);

        strcpy(root->name , temp->name);
        root->count = temp->count;

        //printf("new %s \n", root->name);

        // recursively remove the value that is now a duplicate
        root->left = deleteItem(ptr, root->left, root->name);
      }

      return root;

    }

    void delete(FILE* outfile,treeNameNode* root, char treeName[], char item[])
    {
      root = searchNameNode(root, treeName);

      deleteItem(root, root->theTree, item);

      printf("%s deleted from %s\n",item, root->treeName );
      fprintf(outfile, "%s deleted from %s\n",item, root->treeName );
    }



  
  
  //REDUCE
    itemNode* findItem(treeNameNode* ptr , itemNode* root, char item[])
    {
      if(root != NULL)
      {
        if (strcmp(root->name, item) == 0) //found a match
        {
          //printf("%d %s found in %s \n",root->count, root->name, ptr->treeName);
          return root;
        }

        if (strcmp(root->name, item) < 0) // we want to go right)
        {
          return findItem(ptr, root->right, item);
        }

        else //we want to go left
        {
          return findItem(ptr, root->left, item);
        }
      }

      //printf("%s not found in %s \n", item, ptr->treeName);
      return NULL;
    }

    void subt(itemNode* itNode, int val)
    {
      itNode->count = itNode->count - val;
    }

    void reduce(FILE* outfile, treeNameNode*root, char treeName[], char item[], int val)
    {
      
      root = searchNameNode(root, treeName);
      itemNode* itNode = findItem(root , root->theTree, item);

      //printf("%s, %s\n", root->treeName, item);
      printf("%s reduced\n", itNode->name);
      fprintf(outfile,"%s reduced\n", itNode->name );

      if(itNode->count < val)
        deleteItem(root, root->theTree, item);

      else
        subt(itNode, val);


      
    }



  
  
  //DELETE_NAME
  
    
    void freePostOrder(treeNameNode* ptr, itemNode* root)
    {
      //frees items
      // we have an empty tree OR we have reached a NULL child
      if (root == NULL)
      {
        return;
      }

      // we know we need 2 recursive calls
      freePostOrder(ptr, root->left);
      freePostOrder(ptr, root->right);

      free(root);
    }

    treeNameNode* delete_name(FILE* outfile, treeNameNode* ptr, char treeName[])
    {
      
      if (ptr == NULL)
      {
        return ptr;
      }

      //printf("CURRENTLY @ %s",root->name);
      // check if the value is smaller than the current node
      if ((strcmp(ptr->treeName, treeName) < 0))
      {
        //printf("Went Right\n");
        // we need to go to the left
        ptr->right = delete_name(outfile, ptr->right, treeName);
      }

      else if ((strcmp(ptr->treeName, treeName ) > 0))
      {
      // printf("Went left\n");
        // we need to go the right
        ptr->left = delete_name(outfile, ptr->left, treeName);
      }

      if((strcmp(ptr->treeName, treeName) == 0))
      {

        
          freePostOrder(ptr, ptr->theTree);
          //attempt to connect bird to root
        treeNameNode* temp;
        // this is where we will write the code to remove the node
        // check if the node has 0 children
        if (ptr->left == NULL && ptr->right == NULL)
        {
          // free the node
          //printf("freed, %s\n", root->name);
           printf("%s deleted\n", ptr->treeName);
           fprintf(outfile,  "%s deleted\n", ptr->treeName);
          free(ptr);
          // return NULL in its place
          return NULL;
        }

        // check if only a right child
        if (ptr->right != NULL && ptr->left == NULL)
        {
          // store the child in a temp variable
          temp = ptr->right;
          // free the node
          printf("%s deleted\n", ptr->treeName);
          fprintf(outfile,  "%s deleted\n", ptr->treeName);
          free(ptr);
          // return the child in place of it parent
          return temp;
        }

        // check if only a left child
        if (ptr->left != NULL && ptr->right == NULL)
        {
          temp = ptr->left;

           printf("%s deleted\n", ptr->treeName);
          fprintf(outfile,  "%s deleted\n", ptr->treeName);
      
          free(ptr);
          return temp;
        }


        // if we get to this point it is assumed we have 2 children
        
        // find the largest value on the left
        // store the left child in a temp variable
        temp = ptr->left;

        // move to the right of that left child until we can't anymore
        while(temp->right != NULL)
        {
          temp = temp->right;
        }

        // temp should be storing the largest value on the left side of our main node

        // replace the root node with the value we just found

        strcpy(ptr->treeName , temp->treeName);

        // recursively remove the value that is now a duplicate
        ptr->left = delete_name(outfile, ptr->left, ptr->treeName);

      }
      
        //treeNameNode freed
        return ptr;
    }

      
      

  //Quiery Menu
    void quieryMenu(FILE*infile, FILE* outfile, treeNameNode* root, char q_command[])
    {
      treeNameNode* tnn;
      char treeName [MAXLEN];
      char item [MAXLEN];
      int val = 0;

      if(strcmp("search", q_command)==0)
      {
        fscanf(infile,"%s %s", treeName, item);
        search(outfile, root, treeName, item);
      }
        
      if(strcmp("item_before", q_command)==0)
      {
        fscanf(infile,"%s %s", treeName, item);
        item_before(outfile, root, treeName, item);
      }
      

      if(strcmp("height_balance", q_command)==0)
      {
        fscanf(infile,"%s", treeName);
        height_balance(outfile, root, treeName);
      }
        

      if(strcmp("count", q_command)==0)
      {
        fscanf(infile,"%s", treeName);
        count(outfile, root, treeName);
      }


      if(strcmp("reduce", q_command)==0)
      {
        fscanf(infile,"%s %s %d", treeName, item, &val);
        reduce(outfile, root, treeName, item, val);
      } 


      if(strcmp("delete", q_command)==0)
      {
        fscanf(infile,"%s %s", treeName, item);
        delete(outfile, root, treeName, item);
      } 


      if(strcmp("delete_name", q_command)==0)
      {
        fscanf(infile,"%s", treeName);
        delete_name(outfile, root, treeName);
      } 
    }




//

//Free AFTER
  treeNameNode* delete_name2(FILE* outfile, treeNameNode* ptr, char treeName[])
    {
      
      if (ptr == NULL)
      {
        return ptr;
      }

      //printf("CURRENTLY @ %s",root->name);
      // check if the value is smaller than the current node
      if ((strcmp(ptr->treeName, treeName) < 0))
      {
        //printf("Went Right\n");
        // we need to go to the left
        ptr->right = delete_name2(outfile, ptr->right, treeName);
      }

      else if ((strcmp(ptr->treeName, treeName ) > 0))
      {
      // printf("Went left\n");
        // we need to go the right
        ptr->left = delete_name2(outfile, ptr->left, treeName);
      }

      if((strcmp(ptr->treeName, treeName) == 0))
      {

        
          freePostOrder(ptr, ptr->theTree);
          //attempt to connect bird to root
        treeNameNode* temp;
        // this is where we will write the code to remove the node
        // check if the node has 0 children
        if (ptr->left == NULL && ptr->right == NULL)
        {
          // free the node
          //printf("freed, %s\n", root->name);
           printf("%s deleted\n", ptr->treeName);
           fprintf(outfile,  "%s deleted\n", ptr->treeName);
          free(ptr);
          // return NULL in its place
          return NULL;
        }

        // check if only a right child
        if (ptr->right != NULL && ptr->left == NULL)
        {
          // store the child in a temp variable
          temp = ptr->right;
          // free the node
          printf("%s deleted\n", ptr->treeName);
          fprintf(outfile,  "%s deleted\n", ptr->treeName);
          free(ptr);
          // return the child in place of it parent
          return temp;
        }

        // check if only a left child
        if (ptr->left != NULL && ptr->right == NULL)
        {
          temp = ptr->left;

           printf("%s deleted\n", ptr->treeName);
          fprintf(outfile,  "%s deleted\n", ptr->treeName);
      
          free(ptr);
          return temp;
        }


        // if we get to this point it is assumed we have 2 children
        
        // find the largest value on the left
        // store the left child in a temp variable
        temp = ptr->left;

        // move to the right of that left child until we can't anymore
        while(temp->right != NULL)
        {
          temp = temp->right;
        }

        // temp should be storing the largest value on the left side of our main node

        // replace the root node with the value we just found

        strcpy(ptr->treeName , temp->treeName);

        // recursively remove the value that is now a duplicate
        ptr->left = delete_name2(outfile, ptr->left, ptr->treeName);

      }
      
        //treeNameNode freed
        return ptr;
    }

    void freeAfter(FILE* outfile, treeNameNode* root)
    { 
      
      if(root == NULL)
        return;

      freeAfter(outfile, root->right);
      freeAfter(outfile, root->left);

     // printf("@ %s\n", root->treeName);
      freePostOrder(root, root->theTree);
      free(root);
    }


//

int main(void) {

 atexit(report_mem_leak);
 FILE* infile = fopen("in.txt", "r");
 FILE* outfile = fopen("out.txt", "w");
 
 if (infile != NULL)
 {

  char treeName[MAXLEN];
  char theTree[MAXLEN];
  char name[MAXLEN];
  char q_command[MAXLEN];
  
  int num_treeNames;
  int num_items;
  int num_queries;
  int count; 
 
  fscanf(infile,"%d %d %d", &num_treeNames, &num_items, &num_queries);

  treeNameNode* root = NULL;

  //building my treeNamesTree
  for (int i = 0; i < num_treeNames; i++)
  { 
    fscanf(infile, "%s", treeName);
    root =  buildNameTree(infile, root, treeName);  
  }

  //building my items tree
  for(int i = 0; i < num_items; i++)
  {
    fscanf(infile, "%s %s %d", theTree, name, &count );
    root = addtoTree(infile, root, theTree, name, count);
  }

  //printing my forest
  traverse_in_traverse(outfile,root);
  
  printf("\n");
  fprintf(outfile,"\n" );

  //Calling my quieres
  for(int i = 0; i < num_queries; i++)
  {
    fscanf(infile,"%s", q_command);
    quieryMenu(infile, outfile, root, q_command); 
  }

  //freeAfter(root);


  fclose(infile);
 }
 
  else
  {
      printf("File could not be read\n");
      fprintf(outfile,"File could not be read\n");
      exit(-1);
  }
  
  fclose(outfile);
  return 0;
}
