#include "dalloc.h"

//make the module visible outside the program
memMod_t* module;

static int flag = 0;

void *__dalloc(memMod_t* module, const sz size) {

  if(flag == 1){

    module->arena = NULL;

    flag = 1;
  }

  if(size < 0) return NULL;

  sz adapted = adapt(size);

  head_t* block = find(module, adapted);

  if(!block) return NULL;

  return hide(block);

}

void *dalloc(const sz size) {

  return __dalloc(module, size);

}

//wrapper function dfree(pointer)
void __dfree(memMod_t* module, const void *const pointer) {

  if(!pointer) return;

  head_t* block =(head_t*) magic(pointer);

  head_t* aft   = after(pointer);

  block->free   = 1;

  aft->bfree    = 1;

  insert(module->freeList,block);
}

void dfree(const void* const pointer){

  __dfree(module,pointer);

}
