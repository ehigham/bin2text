// Paul Draghicescu
// pdraghicescu@pnri.org
//

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "run.h"
#include "string.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "tuple.h"
#include "var.h"

#ifdef DELTA_PREC
#define PRINT_PRC_D "%.10Lf%c"
#else
#define PRINT_PRC_D "%.10lf%c"
#endif


//TODO: Implement this to clear up any malloc'd mem
//void on_exit(void) {}

// write all tuples above cutoff
//unsigned long count_tuples_bin_cutoff(const option_t* opt, FILE* fbin, int d, double cutoff, long unsigned n_tuples, double average, double std) {
//    double next;
//    unsigned long c = 0;
//    int skip = d+sizeof(double)/sizeof(int);
//    int i,j;
//    int *ip;
//    int bufsize = (d*TUP_BUF_SIZE)+((sizeof(double)/sizeof(int))*TUP_BUF_SIZE);
//    int buffer[bufsize];
//    unsigned int n;
//    FILE* fout5 = fopen("out5.txt","w");
//    if (!fout5) {
//        fprintf(stderr, "Error: couldn't open out5.txt\n");
//        return -1;
//    }
//    while(n_tuples > 0)  {
//        n = fread(buffer, sizeof(int), bufsize,  fbin);
//        if (!n) break;
//        for (j = 0; (j < n) && (--n_tuples > 0); j+=skip) {
//            memcpy(&next,&buffer[j+d],sizeof(double));
//            // if next score is in the tail
//            if ((!(d%2) && (next >= cutoff)) || ((d%2) && (next <= cutoff))) {
//                //TODO: inefficient, need different types for different d
//                for (i = 0, ip = &buffer[j]; i < d; i++, ip++) {
//                    fprintf(fout5,"%d\t",*ip);
//                }
//                fprintf(fout5, PRINT_PRC_D,next,'\t');
//                fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
//                c++;
//            }
//        }
//    }
//    fclose(fout5);
//    return c;
//}
//
//// calculate standard deviation, min and max delta
//double calculate_std_bin(FILE* fbin, double average, int d, unsigned long n_tuples) {
//    double next;
//    unsigned long c = 0;
//    double sum = 0.0;
//    double result;
//    double min = DBL_MAX;
//    double max = -DBL_MAX;
//    int skip = d+sizeof(double)/sizeof(int);
//    int j;
//    // TODO: hard code types for different values of d
//    int bufsize = (d*TUP_BUF_SIZE)+((sizeof(double)/sizeof(int))*TUP_BUF_SIZE);
//    int buffer[bufsize];
//    unsigned int n;
//    while (n_tuples > 0) {
//        n = fread(buffer, sizeof(int), bufsize,  fbin);
//        if (!n) break;
//        for (j = 0; (j < n) && (--n_tuples > 0); j+=skip) {
//            memcpy(&next,&buffer[j+d],sizeof(double));
//            if (next < min)
//                min = next;
//            if (next > max)
//                max = next;
//            sum += pow(next-average, 2);
//            c++;
//        }
//    }
//    fprintf(stderr, "Min Delta:\t\t");
//    fprintf(stderr, PRINT_PRC_D,min, '\n');
//    fprintf(stderr, "Max Delta:\t\t");
//    fprintf(stderr, PRINT_PRC_D,max, '\n');
//    result = sum/c;
//    return sqrtl(result);
//}

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

typedef int var_t;


unsigned long count_tuples_bin_cutoff(const option_t* opt, int d, double cutoff, long unsigned n_tuples, double average, double std) 
{
  double next;
  unsigned long c = 0;
  FILE* fout5 = fopen(output_files.out5_name,"wx");
  if (!fout5) 
  {
    fprintf(stderr, "Error: couldn't open %s\n", output_files.out5_name);
    return -1;
  }
  for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
  {
    next = lookup_tuple[i_tuple].avg;
    // if next score is in the tail
    if ((!(d%2) && (next >= cutoff)) || ((d%2) && (next <= cutoff))) 
    {
      //TODO: inefficient, need different types for different d
      for (int i = 0; i < d; i++)
        fprintf(fout5,"%d\t",lookup_tuple[i_tuple].tuples[i]);
      fprintf(fout5, PRINT_PRC_D,next,'\t');
      fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
      c++;
    }
  }
  fclose(fout5);
  return c;
}

