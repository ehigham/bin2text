#include "tuple.h"

struct tuple *lookup_tuple;

void init_lookup_tuple(const unsigned long ntuples)
{
  lookup_tuple = (struct tuple*) calloc(sizeof(struct tuple), ntuples);
}

// ============================================================================
int compare_tuples (const void * a, const void * b)
{
  double xx = ((struct tuple*)a)->avg;
  double yy = ((struct tuple*)b)->avg;
  return (xx > yy) - (xx < yy);
}

// ============================================================================
//TODO: this sort moves the tuples[8], might be unefficient
void sort_tuples_inplace(struct tuple * arr, const unsigned long n_tuples)
{
  // std
  qsort(arr, n_tuples, sizeof(struct tuple), compare_tuples);
}

// ============================================================================
void fill_tuples(FILE * fbin,
                 const int d,
                 const unsigned long n_tuples,
                 const int nvar)
{
  const size_t bufsize = d*sizeof(var_t) + sizeof(double);
  var_t buffer[bufsize];
  unsigned int n;
  size_t i_tuple = 0;
  while (n_tuples > 0) {
    n = fread(buffer, sizeof(var_t), bufsize,  fbin);
    if (!n) break;
    memcpy(&lookup_tuple[i_tuple].avg,&buffer[d],sizeof(double));

    for(size_t i=0; i < d; ++i)
      lookup_tuple[i_tuple].tuples[i] = buffer[i];
    ++i_tuple;
  }
}

// ============================================================================
void fprintf_tuple(FILE * out, struct tuple* tuple, const int d)
{
  fprintf(out, "%lf", tuple->avg);
  for(size_t i=0; i < d; ++i)
    fprintf(out, "\t%d", tuple->tuples[i]);
  fprintf(out, "\n");
}
