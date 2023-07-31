/*Modify the singly linked list to be a doubly linked list. 
Now write a routine that removes all duplicate data in the doubly linked list. 
The data will be integers generated at random from [0,49]. Initially have a list of 200 elements.  
Now do this in one of two ways. Sort the list by its data field. 
Remove adjacent elements of the sorted list with the same value. 
Or, take the first element—search the remainder of the list for elements with the same data and remove them. 
Then take the second element and do the same. Repeat until only one element is left.*/

#include <stdio.h>  
#include <stdlib.h>
//Represent a node of the doubly linked list  
  
struct node{  
    int data;  
    struct node *previous;  
    struct node *next;  
};      
   
//Represent the head and tail of the doubly linked list  
struct node *head, *tail = NULL;  
   
//addNode() will add a node to the list  
void addNode(int data) {  
    //Create a new node  
    struct node *newNode = (struct node*)malloc(sizeof(struct node));  
    newNode->data = data;  
      
    //If list is empty  
    if(head == NULL) {  
        //Both head and tail will point to newNode  
        head = tail = newNode;  
        //head's previous will point to NULL  
        head->previous = NULL;  
        //tail's next will point to NULL, as it is the last node of the list  
        tail->next = NULL;  
    }  
    else {  
        //newNode will be added after tail such that tail's next will point to newNode  
        tail->next = newNode;  
        //newNode's previous will point to tail  
        newNode->previous = tail;  
        //newNode will become new tail  
        tail = newNode;  
        //As it is last node, tail's next will point to NULL  
        tail->next = NULL;  
    }  
}  
   
//removeDuplicateNode() will remove duplicate nodes from the list  
void removeDuplicateNode() {  
    //Node current will point to head  
    struct node *current, *index, *temp;  
      
    //Checks whether list is empty  
    if(head == NULL) {  
        return;  
    }  
    else {  
        //Initially, current will point to head node  
        for(current = head; current != NULL; current = current->next) {  
            //index will point to node next to current  
            for(index = current->next; index != NULL; index = index->next) {  
                if(current->data == index->data) {  
                    //Store the duplicate node in temp  
                    temp = index;  
                    //index's previous node will point to node next to index thus, removes the duplicate node  
                    index->previous->next = index->next;  
                    if(index->next != NULL)  
                        index->next->previous = index->previous;  
                    //Delete duplicate node by making temp to NULL  
                    temp = NULL;  
                }  
            }  
        }  
    }  
}  
   
//display() will print out the nodes of the list  
void display() {  
    //Node current will point to head  
    struct node *current = head;  
    if(head == NULL) {  
        printf("List is empty\n");  
        return;  
    }  
    while(current != NULL) {  
        //Prints each node by incrementing pointer.  
        printf("%d ", current->data);  
        current = current->next;  
    }  
    printf("\n");  
}  
   
int main()  
{  
    //Add nodes to the list  
    addNode(1);  
    addNode(2);  
    addNode(3);  
    addNode(2);  
    addNode(2);  
    addNode(4);  
    addNode(5);  
    addNode(3);  
      
    printf("Originals list: \n");  
    display();  
      
    //Removes duplicate nodes  
    removeDuplicateNode();  
      
    printf("List after removing duplicates: \n");  
    display();  
   
    return 0;  
} 



