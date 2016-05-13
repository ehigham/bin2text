#ifndef TUPLE_H__
#define TUPLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>

//__attribute__((packed)) //TODO: to be considered
struct tuple
{
  int32_t tuples[8];
  double avg;
};

typedef int32_t var_t;

extern struct tuple *lookup_tuple;

void init_lookup_tuple(const unsigned long ntuples);

void fill_tuples(FILE * fbin,
                 const int d,
                 const unsigned long n_tuples,
                 const int nvar);

void fprintf_tuple(FILE * out,
                   struct tuple* tuple,
                   const int d);

void sort_tuples_inplace(struct tuple * arr,
                         const unsigned long n_tuples);

#endif

