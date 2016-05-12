// Paul Draghicescu
// pdraghicescu@pnri.org
//

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "run.h"
#include "string.h"
#include <stdint.h>
#include "tuple.h"
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef DELTA_PREC
#define PRINT_PRC_D "%.10Lf%c"
#else
#define PRINT_PRC_D "%.10lf%c"
#endif


//TODO: Implement this to clear up any malloc'd mem
//void on_exit(void) {}

// write all tuples above cutoff
unsigned long count_tuples_bin_cutoff(const option_t* opt, FILE* fbin, int d, double cutoff, long unsigned n_tuples, double average, double std) {
    double next;
    unsigned long c = 0;
    int skip = d+sizeof(double)/sizeof(int);
    int i,j;
    int *ip;
    int bufsize = (d*TUP_BUF_SIZE)+((sizeof(double)/sizeof(int))*TUP_BUF_SIZE);
    int buffer[bufsize];
    unsigned int n;
    FILE* fout5 = fopen("out5.txt","w");
    if (!fout5) {
        fprintf(stderr, "Error: couldn't open out5.txt\n");
        return -1;
    }
    while(n_tuples > 0)  {
        n = fread(buffer, sizeof(int), bufsize,  fbin);
        if (!n) break;
        for (j = 0; (j < n) && (--n_tuples > 0); j+=skip) {
            memcpy(&next,&buffer[j+d],sizeof(double));
            // if next score is in the tail
            if ((!(d%2) && (next >= cutoff)) || ((d%2) && (next <= cutoff))) {
                //TODO: inefficient, need different types for different d
                for (i = 0, ip = &buffer[j]; i < d; i++, ip++) {
                    fprintf(fout5,"%d\t",*ip);
                }
                fprintf(fout5, PRINT_PRC_D,next,'\t');
                fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
                c++;
            }
        }
    }
    fclose(fout5);
    return c;
}

// calculate standard deviation, min and max delta
double calculate_std_bin(FILE* fbin, double average, int d, unsigned long n_tuples) {
    double next;
    unsigned long c = 0;
    double sum = 0.0;
    double result;
    double min = DBL_MAX;
    double max = -DBL_MAX;
    int skip = d+sizeof(double)/sizeof(int);
    int j;
    // TODO: hard code types for different values of d
    int bufsize = (d*TUP_BUF_SIZE)+((sizeof(double)/sizeof(int))*TUP_BUF_SIZE);
    int buffer[bufsize];
    unsigned int n;
    while (n_tuples > 0) {
        n = fread(buffer, sizeof(int), bufsize,  fbin);
        if (!n) break;
        for (j = 0; (j < n) && (--n_tuples > 0); j+=skip) {
            memcpy(&next,&buffer[j+d],sizeof(double));
            if (next < min)
                min = next;
            if (next > max)
                max = next;
            sum += pow(next-average, 2);
            c++;
        }
    }
    fprintf(stderr, "Min Delta:\t\t");
    fprintf(stderr, PRINT_PRC_D,min, '\n');
    fprintf(stderr, "Max Delta:\t\t");
    fprintf(stderr, PRINT_PRC_D,max, '\n');
    result = sum/c;
    return sqrtl(result);
}

// read and store header information
int get_header(FILE* fbin, int *d, int *n_vars, unsigned long *n_tuples, double *average) {
    int err;
    err = fread(d, 1, sizeof(int), fbin);
    if (!err) {
        fprintf(stderr, "Error: couldn't read dummy 0 from file\n");
        return -1;
    }
    err = fread(d, 1, sizeof(int), fbin);
    if (!err) {
        fprintf(stderr, "Error: couldn't read dimension from file\n");
        return -1;
    }
    err = fread(n_vars, 1, sizeof(int), fbin);
    if (!err) {
        fprintf(stderr, "Error: couldn't read n_vars from file\n");
        return -1;
    }
    err = fread(n_tuples, 1, sizeof(unsigned long), fbin);
    if (!err) {
        fprintf(stderr, "Error: couldn't read n_tuples from file\n");
        return -1;
    }
    err = fread(average, 1, sizeof(double), fbin);
    if (!err) {
        fprintf(stderr, "Error: couldn't read average from file\n");
        return -1;
    }
    return 0;
}

// Thomas: Code for out1/2/3/4 TODO: test it =p

typedef int var_t;
struct tuple *lookup_tuple;
struct var *lookup_var;

extern struct output_filenames output_files;

void init_lookup_tables(const int nvar,
                        const unsigned long ntuples)
{
  lookup_tuple = (struct tuple*) calloc(sizeof(struct tuple), ntuples);
  lookup_var = (struct var*) calloc(sizeof(struct var), nvar);
}

//void fill_lookup_tables(FILE* fbin, const int d,
//                        const unsigned long n_tuples, const int nvar)
//{
//  const size_t bufsize = d*sizeof(var_t) + sizeof(double);
//  var_t buffer[bufsize];
//  unsigned int n;
//  size_t i_tuple = 0;
//  while (n_tuples > 0) {
//    n = fread(buffer, sizeof(var_t), bufsize,  fbin);
//    if (!n) break;
//
//    //lookup_tuple[i_tuple].avg = *(double*)(buffer+d*sizeof(var_t)); // get the AVG on double TODO: strict alias?
//    memcpy(&lookup_tuple[i_tuple].avg,&buffer[d],sizeof(double));
//
//    for(size_t i=0; i < d; ++i)
//    {
//      lookup_tuple[i_tuple].tuples[i] = buffer[i];
//      lookup_var[buffer[i]].avg += lookup_tuple[i_tuple].avg; // TODO: overflow risk
//      ++lookup_var[buffer[i]].count;
//    }
//    ++i_tuple;
//  }
//
//  // normalize avg for each var
//  for(size_t i=0; i < nvar; ++i)
//    lookup_var[i].avg /= lookup_var[i].count;
//}

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

