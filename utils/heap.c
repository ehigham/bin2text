#include "heap.h"

#define value_type uint32_t
#define swap(a, b) {value_type tmp = a; a = b; b = tmp;}
 
// ============================================================================
inline void helper_push_back(struct heap *this, const value_type content)
{
  assert(this->size+1 <= this->allocated_size);
  this->data[this->size++] = content;                                          
}

// ============================================================================
inline void helper_pop_back(struct heap *this)
{
  assert(this->size != 0);
  --this->size;
}

// ============================================================================
inline value_type helper_back(struct heap *this)
{
  assert(this->size != 0);
  return this->data[this->size-1];
}

// ============================================================================
inline value_type helper_front(struct heap *this)
{
  assert(this->size != 0);
  return this->data[0];
}

// ============================================================================
void heap_init(struct heap *this, const size_t N)
{
  this->size=0;
  this->allocated_size=N;
  this->data = (value_type*)malloc(sizeof(value_type)*N);
}

// ============================================================================
void heap_delete(struct heap *this)
{
  assert(this->data != NULL);
  free(this->data);
}

// ============================================================================
void heap_max_insert(struct heap *this, const value_type content)                      
{                                                                              
  helper_push_back(this, content);
  size_t current_id = this->size-1;                                            
  size_t parent_id  = current_id/2;                                            
                                                                               
  while(current_id != 0)                                                       
    if(this->data[current_id] > this->data[parent_id])
    {                                                                          
      swap(this->data[parent_id], this->data[current_id]);                     
      
      current_id = parent_id;                                                  
      parent_id = current_id/2;                                                
    }                                                                          
    else                                                                       
      break;                                                                   
}                                                                              

// ============================================================================
value_type heap_max_peek(struct heap *this)
{
  assert(this->data);
  assert(this->size != 0);

  value_type res = helper_front(this);
  this->data[0] = helper_back(this);
  helper_pop_back(this);

  size_t current_id=0;

  while(current_id < this->size/2)
  {
    size_t left_chil_id  = 2*current_id + 1;
    size_t right_chil_id = 2*current_id + 2;

    size_t to_swap = left_chil_id;

    // Transitivity trick
    if(this->data[left_chil_id] < this->data[right_chil_id])
      to_swap = right_chil_id;

    if(this->data[to_swap] > this->data[current_id]) 
    {
      swap(this->data[current_id], this->data[to_swap]);
      current_id = to_swap;
    }
    else break;
  }

  return res;
}

// ============================================================================
void heap_min_insert(struct heap *this, const value_type content)                      
{                                                                              
  assert(this->data);

  helper_push_back(this, content);
  size_t current_id = this->size-1;                                            
  size_t parent_id  = current_id/2;                                            
                                                                               
  while(current_id != 0)                                                       
    if(this->data[current_id] < this->data[parent_id])
    {                                                                          
      swap(this->data[parent_id], this->data[current_id]);                     
      current_id = parent_id;                                                  
      parent_id = current_id/2;                                                
    }                                                                          
    else                                                                       
      break;                                                                   
}                                                                              

// ============================================================================
value_type heap_min_peek(struct heap *this)
{
  assert(this->data);
  assert(this->size != 0);

  value_type res = helper_front(this);
  this->data[0] = helper_back(this);
  helper_pop_back(this);

  size_t current_id=0;

  while(current_id < this->size/2)
  {
    size_t left_chil_id  = 2*current_id + 1;
    size_t right_chil_id = 2*current_id + 2;

    size_t to_swap = left_chil_id;

    // Transitivity trick
    if(this->data[left_chil_id] > this->data[right_chil_id])
      to_swap = right_chil_id;

    if(this->data[to_swap] < this->data[current_id]) 
    {
      swap(this->data[current_id], this->data[to_swap]);
      current_id = to_swap;
    }
    else break;
  }
  return res;
}
