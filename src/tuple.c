#include "tuple.h"

struct tuple *lookup_tuple = NULL;
var_t *tuple_values = NULL;

void init_lookup_tuple(const unsigned long ntuples,
                       const size_t d)
{
  lookup_tuple = (struct tuple*) calloc(sizeof(struct tuple), ntuples);
  tuple_values = (var_t*) calloc(sizeof(var_t), ntuples*d);

  for(size_t i=0; i < ntuples; ++i)
    lookup_tuple[i].values = &tuple_values[i*d];
}

void delete_lookup_tuple()
{
  free(lookup_tuple);
  free(tuple_values);
}

int compare_tuples (const void * a, const void * b)
{
  double xx = ((struct tuple*)a)->avg;
  double yy = ((struct tuple*)b)->avg;
  return (xx > yy) - (xx < yy);
}

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

#define BUF_SIZE 4096
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
        lookup_tuple[i_tuple].values[i] = buffer[n+i];

      union double_var_t avg;
      avg.v[0] = buffer[n+d];
      avg.v[1] = buffer[n+d+1];
      lookup_tuple[i_tuple].avg = avg.d;

      ++i_tuple;
    }
  }
}
#undef BUF_SIZE

void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const size_t d)
{
  const var_t * const value = _tuple->values;

  // There are minimum 2 values per tuple 
  fprintf(file, "%d\t%d\t", value[0], value[1]);

  // any others
  for(size_t i = 2; i < d; ++i)
    fprintf(file, "%d\t", value[i]);

  fprintf(file, "%.10lf\n", _tuple->avg);
}


#ifdef DELTA_PREC
#define PRINT_PRC_D "%.10Lf%c"
#else
#define PRINT_PRC_D "%.10lf%c"
#endif
unsigned long count_tuples_bin_cutoff(FILE* fout5,
                                      size_t d,
                                      double cutoff,
                                      long unsigned n_tuples,
                                      double average,
                                      double std,
                                      bool lookup_tuple_sorted) 
{
  double next;
  unsigned long c = 0;
  if(lookup_tuple_sorted)
  {
    if(d%2)
    {
      size_t i_tuple = 0;
      while(i_tuple < n_tuples && ((next = lookup_tuple[i_tuple].avg) <= cutoff))
      {
        for (size_t i = 0; i < d; i++)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);
        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
        ++i_tuple;
      }
    }
    else 
    {
      size_t i_tuple = n_tuples - 1;
      while(i_tuple < n_tuples  // unsigned, will never be lower than 0
            && ((next = lookup_tuple[i_tuple].avg) >= cutoff))
      {
        for (size_t i = 0; i < d; i++)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);
        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
        --i_tuple;
      }
    }
  }
  else
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      // if next score is in the tail
      if ((!(d%2) && (next >= cutoff)) || ((d%2) && (next <= cutoff))) 
      {
        //TODO: inefficient, need different types for different d
        for (size_t i = 0; i < d; i++)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);

        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
      }
    }
  return c;
}
#undef DELTA_PREC

// calculate standard deviation, min and max delta
double calculate_std_bin(double average,
                         int d,
                         const unsigned long n_tuples,
                         bool lookup_tuple_sorted) 
{
  double next;
  double sum = 0.0;
  double result;
  double min = DBL_MAX;
  double max = -DBL_MAX;
  // TODO: hard code types for different values of d
  if(lookup_tuple_sorted)
  {
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      if (next < min)
        min = next;
      if (next > max)
        max = next;
      sum += pow(next-average, 2);
      //c++;
    }
  }
  else
  {
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      sum += pow(next-average, 2);
    }
    min = lookup_tuple[0].avg;
    max = lookup_tuple[n_tuples-1].avg;
  }
  fprintf(stderr, "Min Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,min, '\n');
  fprintf(stderr, "Max Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,max, '\n');

  result = sum/n_tuples;
  return sqrtl(result);
}

void write_n_tuples_lo(FILE * const __restrict file,
                       const struct tuple * const __restrict tuples,
                       int n,
                       const int d,
                       const uint64_t n_tuples)
{
  if ((uint64_t)n > n_tuples) n = (int)n_tuples;
  for (int i = 0; i < n; ++i)
      write_tuple_to_file(file, &tuples[i], d);
}

void write_n_tuples_hi(FILE * const __restrict file,
                       const struct tuple * const __restrict tuples,
                       int n,
                       const int d,
                       const uint64_t n_tuples)
{
  assert(n_tuples > 0);
  if ((uint64_t)n > n_tuples) n = (int)n_tuples;
  while (n > 0)
      write_tuple_to_file(file, &tuples[--n], d);
}

void write_scoring_histogram(FILE*out4,
                             const unsigned long ntuples,
                             const double b, 
                             const double min,
                             const double max)
{
  assert(b != 0);

  double tmp_line_num = (double)floor((max-min)/b)+1;
  size_t line_num = (size_t) tmp_line_num;

  fprintf(out4, "%.10lf\t%.10lf\n", min, max);
  size_t i_tuples=0;
  double max_bound = min;
  for(size_t i=0; i < line_num; ++i)
  {
    max_bound += b;
    uint32_t counter = 0;
    // ntuples
    while(i_tuples < ntuples && lookup_tuple[i_tuples].avg <= max_bound)
    {
      ++counter;
      ++i_tuples;
    }
    fprintf(out4, "%d\n", counter);
  }
}
