#include "headAPI.h"

//define both the align and the size of the arena in the file needed
const u16 ALIGN = 8;

const sz ARENA = 64 * 1024;

head_t *after(const head_t *const block) {

  return (head_t*)((char*) block + sizeofHead() + block->size);

}

head_t *before(const head_t *const block) {

  return (head_t*)((char*) block - sizeofHead() - block->size);

}

void initialise(memMod_t* module) {

  if(module->arena) return;

  head_t* newBlock = mmap(NULL, ARENA, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS,-1,0);

  if(!newBlock) return;

  sz size          = ARENA - 2*sizeofHead();

  newBlock->bfree  = 0;
  newBlock->bsize  = 0;
  newBlock->free   = 1;
  newBlock->size   = size;

  head_t* sentinel = after(newBlock);

  sentinel->bfree  = 1;
  sentinel->free   = 0;

  module->arena    = (head_t*)newBlock;

  module->freeList = newBlock;

}

// detach a block from the linked list.
void detach(memMod_t* module, head_t *block) {

  if (block->next) block->next->previous = block->previous;

  if(block->previous)block->previous->next = block->next;

  else module->freeList = block;

}

// insert a block on the head of the linked list with the free nodes.
void _insert(head_t* freeList, head_t *block) {

  block->next              = freeList;

  block->previous          = freeList->previous;

  freeList->previous->next = block;

  freeList->previous       = block;

  freeList                 = block;

}

void insert(head_t *freeList, head_t* block) {

  head_t* tptr = freeList;

  if(!tptr) _insert(freeList, block);

  else if(block->size <= tptr->size) _insert(freeList,block);

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

//find the next block available from the free list
head_t *find(memMod_t* module, const sz size) {

  //iterator
  head_t* tmp = module->freeList;

  //helper variable to ensure we check the first block
  //in the linked list once(without flip it would stop at the 1st iteration)
  //smaller more compact code.
  unsigned int flip = 0;

  for(; tmp != module->freeList && flip == 1; tmp = tmp->next){

    //fake the for loop to check the 1st element in the 1st iteration
    if(flip == 0) flip = 1;

    if(tmp->size >= size){

      if(tmp->size > limit(size)) return split(tmp,size);

      detach(module, tmp);

      tmp->free = 0;

      after(tmp)->bfree = 0;

      return tmp;

    }
  }
  return NULL;
}

// split a block based on its size
head_t *split(head_t *block, const sz size) {

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

head_t *merge(memMod_t* module,head_t *block) {

  //get the 2 blocks adjacent to out current block
  head_t *n = after(block), *p = before(block);

  sz newSize;

  if(p && block->bfree){

    detach(module,p);

    newSize            = sizeofHead() + block->size + p->size;
    n->bsize           = newSize;
    p->size            = newSize;
    block              = p;

  }

  if(n->free){

    detach(module,n);

    newSize            = sizeofHead() + block->size + n->size;
    block->size        = newSize;
    block->size        = newSize;
    block->free        = 1;

  }

  return block;

}

