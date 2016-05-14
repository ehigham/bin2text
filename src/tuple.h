#ifndef TUPLE_H__
#define TUPLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>

typedef int32_t var_t;

//__attribute__((packed)) //TODO: to be considered
struct tuple
{
  __attribute__((aligned(16)))
  var_t tuples[8];
  double avg;
};

extern struct tuple *lookup_tuple;

void init_lookup_tuple(const unsigned long ntuples);

void fill_tuples(FILE * fbin,
                 const int d,
                 const unsigned long n_tuples);

void fprintf_tuple(FILE * out,
                   struct tuple* tuple,
                   const int d);

void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const int d);


void sort_tuples_inplace(struct tuple * arr,
                         const unsigned long n_tuples);

void delete_lookup_tuple();

#endif

