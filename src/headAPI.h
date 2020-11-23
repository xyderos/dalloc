#include "header.h"
#include <stdlib.h>
#include <sys/mman.h>

// API for the head_t
head_t*       after(const head_t *const);

head_t*       before(const head_t *const);

// arena could be passed as a global variable and the result will
// be assigned into the free list.
head_t*       initialise(head_t*);

//needs the freeList as an argument, freeList can be a global in the dalloc.c file
void          detach(head_t*, head_t*);

void          insertInHead(head_t*, head_t*);

void          insertInPlace(head_t*, head_t*);

head_t*       split(head_t *, const sz);

//needs both the arena and the freelist
head_t*       find(head_t*,head_t*,const sz);

//needs the freeList
head_t*       merge(head_t*,head_t*);

