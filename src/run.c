// Paul Draghicescu
// pdraghicescu@pnri.org
//

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "run.h"
#include "string.h"

#ifdef DELTA_PREC
#define PRINT_PRC_D "%.10Lf%c"
#else
#define PRINT_PRC_D "%.10lf%c"
#endif

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

