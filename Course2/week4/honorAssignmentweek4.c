/*Open and read a file of integer pairs into an array that is created with the first integer telling you how many to read.  
So  4 9  11  4  5  would mean create an integer array size 4 and read into data[].   
Write a routine that places these values into a binary tree structure. Then walk the tree “inorder” and print these values to the screen. */

#include <stdio.h>  //standar io
#include <stdlib.h> //for amlloc() and exit() system function

/*Creating the structure for Binary Tree using the node */
struct btnode
{
    int value;
    struct btnode *left; //creating the left leaf
    struct btnode *right; // creating the right leaf
}*root = NULL, *temp = NULL; //creating two node main root node and other is temp pointer for binary tree node

typedef struct btnode node; //creating the node for the binary

/*Creating function defination for the use of binary tree cretaion, insert , preoder ,postorder and inorder*/
node* bt(int arr[],int,int);
node* new(int);
void inorder(node *tem);
void preorder(node *tem);
void postorder(node *tem);

void main()
{
    int  size;
	FILE *toRead = NULL ;
	/*Opening the inputdata.txt file for storing the elemnts in data[]*/
	toRead = fopen ("inputdata.txt" , "r");

	int array[4];
	/*
	*	Element  ----> variable is for finding all the available int elements in the file
	*	loop  -------> variable is used for the displaying the specific element of array data[]
	*	prev_number -> variable is used for stroing the previous element to find max
	*/
	int element = 0 ,loop =0 ;



	/*Data is the Dynamic memory for data[]. The pointer should be initialize with the NULL*/
	int *data=NULL;

	/*inputdata.txt file is available or not available checking. If the file is not available than not to do any function*/
	if (toRead == NULL)
	{
		printf("Getting an error while reading a file\r\n");
		exit(0); //exit from the main file as the file is not present
	}
	else
	{
		printf("File open sucessfully \r\n");
	}
	/*As per defination of the program first int element in the file will tell us the total available elements in the file*/
	fscanf(toRead,"%d",&element);

	/*As the elements may vary as per the user input we have created dynamic array  for the array.*/
	data = (int *)malloc(element * sizeof(int));
	//root = insert(root, element);

	/*copy the elements to the dynamic memory of the data in the form of int we used fscanf()*/
	for(;fscanf(toRead,"%d",&*(++data)) != EOF;)
	{

		printf("Element %d: %d \r\n",loop,*(data)); // as per the given input need to print all the elements on std o/p.
		//insert(root,*(data));
		array[loop] = *data;
		printf("%d \r\n",array[loop]);
		loop++;
	}
	fclose(toRead);
    size = sizeof(array) / sizeof(array[0]);

    printf("\n1- Inorder\n");

    root = bt(array, 0, size-1);
    printf("Given inorder traversal as input\n");
    for (loop = 0;loop<= element-1;loop++)
		printf("%d->", array[loop]);
    printf("\npreorder traversal of tree\n");
    preorder(root);
    printf("\ninorder traversal of tree\n");
    inorder(root);
    printf("\npostorder traversal of tree\n");
    postorder(root);
}

/* To create a new node */
node* new(int val)
{
    node* node1 = (node*)malloc(sizeof(node));

    node1->value = val;
    node1->left = NULL;
    node1->right  =  NULL;
    return node1;
}

/* To create a balanced binary search tree */
node* bt(int arr[], int first, int last)
{
    int mid;

    node* root = (node*)malloc(sizeof(node));
    if (first > last)
        return NULL;
    mid = (first + last) / 2;
    root = new(arr[mid]);
    root->left = bt(arr, first, mid - 1);
    root->right = bt(arr, mid + 1, last);
    return root;
}

/* to display inorder of tree */
void inorder(node *t)
{
    if (t->left != NULL)
        inorder(t->left);
    printf("%d->", t->value);
    if (t->right != NULL)
        inorder(t->right);
}

/* to display preorder traversal of tree */
void preorder(node *t)
{
    printf("%d->", t->value);
    if (t->left != NULL)
        inorder(t->left);
    if (t->right != NULL)
        inorder(t->right);
}

/* to display postorder traversal of tree */
void postorder(node *t)
{
    if (t->left != NULL)
        inorder(t->left);
    if (t->right != NULL)
        inorder(t->right);
    printf("%d->", t->value);
}
