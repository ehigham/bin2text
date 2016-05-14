// Paul Draghicescu
// pdraghicescu@pnri.org
//

#ifndef RUN_H 
#define RUN_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define TUP_BUF_SIZE 10000

typedef struct option {
    unsigned int n, k;
    double b, s_std;
    int s_option;
    char in_file1[50];
    char in_file2[50];
} option_t;

int get_header(FILE* fbin,
               unsigned int *d,
               unsigned int *n_vars,
               unsigned long *n_tuples,
               double *average);
int run(option_t *opt);
//double calculate_std_bin(FILE* fbin, double average, int d, unsigned long n_tuples);
double calculate_std_bin(double average,
                         int d,
                         unsigned long n_tuples,
                         bool sorted_tuple);

#endif

