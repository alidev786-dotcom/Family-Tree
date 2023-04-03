#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>
#include<unistd.h>

#pragma warning(disable:4996)       
#define _CRT_SECURE_NO_WARNINGS

//######################################## FAMILY NODE STRUCT ###################################
typedef struct FamilyNode {
    char name[100];                        //stores name of person
    int age;                               //stores age of person
    struct FamilyNode* left_parent;        //pointer to the left parent
    struct FamilyNode* right_parent;       //pointer to the right parent
} FamilyNode;

//################################## FUNCTION TO DISPLAY FAMILY NODE #############################
void displayNode(struct FamilyNode* node)
{
    if (strcmp(node->name, "None") != 0)
    {
        printf("name: %s\n", node->name);   //printing the name
        printf("age: %d\n\n", node->age);   //printing the age
    }
}

// ################################# FUNCTION TO DISPLAY TREE LEAVES ##############################
void displayLeaveNode(struct FamilyNode* node)
{
    if ( strcmp(node->left_parent->name,"None")==0 && strcmp(node->right_parent->name,"None")==0 )
    {
        printf("name: %s\n", node->name);   //printing the name
        printf("age: %d\n\n", node->age);   //printing the age
    }
}

static int found = 0;                       //static variavble to check if node is found

// ################################ FUNCTION TO INSERT NODE BY READING FROM FILE ###################
void insertFamilyNode(FamilyNode* newNode, FamilyNode** rootNode)
{
    if ((*rootNode) == NULL)                                      //if root is null, simply return
    {
        (*rootNode) = newNode;
        return;
    }
    else if (strcmp((*rootNode)->name, newNode->name) == 0)       //if child is found, simply insert parent 
    {
        (*rootNode) = newNode;
        found++;
        return;
    }
    else
    {
        if ((*rootNode)->left_parent != NULL)
        {
            insertFamilyNode(newNode, &((*rootNode)->left_parent));       //calls recursively left parents 
        }
        if (found == 0)
        {
            if ((*rootNode)->right_parent != NULL)
            {
                insertFamilyNode(newNode, &((*rootNode)->right_parent));  //calls recursively right parents
            }
        }
    }
}

//###################### FUNCTION TO READ DATA FROM FILE AND INSERTING INTO TREE ###############
FamilyNode* readDataFromFile(char* file_name)
{
    FamilyNode* rootNode = NULL;                                       
    FamilyNode* temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);
    FamilyNode* left_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);      //creating child and parent nodes
    FamilyNode* right_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);
    temp->left_parent = left_temp; temp->right_parent = right_temp;
    left_temp->left_parent = left_temp->right_parent = NULL;
    right_temp->left_parent = right_temp->right_parent = NULL;

    char name[100]; int age; char left[100]; char right[100];                   //stores user input data


    FILE* fptr = fopen(file_name, "r");
    fscanf(fptr, "%s %d %s %s", name, &age, left, right);
    strcpy(temp->name, name); temp->age = age; strcpy(temp->left_parent->name, left); strcpy(temp->right_parent->name, right);
    insertFamilyNode(temp, &rootNode);
    int count = 0;
    while (true)
    {
        FamilyNode* temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);
        FamilyNode* left_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);    //stores user input data 
        FamilyNode* right_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);
        temp->left_parent = left_temp; temp->right_parent = right_temp;
        left_temp->left_parent = left_temp->right_parent = NULL;
        right_temp->left_parent = right_temp->right_parent = NULL;

        char name[100]; int age; char left[100]; char right[100];

        fscanf(fptr, "%s %d %s %s", name, &age, left, right);
        if (strcmp(name, "end") == 0)                                            //if end found, stop reading from file
        {
            break;
        }
        strcpy(temp->name, name); temp->age = age; strcpy(temp->left_parent->name, left); strcpy(temp->right_parent->name, right);
        found = 0 ;
        insertFamilyNode(temp, &rootNode);                                       //insert node to tree
    }
    fclose(fptr);
    return rootNode;                                                              //return root node
}


//########################## FUNCTION TO ADD NEW FAMILY MEMBER INTO TREE #######################
bool addNewFamilyMember(char *child, FamilyNode **root, FamilyNode **newNode)
{
    bool res ;
    if( strcmp((*root)->name,child)==0 )                        //if child is found
    {
        //checking if this child has both parents
        if( strcmp( (*root)->left_parent->name,"None")!=0 && strcmp((*root)->right_parent->name,"None")!=0 )
        {
            printf("\nThis child '%s' already has both children!\n\n",child) ;
            return false ;
        }
        if( strcmp((*root)->left_parent->name,"None") == 0 )        //if left parent is null, put new parent in left
        {
            (*root)->left_parent = *newNode ;                           
        }
        else if( strcmp((*root)->right_parent->name,"None") == 0 )  //if right parent is null,put new parent in right
        {
            (*root)->right_parent = *newNode ;
        }
        found++ ; 
        return true;                                                //return true if child is found
    }
    else
    {
        
        if ((*root)->left_parent != NULL)
        {
            res = addNewFamilyMember(child, &((*root)->left_parent),newNode) ;         //calls for left sub tree
        }
        if (found == 0)
        {
            if ((*root)->right_parent != NULL)
            {
                res = addNewFamilyMember(child, &((*root)->right_parent),newNode) ;    //calls for right sub tree
            }
        }
    }
    return res ;                                                                       //returns result true/false
}

