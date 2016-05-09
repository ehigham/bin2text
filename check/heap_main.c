#include "../utils/heap.h"
#include <stdio.h>

int main(int argc, const char **argv)
{

  struct heap max_heap;
  struct heap min_heap;

  heap_init(&max_heap, 10);
  heap_init(&min_heap, 10);

  for(uint32_t i=0; i < 10; ++i)
  {
    heap_max_insert(&max_heap, i);
    heap_min_insert(&min_heap, i);
  }

  for(uint32_t i=0; i < 10; ++i)
  {
    assert(heap_max_peek(&max_heap) == (10 - 1 - i));
    assert(heap_min_peek(&min_heap) == i);
  }
  return 0;
}
