
#ifndef UTILS_HEAP_H__
#define UTILS_HEAP_H__

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define value_type uint32_t

// Binary heap structure
struct heap
{
  size_t size;
  size_t allocated_size;
  value_type * data;
};

// Heap ctro & dtor
void heap_init(struct heap *this, const size_t N);
void heap_delete(struct heap *this);

// Max heap operations
void heap_max_insert(struct heap *this, const value_type newkey);
value_type heap_max_peek(struct heap *this);

// Min Heap operations
void heap_min_insert(struct heap *this, const value_type newkey);
value_type heap_min_peek(struct heap *this);



#undef value_type

#endif
