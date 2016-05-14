#include "tuple.h"

struct tuple *lookup_tuple;

void init_lookup_tuple(const unsigned long ntuples)
{
  lookup_tuple = (struct tuple*) calloc(sizeof(struct tuple), ntuples);
}

void delete_lookup_tuple()
{
  free(lookup_tuple);
}

int compare_tuples (const void * a, const void * b)
{
  double xx = ((struct tuple*)a)->avg;
  double yy = ((struct tuple*)b)->avg;
  return (xx > yy) - (xx < yy);
}

//TODO: this sort moves the tuples[8], might be unefficient
void sort_tuples_inplace(struct tuple * arr, const unsigned long n_tuples)
{
  // std
  qsort(arr, n_tuples, sizeof(struct tuple), compare_tuples);
}

union double_var_t
{
  var_t v[2];
  double d;
};

#define BUF_SIZE 512
void fill_tuples(FILE * fbin,
                 const size_t d,
                 const unsigned long n_tuples)
{
  const size_t tuplesize = d + (sizeof(double)/sizeof(var_t));
  const size_t bufsize = BUF_SIZE * tuplesize;

  var_t buffer[bufsize];
  size_t n;
  size_t i_tuple = 0;
  while ((n = fread(buffer, sizeof(var_t), bufsize,  fbin)))
  {
    assert(i_tuple <= n_tuples);
    while(n)
    {
      n -= tuplesize;
      for(size_t i=0; i < d; ++i)
        lookup_tuple[i_tuple].tuples[i] = buffer[n+i];

      union double_var_t avg;
      avg.v[0] = buffer[n+d];
      avg.v[1] = buffer[n+d+1];
      lookup_tuple[i_tuple].avg = avg.d;

      ++i_tuple;
    }
  }
}

void fprintf_tuple(FILE * out, struct tuple* tuple, const size_t d)
{
  fprintf(out, "%lf", tuple->avg);
  for(size_t i=0; i < d; ++i)
    fprintf(out, "\t%d", tuple->tuples[i]);
  fprintf(out, "\n");
}

void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const size_t d)
{
  const var_t * const value = _tuple->tuples;

  // There are minimum 2 values per tuple 
  fprintf(file, "%d\t%d\t", value[0], value[1]);

  // any others
  for(size_t i = 2; i < d; ++i)
    fprintf(file, "%d\t", value[i]);

  fprintf(file, "%.10lf\n", _tuple->avg);
}


