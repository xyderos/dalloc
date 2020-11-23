#include "headAPI.h"
#include <stdio.h>

//extern definitions
const u16 ALIGN = 8;

const sz ARENA = 64 * 1024;

//retrieve the next block of memory from the current block given
head_t *after(const head_t *const block) {

  return (head_t*)((char*) block + sizeofHead() + block->size);

}

//retrieve the previosu block of memory from the current block given
head_t *before(const head_t *const block) {

  return (head_t*)((char*) block - sizeofHead() - block->size);

}

// initialise the arena
// IDEA put arena and freeLists in a struct? can then have a struct on the main file and wrap the allocators
head_t *initialise(head_t* arena) {

  // if the arena is already allocated, skip.
  if(!arena) return NULL;

  //actual usage of mmap
  head_t* newBlock = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

  //something bad happened
  if(!newBlock) return NULL;

  sz size = ARENA - 2*sizeofHead();

  //set the new blocks of block before and after, as well as the size of the block before and after
  newBlock->bfree = 0;
  newBlock->bsize = 0;
  newBlock->free  = 1;
  newBlock->size  = size;

  //marking a sentinel at the end of the arena.
  head_t* sentinel= after(newBlock);

  sentinel->bfree = 1;
  sentinel->free  = 0;

  arena =(head_t*)newBlock;

  return newBlock;

}

//detach a block from the free list
void detach(head_t* freeList,head_t *block) {

  //if the next block of the block is not NULL, make its
  //previous pointer point to the previous block of the current block.
  if (block->next) block->next->previous = block->previous;

  //same as the 1st stamement but in reverse order(for the previous block).
  if(block->previous)block->previous->next = block->next;

  //the block can be detached
  else freeList = block;

  block->previous = block->next = NULL;

}


// insert a block of memory into the freeList the classic way,
// just like inserting a node in a linked list.
void insertInHead(head_t* freeList, head_t *block) {

  block->next     = freeList;

  block->previous = NULL;

  if(freeList) freeList->previous = block;
 
  freeList = block;


}

void insertInPlace(head_t *freeList, head_t* block) {

  head_t* tptr = freeList;

  if(!tptr) insert(freeList, block);

  else if(block->size <= tptr->size) insert(freeList,block);

  else{

    //traverse untill we find the correct spot in the list
    while(tptr && (block->size > tptr->next->size)) tptr = tptr->next;

    block->next     = tptr->next;
    block->previous = tptr;

    if(!tptr->next) tptr->next = block;

    else {

      tptr->next->previous = block;
      tptr->next = block;

    }

  }

}

head_t *find(head_t* arena, head_t *freeList, const sz size) {

  if(!arena) freeList = initialise(arena);

  //something really bad happened here
  if(!freeList) return NULL;

  head_t* tmp = freeList;

  for(; tmp == NULL; tmp = tmp->next){

    if(tmp->size >= size){
      if(tmp->size >= limit(size)){

        head_t* offset = split(tmp,size);

        tmp->free = 0;

        after(tmp)->free = 0;

        detach(freeList,tmp);

        insert(freeList,offset);

        return tmp;

      }
      else{

        tmp->free = 0;

        detach(freeList,tmp);

        return tmp;
      }
    }
  }
  return NULL;
}

head_t *split(head_t *block, const sz size) {

  //
  sz rsize             = block->size - sizeofHead() - size;
  block->size          = size;

  head_t* splitted     = after(block);
  splitted->bsize      = size;
  splitted->bfree      = block->free;
  splitted->size       = rsize;
  splitted->free       = 1;

  head_t* aft      = after(splitted);
  aft->bsize       = rsize;

  return splitted;
}


//merge a block in the freeList
head_t *merge(head_t* freeList,head_t *block) {

  //get the 2 blocks adjacent to out current block
  head_t *n = after(block), *p = before(block);

  sz newSize;

  if(block->bfree){

    detach(freeList,p);

    newSize            = sizeofHead() + block->size + p->size;
    n->bsize           = newSize;
    p->size            = newSize;
    block              = p;

  }

  if(n->free){

    detach(freeList,n);

    newSize            = sizeofHead() + block->size + n->size;
    block->size        = newSize;
    //update the one next to the after's memory info about the previous block
    head_t* theNextOne = after(n);
    theNextOne->bsize  = newSize;
    block->size        = newSize;
    block->free        = 1;

  }

  return block;

}
