#pragma once

typedef unsigned short u16;

typedef unsigned long sz;

extern const u16 ALIGN;

extern const sz ARENA;

typedef struct __attribute__((packed)) _head {

  u16 bfree, bsize, free, size;

  struct _head *next, *previous;

} head_t;

//Helper functions for the headAPI

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
