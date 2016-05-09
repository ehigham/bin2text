
#ifndef UTILS_HEAP_H__
#define UTILS_HEAP_H__

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define value_type uint32_t

struct heap
{
  size_t size;
  size_t allocated_size;
  value_type * data;
};

#undef value_type

#endif
