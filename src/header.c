#include "header.h"

sz sizeofHead(){

  return sizeof(head_t);

}

sz min(const sz size){

  return size > 8 ? size : 8;

}
sz limit(const sz size){

  return min(0) + sizeofHead() + size;

}

head_t* magic(const void* const ptr){

  return (head_t*)ptr - 1;

}

void* hide(const void* const ptr) {

  return (void*)((head_t*) ptr -1);

}

sz adapt(const sz size) {

  return (size < 8 ) ? min(size) : min(size) + min(0) - size % min(0);

}

