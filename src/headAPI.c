#include "headAPI.h"

head_t *nxt(const head_t *const block) {

  return (head_t*)((char*) block + sizeofHead() + block->size);

}

head_t *prv(const head_t *const block) {

  return (head_t*)((char*) block - sizeofHead() - block->size);

}

head_t *init(head_t *arena) {

  // if the arena is already allocated, skip.
  if(!arena) return NULL;

  head_t* newBlock = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

  //something bad happened
  if(!newBlock) return NULL;

  //return the size of the arena
  sz size = ARENA - 2*sizeofHead();

  //set the new blocks of block before and after, as well as the size of the block before and after
  newBlock->bfree = 0;
  newBlock->bsize = 0;
  newBlock->free  = 1;
  newBlock->size  = size;

  //marking a sentinel at the end of the arena.
  head_t* sentinel= nxt(newBlock);

  sentinel->bfree = 1;
  sentinel->bsize = size;
  sentinel->free  = 0;
  sentinel->size  = 0;

  arena = newBlock;

  return newBlock;

}

//detach a block from the free list
void detach(head_t *fList, head_t *block) {

  //if the block doesnt belong to the list return 
  if(!block->next || !block->previous) return;

  //temp pointers to the free list
  head_t* n  = block->next;
  head_t* p  = block->previous;

  //point the next one of the list to the free list
  //
  p->next    = n;
  n->previous= p;

  if(fList == block ) fList = n;

}

void insert(head_t *fList, head_t *block) {

  head_t* p       = block->previous;

  block->next     = fList;
  block->previous = fList->previous;
  p->next         = block;
  fList->previous = block;
  fList           = block;

}

head_t *split(head_t *block, const sz size, sz used) {

  used            += size;
  sz resize        =  block->size - sizeofHead() - size;

  block->size      = resize;

  head_t* splitted = nxt(block);
  splitted->bsize  = resize;
  splitted->bfree  = block->free;
  splitted->size   = size;
  splitted->free   = 0;

  head_t* aft      = nxt(splitted);
  aft->bsize       = size;
  aft->bfree       = 0;

  return splitted;

}

head_t* find(head_t* fList, const sz size, const sz used){

  head_t* first = fList;

  if(first->size >- size){

    if(first->size > limit(size)) return split(first, size, used);

    detach(fList, first);

    first->free = 0;

    head_t* after = nxt(first);

    after->bfree = 0;

    return first;

  }
  else{

    head_t* n = first->next;

    while (n != first) {

      if(n->size >= size){

        if(n->size > limit(size))
          return split(fList, size, used);

        head_t* after = nxt(n);

        after->bfree = 0;

        after->free = 0;

        detach(fList,n);

        return n;

      }

      n = n->next;
    }
    return NULL;
  }
}

