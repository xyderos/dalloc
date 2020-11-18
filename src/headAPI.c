#include "headAPI.h"

sz used = 24;

head_t *arena=NULL;

head_t* freeList;

head_t *after(const head_t *const block) {

  return (head_t*)((char*) block + sizeofHead() + block->size);

}

head_t *before(const head_t *const block) {

  return (head_t*)((char*) block - sizeofHead() - block->size);

}

head_t *initialise() {

  // if the arena is already allocated, skip.
  if(!arena) return NULL;

  //actual usage of mmap
  head_t* newBlock = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

  printf("Arena is located at: %p \n", newBlock);

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
  head_t* sentinel= after(newBlock);

  sentinel->bfree = 1;
  sentinel->bsize = size;
  sentinel->free  = 0;
  sentinel->size  = 0;

  arena =(head_t*)newBlock;

  return newBlock;

}

//detach a block from the free list
void detach(head_t *block) {

  //if the block doesnt belong to the list return
  if(!block->next || !block->previous){

    printf("This block doesnt belong to the list. \n");
    exit(1);
  }

  //temp pointers to the free list
  head_t* n  = block->next;
  head_t* p  = block->previous;

  //point the next one of the list to the free list
  //
  p->next    = n;
  n->previous= p;

  if(freeList == block ) freeList = n;

}

void insert(head_t *block) {

  head_t* p       = freeList->previous;
  block->next     = freeList;
  block->previous = freeList->previous;
  p->next         = block;

  freeList->previous = block;
  freeList           = block;

}

head_t *split(head_t *block, const sz size) {

  used +=  sizeofHead();

  sz rsize        = block->size - sizeofHead() - size;
  block->size      = rsize;

  head_t* splitted = after(block);
  splitted->bsize  = rsize;
  splitted->bfree  = block->free;
  splitted->size   = size;
  splitted->free   = 0;

  head_t* aft      = after(splitted);
  aft->bsize       = size;
  aft->bfree       = 0;

  return splitted;

}

head_t* find(const sz size){

  head_t* first = freeList;

  if(first->size >= size){

    if(first->size > limit(size)) return split(first, size);

    detach(first);

    first->free = 0;

    head_t* aft = after(first);

    aft->bfree = 0;

    return first;

  }
  else{

    head_t* n = first->next;

    while (n != first) {

      if(n->size >= size){

        if(n->size > limit(size))
          return split(size, used);

        head_t* aft = after(n);

        aft->bfree = 0;

        aft->free = 0;

        detach(n);

        return n;

      }

      n = n->next;
    }
    return NULL;
  }
}

head_t *merge(head_t *block) {

  head_t *n = after(block), *p = before(block);

  sz ns, x=0;

  if(p != NULL && block->bfree){

    used -= sizeofHead();

    detach(p);

    ns = sizeofHead() + 2*block->size;

    block = p;

    block->size = ns;

    x = 1;

  }

  if(n != NULL && n->bfree){

    used -= sizeofHead();

    detach(n);

    ns = sizeofHead() + 2*block->size;

    block->size = ns;

    block->free = 1;

    x = 1;

  }
  return block;
}