// calculate standard deviation, min and max delta
double calculate_std_bin(double average, int d, unsigned long n_tuples) 
{
  double next;
  unsigned long c = 0;
  double sum = 0.0;
  double result;
  double min = DBL_MAX;
  double max = -DBL_MAX;
  // TODO: hard code types for different values of d
  for(size_t i_tuple=0; i_tuple< n_tuples; ++i_tuple)
  {
    next = lookup_tuple[i_tuple].avg;
    if (next < min)
      min = next;
    if (next > max)
      max = next;
    sum += pow(next-average, 2);
    c++;
  }
  fprintf(stderr, "Min Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,min, '\n');
  fprintf(stderr, "Max Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,max, '\n');
  result = sum/c;
  return sqrtl(result);
}

FILE * create_file_if_not_exists(const char * const filename)
{
  FILE * file = fopen(filename,"wx");
  if (file != NULL) return file;

  fprintf(stderr, "Could not create file %s\n", filename);
  fprintf(stderr, "Please verify it does not already exit.\n");
  exit(-1);
}

void write_tuple_to_file(FILE * const file,
                         const struct tuple * const tuples,
                         const int d,
                         const size_t tuple_idx)
{
  const int32_t * const value = tuples[tuple_idx].tuples;

  // There are minimum 2 values per tuple 
  fprintf(file, "%d\t%d\t", *value, *(value+1));

  // any others
  for (size_t j = 2; j < d; ++j)
    fprintf(file, "%d\t", *(value + j));

  fprintf(file, "%lf\n", tuples[tuple_idx].avg);
}

void write_n_tuples_hi(const struct tuple * const tuples,
                       const int n,
                       const int d)
{

  FILE * file = create_file_if_not_exists(output_files.out1_name);
  for (size_t i = 0; i < n; ++i)
    write_tuple_to_file(file, tuples, d, i);

  fclose(file);
}

void write_n_tuples_lo(const struct tuple * const tuples,
                       const int n,
                       const int d,
                       const uint64_t n_tuples)
{

  FILE * file = create_file_if_not_exists(output_files.out2_name);
  for (size_t i = n_tuples-1; i > n_tuples - 1 - n; --i)
    write_tuple_to_file(file, tuples, d, i);

  fclose(file);
}

void output_out3(const int nvar, const int k, const int d)
{
  FILE * out3 = fopen(output_files.out3_name, "wx");
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
int run(option_t *opt) 
{
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
    fprintf(stderr, "Error closing binary file 1.\n");
    return -1;
  }
  sign = pow(-1,d);
  fprintf(stderr, "Dimension:\t\t%d\n",d);
  fprintf(stderr, "Number of Variables:\t%d\n",n_vars);
  fprintf(stderr, "Number of Tuples:\t%ld\n",n_tuples);
  fprintf(stderr, "Average:\t\t%.10f\n",average);

  if(opt->n !=0 || opt->k != 0)
  {
    init_lookup_tuple(n_tuples);
    init_lookup_var(n_vars);

    fill_tuples(fbin2, d,
                n_tuples, n_vars);
    sort_tuples_inplace(lookup_tuple, n_tuples);
    fill_vars(n_tuples, d, opt->k, n_vars);
    rewind(fbin2);
    printf("%.10f %.10f\n", lookup_tuple[0].avg, lookup_tuple[n_tuples-1].avg);

    if (opt->n != 0) {
      write_n_tuples_hi(lookup_tuple, opt->n, d);
      write_n_tuples_lo(lookup_tuple, opt->n, d, n_tuples);
    }

    if(opt->k != 0)
      output_out3(n_vars, opt->k, d);

    if(opt->b != 0)
      output_out4(lookup_tuple, n_tuples, opt->b, lookup_tuple[0].avg, lookup_tuple[n_tuples-1].avg);
  }

  // if -s is passed, pass through the file once to calculate std, and again to create out5.txt
  if (opt->s_option) {
    std = calculate_std_bin(average, d, n_tuples+1);
    rewind(fbin2);
    cutoff = average + sign*std*opt->s_std;
    fprintf(stderr, "St. Dev.:\t\t%.10f\n",std);
    fprintf(stderr, "Cutoff:\t\t\t%.10f\n",cutoff);

    // count and print tuples above cutoff
    c = count_tuples_bin_cutoff(opt, d, cutoff, n_tuples+1, average, std);
    fprintf(stderr, "Tuples Above Cutoff:\t%ld\t(%.2f%%)\n", c, 100.0*c/n_tuples);
  }

  if (fclose(fbin2) != 0) {
    fprintf(stderr, "Error closing binary file 2.\n");
    return -1;
  }
  return 0;
}

