#include "header.h"
#include <stdlib.h>
#include <sys/mman.h>

// API for the head_t
head_t*       after(const head_t *const);

head_t*       before(const head_t *const);

head_t*       initialise(head_t*);

void          detach(head_t*, head_t*);

void          insert(head_t*, head_t*);

head_t*       split(head_t *, const sz);

head_t*       find(head_t*,head_t*,const sz);

head_t*       merge(head_t*,head_t*);

