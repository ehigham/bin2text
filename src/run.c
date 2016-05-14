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

#include <stdlib.h>
#include <stdio.h>

#include "tuple.h"
#include "var.h"

#include "resource/strings.h"


// read and store header information
int get_header(FILE* fbin,
               unsigned int *d,
               unsigned int *n_vars,
               unsigned long *n_tuples,
               double *average) {
  size_t err;
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

void check_binary_files(FILE* fbin1,
                        FILE* fbin2,
                        const option_t *opt,
                        unsigned int *d,
                        unsigned int *n_vars,
                        unsigned long *n_tuples,
                        double *average)
{
  // open output file, get dimension and other information from header
  fbin1 = open_file_exit_if_error(opt->in_file1);
  fbin2 = open_file_exit_if_error(opt->in_file2);

  exit_if_empty_file(fbin1, "Error: empty binary file 1  \n");
  exit_if_empty_file(fbin2, "Error: empty binary file 2  \n");

  get_header(fbin1, d, n_vars, n_tuples, average);
  close_file_exit_if_error(fbin1, "Error closing binary file 1.\n");
}

// main function 
int run(option_t *opt) 
{
  FILE* fbin1;
  FILE* fbin2;
  double average = 0.0;
  double std = 0.0;
  double cutoff;
  unsigned int d, n_vars;
  double sign=1;
  unsigned long n_tuples, c;

  check_binary_files(fbin1, fbin2, opt, &d, &n_vars, &n_tuples, &average);
  const struct output_files output_files = open_and_check_output_files(opt);

  sign = pow(-1,d);
  fprintf(stderr, "Dimension:\t\t%d\n",d);
  fprintf(stderr, "Number of Variables:\t%d\n",n_vars);
  fprintf(stderr, "Number of Tuples:\t%ld\n",n_tuples);
  fprintf(stderr, "Average:\t\t%.10f\n",average);

  if(opt->n !=0 || opt->k != 0 || opt->s_option)
  {
    init_lookup_tuple(n_tuples, d);

    fill_tuples(fbin2, d,
                n_tuples);

    close_file_exit_if_error(fbin2, "Error closing binary file 1.\n");

    bool sorted_tuple = false;
    if(opt->k != 0 || opt->n != 0)
    {
      sort_tuples_inplace(lookup_tuple, n_tuples);
      sorted_tuple = true;
    }

    if (opt->n > 0) {
      write_n_tuples_hi(output_files.out1, lookup_tuple, opt->n, d, n_tuples);
      write_n_tuples_lo(output_files.out2,lookup_tuple, opt->n, d, n_tuples);
    }

    if(opt->k != 0)
    {
      init_lookup_var(n_vars, opt->k);
      fill_vars(n_tuples, d, opt->k, n_vars);
      write_var_avg_and_participation(output_files.out3,n_vars, d);
      delete_lookup_var();
    }

    if(opt->b != 0)
      write_scoring_histogram(output_files.out4,n_tuples, opt->b, lookup_tuple[0].avg, lookup_tuple[n_tuples-1].avg);

    // if -s is passed, pass through the file once to calculate std, and again to create out5.txt
    if (opt->s_option) 
    {
      std = calculate_std_bin(average, d, n_tuples+1, sorted_tuple);
      cutoff = average + sign*std*opt->s_std;
      fprintf(stderr, "St. Dev.:\t\t%.10f\n",std);
      fprintf(stderr, "Cutoff:\t\t\t%.10f\n",cutoff);

      // count and print tuples above cutoff
      c = count_tuples_bin_cutoff(output_files.out5,d, cutoff, n_tuples+1, average, std, sorted_tuple);
      fprintf(stderr, "Tuples Above Cutoff:\t%ld\t(%.2f%%)\n", c, 100.0*c/n_tuples);
    }

    delete_lookup_tuple();
  }
  else if (fclose(fbin2) != 0) {
    fprintf(stderr, "Error closing binary file 2.\n");
    return -1;
  }

  return 0;
}

