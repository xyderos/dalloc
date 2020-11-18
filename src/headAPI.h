#include "header.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <stdio.h>

// API for the head_t
head_t*       after(const head_t *const);

head_t*       before(const head_t *const);

head_t*       initialise();

void          detach(head_t*);

void          insert(head_t*);

head_t*       split(head_t *, const sz);

head_t*       find(const sz);

head_t*       merge(head_t*);

