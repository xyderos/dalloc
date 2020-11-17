#include "header.h"
#include <stdlib.h>
#include <sys/mman.h>

// API for the head_t
head_t*       nxt(const head_t *const);

head_t*       prv(const head_t *const);

head_t*       init(head_t*);

void          detach(head_t*, head_t*);

void          insert(head_t *, head_t *);

head_t*       split(head_t *, const sz, sz);

head_t*       find(head_t*, const sz, const sz used);

head_t*       merge(head_t*, const sz);