// ##################################### PERFORMS MENU OPTION 1 ##############################
void menuOption1(FamilyNode** rootNode)
{
    char name[100]; int age ; char child[100] ;
    printf("Enter new member name: ") ;                                                 //taking input from user
    scanf("%s",name) ;
    printf("Enter new member age: ") ;
    scanf("%d",&age) ;

    FamilyNode* temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);
    FamilyNode* left_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);                //nodes to store new data
    FamilyNode* right_temp = (FamilyNode*)malloc(sizeof(FamilyNode) * 1);

    strcpy(left_temp->name,"None") ;
    strcpy(right_temp->name,"None") ;

    left_temp->left_parent = left_temp->right_parent = NULL;
    right_temp->left_parent = right_temp->right_parent = NULL;
    temp->left_parent = left_temp; temp->right_parent = right_temp;

    //taking input child name for whom you want to add parents
    printf("Enter child name: ") ; 
    scanf("%s",child) ;

    strcpy(temp->name,name) ;
    temp->age = age ;
    found = 0;
    bool res = addNewFamilyMember(child, &(*rootNode),&temp) ;
    if(res==true)
    {
        printf("\nPerson added successfully!\n\n") ;
    }
    else
    {
        printf("\nChild not found!\n\n") ;
    }
}

// ############################## INORDER TRAVERSAL OF TREE ##########################
void inOrderTraversal(FamilyNode* root)
{
    if (root == NULL)
    {
        return;
    }
    inOrderTraversal(root->left_parent);
    displayNode(root);
    inOrderTraversal(root->right_parent);
}

// ############################# PREORDER TRAVERSAL OF TREE ##########################
void preOrderTraversal(FamilyNode* root)
{
    if(root == NULL)
    {
        return ;
    }
    displayNode(root) ;
    preOrderTraversal(root->left_parent) ;
    preOrderTraversal(root->right_parent) ;
}

// ############################# POST ORDER TRAVERSAL OF TREE #########################
void postOrderTraversal(FamilyNode *root)
{
    if(root == NULL)
    {
        return ;
    }
    postOrderTraversal(root->left_parent) ;
    postOrderTraversal(root->right_parent) ;
    displayNode(root) ;
}


// ############################# FUNCTION TO FIND OLDEST IN TREE #######################
FamilyNode* findOldest(FamilyNode *root, FamilyNode **intializer)
{
    if(root == NULL)
    {
        return (*intializer);
    }
    if(root->age > (*intializer)->age && (strcmp(root->name,"None")!=0))   //if greater age is found, update in initializer
    {
        (*intializer) = root ;
    }
    if(root->left_parent != NULL)
    {
        findOldest(root->left_parent,&(*intializer)) ;                     //calls itself for left sub tree
    }
    if(root->right_parent != NULL)
    {
        findOldest(root->right_parent, &(*intializer)) ;                   //calls itself for right sub tree
    }
    return (*intializer) ;
}

// ############################ PERFORMS MENU OPTION 2 #####################
void menuOption2(FamilyNode *root)
{
    int choice ;
    printf("1- Inorder Traversal\n")    ;
    printf("2- Postorder Traversal\n")  ;
    printf("3- Preorder Traversal\n\n") ;
    printf("Enter your choice(1-3): ")  ;
    scanf("%d",&choice) ;
    //validating the input
    while( choice<1 || choice>3 )
    {   
        printf("\n\nEnter valid choice(1-3): ") ;                           //asking for which traversal to perform
        scanf("%d",&choice) ;
    }
    switch(choice)
    {
        case 1:
            printf("\n\n############### INORDER TRAVERSAL OF FAMILY TREE ################\n\n") ;
            inOrderTraversal(root) ;
            break ;
        case 2:
        printf("\n\n############### POSTORDER TRAVERSAL OF FAMILY TREE ################\n\n") ;
            postOrderTraversal(root) ;
            break ;
        case 3:
            printf("\n\n############### PREORDER TRAVERSAL OF FAMILY TREE ################\n\n") ;
            preOrderTraversal(root);
            break ;
        default:
            break ;
    } 
}


// ################################## PERFORMS MENU OPTION 3 #############################
void menuOption3(FamilyNode *root)
{
    printf("\n\n#################### FINDING THE OLDEST IN FAMILY ################\n\n") ;
    FamilyNode *oldest = findOldest(root, &root) ;
    printf("Name: %s\n",oldest->name) ;   //printing found oldest member in family
    printf("Age: %d\n",oldest->age) ;
}


