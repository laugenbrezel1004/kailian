#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include "error.h"

typedef struct {
    void *ptr;
    size_t size;
} MemoryBlock;

MemoryBlock* memory_alloc(size_t size);
void memory_free(MemoryBlock **block);
ErrorCode memory_resize(MemoryBlock *block, size_t new_size);

#define SAFE_FREE(ptr) do { \
    if (ptr) { \
        memory_free((MemoryBlock**)&(ptr)); \
        ptr = NULL; \
    } \
} while(0)

#endif 