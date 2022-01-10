/* http://tharikasblogs.blogspot.com/p/how-to-write-your-own-malloc-and-free.html */


#include<stdio.h>
#include<stddef.h>  //contains the defination of size_t datatype
#include "mymalloc.h"

/*we initialize a pointer of type "block", named freeList pointing to the starting address of the chunk of memory we created before. 
This freeList pointer will point to the start of the linked list of metadata blocks.
The starting address of the array (memory) should be casted to type void so that we are able to allocate blocks of memory which are 
of different datatypes.(int, char, float etc.) */

void initialize(){
 freeList->size=20000-sizeof(struct block); /*The size of the block that it refers to is (20000 bytes- the_size_of_one_metadata_block)*/
 freeList->free=1;                          /*To indicate that the block is not yet allocated, we set the free flag to 1.*/
 freeList->next=NULL;                       /*And the first metadata block has no next metadata block yet. So we set next to NULL.*/
}


void split(struct block *fitting_slot,size_t size){
 struct block *new=(void*)((void*)fitting_slot+size+sizeof(struct block));
 new->size=(fitting_slot->size)-size-sizeof(struct block);
 new->free=1;
 new->next=fitting_slot->next;
 fitting_slot->size=size;
 fitting_slot->free=0;
 fitting_slot->next=new;
}


void *MyMalloc(size_t noOfBytes){
 struct block *curr,*prev;
 void *result;
 if(!(freeList->size)){
  initialize();
  printf("Memory initialized\n");
 }
 curr=freeList;
 while((((curr->size)<noOfBytes)||((curr->free)==0))&&(curr->next!=NULL)){
  prev=curr;
  curr=curr->next;
  printf("One block checked\n");
 }
 if((curr->size)==noOfBytes){
  curr->free=0;
  result=(void*)(++curr);
  printf("Exact fitting block allocated\n");
  return result;
 }
 else if((curr->size)>(noOfBytes+sizeof(struct block))){
  split(curr,noOfBytes);
  result=(void*)(++curr);
  printf("Fitting block allocated with a split\n");
  return result;
 }
 else{
  result=NULL;
  printf("Sorry. No sufficient memory to allocate\n");
  return result;
 }
}

void merge(){
 struct block *curr,*prev;
 curr=freeList;
 while((curr->next)!=NULL){
  if((curr->free) && (curr->next->free)){
   curr->size+=(curr->next->size)+sizeof(struct block);
   curr->next=curr->next->next;
  }
  prev=curr;
  curr=curr->next;
 }
}

void MyFree(void* ptr){
 if(((void*)memory<=ptr)&&(ptr<=(void*)(memory+20000))){
  struct block* curr=ptr;
  --curr;
  curr->free=1;
  merge();
 }
 else printf("Please provide a valid pointer allocated by MyMalloc\n");
}

                                                                                                                                            

mymalloc.h


#include<stdio.h>
#include<stddef.h>

char memory[20000];   //Our memory pool from where we are allocating the memory. Array declaration will give continous memory

//Metadata block structure. This is required to keep track which block of memory is freed and which one is available or already allocated.
struct block{
 size_t size; /*Carries the size of the block described by it*/
 int free;     /*flag is used to know whether the block described by the metadata structure is free or not -- > if free, it is set to 1. Otherwise 0.*/
 struct block *next; /*Points to the next metadata block*/ 
};

struct block *freeList=(void*)memory; /*Pointing to the main block of memory which is initially free (no storage allocation yet)*/

void initialize();
void split(struct block *fitting_slot,size_t size);
void *MyMalloc(size_t noOfBytes);
void merge();
void MyFree(void* ptr);

                                                                                                                                            

Main.c


#include<stdio.h>

int main(){
 
 int *p=(int)MyMalloc(100*sizeof(int));
 char *q=(char)MyMalloc(250*sizeof(char));
 int *r=(int)MyMalloc(1000*sizeof(int));
 MyFree(p);
 char *w=(char)MyMalloc(700);
 MyFree(r);
 int *k=(int)MyMalloc(500*sizeof(int));
 printf("Allocation and deallocation is done successfully!");
 
}
