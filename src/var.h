#ifndef VAR_H__
#define VAR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>

#include "tuple.h"

struct var
{
  size_t count;
  double avg;
  size_t *tuple_indexes_max;
  size_t size_tuple_max;
  size_t *tuple_indexes_min;
  size_t size_tuple_min;
};

typedef int32_t var_t;

extern struct var *lookup_var;
/*
 * Initialize lookup variable table containing tuples' variable information
 */
void init_lookup_var(const size_t nvar,
                     const size_t k);

/*
 * Fill information of variables according to the tuples where they are defined
 */
void fill_vars(const unsigned long n_tuples,
               const size_t d,
               const size_t k,
               const size_t nvar);
/*
 * Free spaces
 */
void delete_lookup_var();

/*
 * Output a list of variable containing for each variable its own
 * average as well as the list of tuples where the variable is
 */
void write_var_avg_and_participation(FILE* out3, const size_t nvar, const size_t d);

#endif
