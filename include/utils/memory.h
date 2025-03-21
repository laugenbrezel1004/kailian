#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

void *safe_malloc(size_t size);
void *safe_calloc(size_t count, size_t size);
void safe_free(void **ptr);

#define SAFE_FREE(ptr) do { safe_free((void**)&(ptr)); } while(0)

#endif // MEMORY_H 