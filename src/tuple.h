#ifndef TUPLE_H__
#define TUPLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

typedef int32_t var_t;

//__attribute__((packed)) //TODO: to be considered
struct tuple
{
  __attribute__((aligned(16)))
  var_t *values;
  double avg;
};

extern struct tuple *lookup_tuple;

/*
 * Initialize the global lookup table cotnaining tuples' information
 */
void init_lookup_tuple(const unsigned long ntuples,
                       const size_t d);

/*
 * Read a binary file and fill the a lookup table of tuples
 */
void fill_tuples(FILE * const __restrict fbin,
                 const size_t d,
                 const unsigned long n_tuples);

/*
 * Write a tuple entry to file (tuple values + score average)
 */
void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const size_t d);
/*
 * Sort tuples in place
 */
void sort_tuples_inplace(struct tuple * arr,
                         const unsigned long n_tuples);
/*
 * Free memory
 */
void delete_lookup_tuple();



unsigned long count_tuples_bin_cutoff(FILE * const __restrict out5,
                                      size_t d,
                                      double cutoff,
                                      long unsigned n_tuples,
                                      double average,
                                      double std,
                                      bool lookup_tuple_sorted);

// calculate standard deviation, min and max delta
double calculate_std_bin(double average,
                         int d,
                         const unsigned long n_tuples,
                         bool lookup_tuple_sorted) ;

void write_n_tuples_hi(FILE * const __restrict out1,
                       const struct tuple * const __restrict tuples,
                       int n,
                       const int d,
                       const uint64_t n_tuples);


void write_n_tuples_lo(FILE * const __restrict out2,
                       const struct tuple * const __restrict tuples,
                       int n,
                       const int d,
                       const uint64_t n_tuples);

void write_scoring_histogram(FILE * const __restrict out4,
                             const unsigned long ntuples,
                             const double b, 
                             const double min,
                             const double max);


#endif

