#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include <stddef.h>

//By: Rishi Shah and Eshan Wadhwa

/* Function that initalizes the block of memory that is pointed to by the Linked List. */
void setUp() {
  freePtr->size=4096-sizeof(Node); // Size of the block is 4096 - the size of one metadata block
  freePtr->isFree=1; // The block is not allocated
  freePtr->next=NULL; // Next metadata block is NULL
}

/* Function which implements the First Fit Algorithm.
 * This function starts from the first metadata block and searches for the first block that has enough space to allocate.
 * newBlock is a metadata block pointer which will then point to the metadata block referring to the next free chunk on memory. */
void divide(Node* bigBlock, size_t size) {
  Node* newBlock=(void*) ((void*) bigBlock+size+sizeof(Node));
  newBlock->size=bigBlock->size-size-sizeof(Node);
  newBlock->isFree=1;
  newBlock->next=bigBlock->next;
  bigBlock->size=size;
  bigBlock->isFree=0;
  bigBlock->next=newBlock;
}

/* Recreating the malloc function. */
void* mymalloc(size_t size, char* file, int line) {
  if(size < 1){ //error if size <1
  	fprintf(stderr,"Error: Can not malloc 0 or negative bytes. File: %s, Line: %d\n", file, line);
  	return NULL;
}

  if (size>(4096-sizeof(Node))) { //error if size is too much
    fprintf(stderr,"Error: Saturation of dynamic memory. File: %s, Line: %d\n",file,line);
	   return NULL;
  }

  if (!(freePtr->size)) { // Initializes the memory if it is not initialized at the beginning.
    setUp();
  }

  Node* current=freePtr; // Pointer that points to the start of the Linked List.
  void* returnPtr;

  while ((((current->size)<size)||((current->isFree)!=1))&&(current->next!=NULL)) { // Keeps iterating through the Linked List until the pointer finds free space.
    current=current->next;
  }

  if (current->size==size) { // The metadata block checked refers to a chunk of memory that exactly fits the required size.
    current->isFree=0;
    returnPtr=(void*)(++current);
  }

  else if ((current->size)>(size+sizeof(Node))) { // The metadata block checked refers to a chunk of memory that is of size greater than what is required.
    divide(current,size);
    returnPtr=(void*) (++current);
  }
  else {
    fprintf(stderr,"Error: Not enough memory to allocate requested size. File %s, Line: %d\n", file, line); //error if not enough memory
    return NULL;
  }
  return returnPtr;
}


/* Combines the consecutive free blocks by removing the metadata block in the middle. */
void combine() {
  Node* current=freePtr;

  while ((current->next)!=NULL) {
    if ((current->isFree)&&(current->next->isFree)) {
      current->size=current->size+current->next->size+sizeof(Node);
      current->next=current->next->next;
    }
    else {
      current=current->next;
    }
  }
}

/* Recreating the free function to detect common errors. */
void myfree(void* ptr, char* file, int line) {
  Node* curr=freePtr;

  if(ptr == NULL) //error if no pointer
{
	fprintf(stderr,"Error: Can not free an address that is not a pointer. File: %s, Line: %d\n", file, line);
	return;
}

 else if (((void*)myblock<=ptr)&&(ptr<=(void*)(myblock+4096))) { // Checks to see if the pointer lies within the heap so that it can be freed.

    Node* current=ptr;
    (--current);

	if(inMem(current) == 0){ //error if pointer not malloced
        fprintf(stderr, "Error: Pointer is not allocated by malloc. File: %s, Line: %d\n", file, line);
        return;
}

    if(current->isFree == 1) //Checks to see if the pointer has been freed multiple times.
{
	fprintf(stderr, "Error: Redundant freeing of pointer. File %s, Line: %d\n", file, line);
}
   else //Free the pointer.
{
    current->isFree=1;
    combine();
}
}
  else{
	fprintf(stderr, "Error: Pointer is not allocated by malloc. File: %s, Line: %d\n", file, line); //pointer not allocated by malloc
	return;
}
}
/* Function which checks to see if pointer was allocated by malloc. */
int inMem(struct Node *n){
struct Node *head = freePtr;

while(head != NULL)
{
if(n == head)
{
return 1;
}
head = head->next;
}
return 0;
}