// ######################## FUNCTION TO REMOVE A PERSON FROM FAMILY #######################
bool removePerson(FamilyNode **root, char *name)
{
    bool res = false;
    if ( strcmp((*root)->name,name)==0 )                                //if person found which is to be deleted
    {
        free( (*root) ) ;                                               //deallocating the memory
        (*root) = NULL  ;
        FamilyNode *temp = (FamilyNode*) malloc(sizeof(FamilyNode)*1) ;
        strcpy(temp->name,"None") ;
        temp->left_parent = temp->right_parent = NULL;
        (*root) = temp ;
        return true ;
    }
    if( (*root)->left_parent != NULL)
    {
        res = removePerson( &((*root)->left_parent), name ) ;            //calls itself for left sub tree
        if(res==true)
        {
            return true ;
        }
    }
    if( (*root)->right_parent != NULL )
    {
        res = removePerson( &((*root)->right_parent), name ) ;           //calls itself for right sub tree
    }
    return res ;
}

// ############################### PERFORMS MENU OPTION 4 ###########################
void menuOption4(FamilyNode **root)
{
    char name[100] ;
    printf("\n\n################## REMOVING THE PERSON ###############\n\n") ;
    printf("Enter name of person you want to remove: ") ;                //asking for the person to be removed
    scanf("%s",name) ;
    bool res = removePerson(&(*root), name) ;
    if(res==true)
    {
        printf("Person removed successfully!\n\n") ;
    }
    else
    {
        printf("Person not found!\n\n") ;
    }
}

// ############################## FUNCTION TO DISPLAY LEAVES OF TREE #################
void displayLeaves(FamilyNode* root)
{
    if ( strcmp(root->name,"None")==0 )
    {
        return;
    }
    
    displayLeaves(root->left_parent);
    displayLeaveNode(root) ;
    displayLeaves(root->right_parent);
}

// ############################## PERFORMS MENU OPTION 5 ############################
void menuOption5(FamilyNode* root)
{
    printf("\n\n#################### PRINTING THE TREE LEAVES ###################\n\n") ;
    displayLeaves(root) ;
}

// ###################### SHOWS MENU AND PERFORM OPERATIONS ACCORDINGLY #################
void showMenu(FamilyNode **rootNode)
{
    printf("\n\n##################### WELCOME TO FAMILY TREE ####################") ;
    int choice ;
    printf("\n\n1- Add a family member\n") ;
    printf("2- Print the tree\n") ;
    printf("3- Find the oldest\n") ;
    printf("4- Remove a person\n") ;
    printf("5- Print tree leaves\n") ;
    printf("6- Exit\n\n") ;
    printf("Enter your choice(1-6): ") ;
    scanf("%d",&choice) ;
    //validating the input
    while(choice<1 || choice>6)
    {
        printf("\n\nInvalid option!") ;
        printf("\n\n##################### WELCOME TO FAMILY TREE ####################") ;
        printf("\n\n1- Add a family member\n") ;
        printf("2- Print the tree\n") ;
        printf("3- Find the oldest\n") ;
        printf("4- Remove a person\n") ;
        printf("5- Print tree leaves\n") ;
        printf("6- Exit\n\n") ;
        printf("Enter your choice(1-6): ") ;
        scanf("%d",&choice) ;
    }
    if(choice==1)
    {
        menuOption1(&(*rootNode)) ;                   //calling menu option 1
        showMenu(&(*rootNode)) ;
    }
    else if(choice==2)
    { 
        menuOption2(*rootNode) ;                      //calling menu option 2
        showMenu(&(*rootNode)) ;
    }
    else if(choice==3)
    {
        menuOption3(*rootNode) ;                      //calling menu option 3
        showMenu(&(*rootNode)) ;
    }
    else if(choice==4)
    {
        menuOption4(&(*rootNode)) ;                   //calling menu option 4
        showMenu(&(*rootNode)) ;
    }
    else if(choice==5)
    {
        menuOption5(*rootNode) ;                      //calling menu option 5
        showMenu(&(*rootNode)) ;
    }
    else if(choice==6)
    {
        exit(0) ;                                     //exits the program
    }
}

// ############################### FUNCTION TO DRIVE THE WHOLE PROGRAM #########################
void Driver()
{
    system("cls") ;                                               //clears screen
    printf("Reading Data from file...\n") ;
    sleep(1) ;
    system("cls") ;                                               //clears screen
    FamilyNode* root1 = readDataFromFile("familyinfo.txt");       //reading data from file
    showMenu(&root1) ;                                            //displays menu
}


// ##################################### MAIN FUNCTION ##########################################
int main()
{
    Driver() ;
    return 0;
}