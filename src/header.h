#pragma once

#include <sys/mman.h>

#define NULL ((void*)0)

//try to use the u16 to keep info for the neighbour blocks
typedef unsigned short u16;

//size implementation
typedef unsigned long sz;

//definition for hte byte allignment
extern const u16 ALIGN;

//how big is the arena?
extern const sz ARENA;

//a packed struct to hold some information each block, a header
typedef struct __attribute__((packed)) _head {

  u16 bfree, bsize, free, size;

  struct _head *next, *previous;

} head_t;

// memory module consists of the freelist and the arena
// TODO: is there a way to use a union to hold all this info?
typedef struct _memmod {

  head_t* arena;
  head_t* freeList;

}memMod_t;

//prefer function definitions over defines, type safe and scope is secured.

// sizeof the head, clang will probably inline it (24 bytes)
sz sizeofHead();
// padding for the block we will allocate (8 bytes)
sz min(const sz);
// minimum size of the block we will allocate in order to be split in at least 2
// blocks, we need to allocate at least 40 bytes (40+24 = 2 * 32 = 24 + 8 + 24 + 8)
sz limit(const sz);
// retrieve the header
head_t* magic(const void *const);
// hide the header
void* hide(const void *const);
//adjust the blocks position
sz adapt(const sz);
