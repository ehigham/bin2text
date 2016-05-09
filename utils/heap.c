#include "heap.h"

#define value_type uint32_t
#define swap(a, b) {value_type tmp = a; a = b; b = tmp;}
 
// ============================================================================
void heap_init(struct heap *this, const size_t N)
{
  this->size=0;
  this->allocated_size=N;
  this->data = (value_type*)malloc(sizeof(value_type)*N);
}

// ============================================================================
void heap_delete(struct heap *this, const size_t N)
{
  assert(this->data != NULL);
  free(this->data);
}

// ============================================================================
void max_insert(struct heap *this, const value_type content)                      
{                                                                              
  this->data[this->size++] = content;                                          
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
value_type max_peek(struct heap *this)
{
  assert(this->data);
  assert(this->size != 0);

  value_type res = this->data[0];
  this->data[0] = this->data[this->size-1];
  --this->size;

  size_t current_id= 0;
  size_t left_chil_id = 2*current_id + 1;
  size_t right_chil_id = 2*current_id + 2;

  while(current_id <= this->size/2)
    if(this->data[current_id] >= this->data[left_chil_id])
    {
      swap(this->data[current_id], this->data[left_chil_id]);
      current_id = left_chil_id;
      left_chil_id = 2*current_id + 1;
      right_chil_id = 2*current_id + 2;
    }
    else if(this->data[current_id] >= this->data[right_chil_id])
    {
      swap(this->data[current_id], this->data[right_chil_id]);
      current_id = right_chil_id;
      left_chil_id = 2*current_id + 1;
      right_chil_id = 2*current_id + 2;

    }
    else break;

  return res;
}

// ============================================================================
void min_insert(struct heap *this, const value_type content)                      
{                                                                              
  assert(this->data);
  this->data[this->size++] = content;                                          
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
value_type min_peek(struct heap *this)
{
  assert(this->data);
  assert(this->size != 0);

  value_type res = this->data[0];
  this->data[0] = this->data[this->size-1];
  --this->size;

  size_t current_id= 0;
  size_t left_chil_id = 2*current_id + 1;
  size_t right_chil_id = 2*current_id + 2;

  while(current_id <= this->size/2)
    if(this->data[current_id] <= this->data[left_chil_id])
    {
      swap(this->data[current_id], this->data[left_chil_id]);
      current_id = left_chil_id;
      left_chil_id = 2*current_id + 1;
      right_chil_id = 2*current_id + 2;
    }
    else if(this->data[current_id] <= this->data[right_chil_id])
    {
      swap(this->data[current_id], this->data[right_chil_id]);
      current_id = right_chil_id;
      left_chil_id = 2*current_id + 1;
      right_chil_id = 2*current_id + 2;

    }
    else break;

  return res;
}
