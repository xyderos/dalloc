#include "header.h"

const sz sizeofHead(){

  return sizeof(head_t);

}

const sz min(const sz size){

  return size > 8 ? size : 8;

}
const sz limit(const sz size){

  return min(0) + sizeofHead() + size;

}

const head_t* magic(const void* const ptr){

  return (head_t*)ptr - 1;

}

const void* hide(const void* const ptr) {

  return (void*)((head_t*) ptr -1);

}
