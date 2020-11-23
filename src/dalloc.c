#include "dalloc.h"

sz used = 24;

head_t* freeList;

//wrapper function for dalloc(size)
void *__dalloc(head_t* arena, const sz size) {

  if(size < 0) return NULL;

  sz adapted = adapt(size);

  head_t* block = find(arena,freeList, adapted);

  if(!block) return NULL;

  return hide(block);

}

void *dalloc(const sz size) {

  static head_t *arena =NULL;

  return __dalloc(arena, size);

}

//wrapper function dfree(pointer)
void __dfree(const void *const pointer) {

  if(!pointer) return;

  head_t* block =(head_t*) magic(pointer);

  head_t* aft   = after(pointer);

  block->free   = 1;

  aft->bfree    = 1;

  insert(freeList,block);
}

void dfree(const void* const pointer){

  __dfree(pointer);

}
