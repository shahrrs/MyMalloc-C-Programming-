#ifndef _MYMALLOC_H
#define _MYMALLOC_H
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* Redefining the malloc and free functions, using macros. */
#define malloc(size) mymalloc(size,__FILE__,__LINE__)
#define free(size) myfree(size,__FILE__,__LINE__)

/* Linked List which contains the metadata of each block. */
typedef struct Node{
  short int size; // Size of the block.
  short int isFree; // 1=free, 0=in use
  struct Node* next; // Points to the next metadata block.
}Node;

static char myblock[4096]; // This array is considered as our memory. It acts as our heap.

static Node* freePtr=(void*) myblock; // Points to the memory.

/* Function Signatures. */
void setUp();
void divide(Node* bigBlock, size_t size);
void* mymalloc(size_t size, char* file, int line);
void combine();
void myfree(void* ptr, char* file, int line);
int inMem(struct Node *n);

#endif
