
#ifndef UTILS_HASH_SET_HH__
#define UTILS_HASH_SET_HH__

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

struct hash_set
{
  uint32_t **sparse_buckets;
  size_t bucket_size;
  size_t bucket_num;
};

void hash_set_init  (struct hash_set *this,
                     size_t bucket_size,
                     size_t max_size);

void hash_set_delete(struct hash_set *this);

void hash_set_insert(struct hash_set *this,
                     uint32_t key,
                     uint32_t value);

#endif
