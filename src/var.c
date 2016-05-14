#include "var.h"

struct var *lookup_var;

// ============================================================================
void init_lookup_var(const size_t nvar)
{
  lookup_var = (struct var*) calloc(sizeof(struct var), nvar);
}

// ============================================================================
void delete_lookup_var(const size_t nvar)
{

  for(size_t i=0; i < nvar; ++i)
  {
    free(lookup_var[i].tuple_indexes_max);
    free(lookup_var[i].tuple_indexes_min);
  }
  free(lookup_var);
}

// ============================================================================
void fill_vars(const unsigned long n_tuples,
               const size_t d,
               const size_t k,
               const size_t nvar)
{

  for(size_t i=0; i < nvar; ++i)// TODO: too much memory maybe
  {
    lookup_var[i].tuple_indexes_max = (size_t*) calloc(sizeof(size_t), k);
    lookup_var[i].tuple_indexes_min = (size_t*) calloc(sizeof(size_t), k);
  }
  for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)// TODO: too much memory maybe
    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[i_tuple].tuples[i];
      lookup_var[var_index].avg += lookup_tuple[i_tuple].avg; // TODO: overflow risk
      ++lookup_var[var_index].count;
    }

  size_t beg = 0, end = n_tuples-1;
  while (beg <= end) // TODO: heuristic counting the number of variable that need to be done, if 0=> exist
  {
    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[beg].tuples[i];
      if(lookup_var[var_index].size_tuple_min < k)
        lookup_var[var_index].tuple_indexes_min[lookup_var[var_index].size_tuple_min++] = beg;
    }

    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[end].tuples[i];
      if(lookup_var[var_index].size_tuple_max < k)
        lookup_var[var_index].tuple_indexes_max[lookup_var[var_index].size_tuple_max++] = end;
    }
    ++beg;
    --end;
  }

  // normalize avg for each var
  for(size_t i=0; i < nvar; ++i)
    lookup_var[i].avg /= (double)lookup_var[i].count;

}
