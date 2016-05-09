#include "hashset.h"

size_t hash(uint32_t key, size_t div)
{
  return key/div;
}

void hash_set_init  (struct hash_set *this,
                     size_t bucket_size,
                     size_t max_size)
{
  size_t bucket_num = max_size / bucket_size;
  this->sparse_buckets = (uint32_t**)malloc(sizeof(uint32_t*) * bucket_num);
  for(size_t i=0; i < this->bucket_num; ++i)
    this->sparse_buckets[i] = NULL;
}

void hash_set_delete(struct hash_set *this)
{
  for(size_t i=0; i < this->bucket_num; ++i)
    if(this->sparse_buckets[i])
      free(this->sparse_buckets[i]);
}

void hash_set_insert(struct hash_set *this,
                     uint32_t key,
                     uint32_t value)
{
  size_t index = hash(key, this->bucket_num);
  assert(index < this->bucket_size * this->bucket_num);
  if(!this->sparse_buckets[index])
    this->sparse_buckets[index] = (uint32_t*)malloc(sizeof(uint32_t) * this->bucket_size);
  this->sparse_buckets[index][key % index] = value;

}
