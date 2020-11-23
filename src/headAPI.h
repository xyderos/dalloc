#include "header.h"

// API for the head_t


// return the next block of the block given in the function.
head_t* after     (const head_t *const);

// return the previous block of the block given in the function.
head_t* before    (const head_t *const);

// initialise a memory module
// TODO use the constructor attribute to initialise the arena
// or the multiple arenas before the functions are called?
void    initialise(memMod_t*);

// detach a block from the free list that exists in the current memory module
void    detach    (memMod_t*, head_t*);

// return a proper block from the current memory module
head_t* find      (memMod_t *, const sz);

void insert(head_t*, head_t*);

// split a block into 2 parts that are needed
head_t* split     (head_t*, const sz);

//merge adjacent blocks
head_t* merge     (memMod_t*,head_t*);

