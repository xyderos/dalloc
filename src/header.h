#include "helpers.h"

//Helper functions for the headAPI

// sizeof the head, clang will probably inline it (24 bytes)
const sz sizeofHead();
// padding for the block we will allocate (8 bytes)
const sz min(const sz);
// minimum size of the block we will allocate in order to be split in at least 2
// blocks, we need to allocate at least 40 bytes (40+24 = 2 * 32 = 24 + 8 + 24 + 8)
const sz limit(const sz);
// retrieve the header
const head_t* magic(const void *const);
// hide the header
const void* hide(const void *const);
//adjust the blocks position
const sz adapt(const sz);