FILE * create_file_if_not_exists(const char * const filename)
{
    FILE * file = fopen(filename,"wx");
    if (file != NULL) return file;

    fprintf(stderr, "Could not create file %s\n", filename);
    fprintf(stderr, "Please verify it does not already exit.\n");
    exit(-1);
}


void write_n_tuples_hi(const struct tuple * const tuples,
                       const int n,
                       const int d)
{

    FILE * file = create_file_if_not_exists(output_files.out1_name);

}


// Need to be called after tuple sorting
void fill_vars(const unsigned long n_tuples,
               const int d,
               const int k,
               const int nvar)
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
    lookup_var[i].avg /= lookup_var[i].count;

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

void fprintf_tuple(FILE * out, struct tuple* tuple, const int d)
{
  fprintf(out, "%lf", tuple->avg);
  for(size_t i=0; i < d; ++i)
    fprintf(out, "\t%d", tuple->tuples[i]);
  fprintf(out, "\n");
}

void output_out3(const int nvar, const int k, const int d)
{
  FILE * out3 = fopen("out3.txt", "wx");
  if (!out3)
  {
    fprintf(stderr, "%s already exsiting!\n", output_files.out3_name);
    exit(1);
  }

  for(var_t i=0; i < nvar; ++i)
  {
    struct var * var = &lookup_var[i];
    //if(var->count == 0) continue;
    fprintf(out3, "%d\t%lf\n", i, var->avg);
    for(size_t tindex = 0; tindex < var->size_tuple_max; ++tindex)
      fprintf_tuple(out3, &lookup_tuple[var->tuple_indexes_max[tindex]], d);

    for(size_t tindex = 0; tindex < var->size_tuple_min; ++tindex)
      fprintf_tuple(out3, &lookup_tuple[var->tuple_indexes_min[tindex]], d);
  }
  fclose(out3);
}

void output_out4(struct tuple * tuples,
                 const unsigned long ntuples,
                 const double b, 
                 const double min,
                 const double max)
{
  assert(b != 0);
  FILE * out4 = fopen(output_files.out4_name, "wx");
  if (!out4)
  {
    fprintf(stderr, "%s already exsiting!\n", output_files.out4_name);
    exit(1);
  }
  size_t line_num = floor((max-min)/b)+1;

  fprintf(out4, "%lf\t%lf\n", min, max);
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
  fclose(out4);
}


// main function 
int run(option_t *opt) {
    FILE* fbin1;
    FILE* fbin2;
    double average = 0.0;
    double std = 0.0;
    double cutoff;
    int d, n_vars, sign=1;
    unsigned long n_tuples, c;

    // open output file, get dimension and other information from header
    fbin1 = fopen(opt->in_file1, "rb");
    if (!fbin1) {
        fprintf(stderr, "Error: couldn't open header file \"%s\".\n",opt->in_file1);
        return -1;
    }
    fbin2 = fopen(opt->in_file2, "rb");
    if (!fbin2) {   
        fprintf(stderr, "Error: couldn't open binary file 2\"%s\".\n",opt->in_file1);
        return -1;
    }
    fseek(fbin1, 0L, SEEK_END);
    if (!ftell(fbin1)) {
        fprintf(stderr, "Error: empty binary file 1  \n");
        return -1;
    }
    fseek(fbin2, 0L, SEEK_END);
    if (!ftell(fbin2)) {
        fprintf(stderr, "Error: empty binary file 2\n");
        return -1;
    }
    rewind(fbin1);
    rewind(fbin2);
    get_header(fbin1, &d, &n_vars, &n_tuples, &average);
    if (fclose(fbin1) != 0) {
        fprintf(stderr, "Error closing binary file 2.\n");
        return -1;
    }
    sign = pow(-1,d);
    fprintf(stderr, "Dimension:\t\t%d\n",d);
    fprintf(stderr, "Number of Variables:\t%d\n",n_vars);
    fprintf(stderr, "Number of Tuples:\t%ld\n",n_tuples);
    fprintf(stderr, "Average:\t\t%.10f\n",average);

    if(opt->n !=0 || opt->k != 0)
    {
      init_lookup_tables(n_vars,
                         n_tuples);
      fill_tuples(fbin2, d,
                  n_tuples, n_vars);
      sort_tuples_inplace(lookup_tuple, n_tuples);
      fill_vars(n_tuples, d, opt->k, n_vars);
      rewind(fbin2);
      printf("%.10f %.10f\n", lookup_tuple[0].avg, lookup_tuple[n_tuples-1].avg);

      if(opt->k != 0)
        output_out3(n_vars, opt->k, d);

      if(opt->b != 0)
        output_out4(lookup_tuple, n_tuples, opt->b, lookup_tuple[0].avg, lookup_tuple[n_tuples-1].avg);
    }

    // if -s is passed, pass through the file once to calculate std, and again to create out5.txt
    if (opt->s_option) {
        std = calculate_std_bin(fbin2, average, d, n_tuples+1);
        rewind(fbin2);
        cutoff = average + sign*std*opt->s_std;
        fprintf(stderr, "St. Dev.:\t\t%.10f\n",std);
        fprintf(stderr, "Cutoff:\t\t\t%.10f\n",cutoff);

        // count and print tuples above cutoff
        c = count_tuples_bin_cutoff(opt, fbin2, d, cutoff, n_tuples+1, average,std);
        fprintf(stderr, "Tuples Above Cutoff:\t%ld\t(%.2f%%)\n", c, 100.0*c/n_tuples);
    }

    if (fclose(fbin2) != 0) {
        fprintf(stderr, "Error closing binary file 2.\n");
        return -1;
    }
    return 0;
}

