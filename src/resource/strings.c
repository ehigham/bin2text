/*    strings.c
 *
 *    defines common strings used by bin2text
 *
 *    This file is part of bin2text.
 *
 *    bin2text is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at youroption) any later version.
 *
 *    bin2text is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with bin2text.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    (C) 2016, Thomas Legris, Edmund Higham
**/

#include "strings.h"

const struct output_filenames output_filenames =
{
    .out1_name = "out1.txt",
    .out2_name = "out2.txt",
    .out3_name = "out3.txt",
    .out4_name = "out4.txt",
    .out5_name = "out5.txt"
};

struct output_files open_and_check_output_files(const option_t * opt)
{
  FILE *f1=NULL, *f2=NULL, *f3=NULL, *f4=NULL, *f5=NULL;
  if(opt->n > 0)
  {
    f1 = create_file_if_not_exists(output_filenames.out1_name);
    f2 = create_file_if_not_exists(output_filenames.out2_name);
  }
  if(opt->k > 0)
    f3 = create_file_if_not_exists(output_filenames.out3_name);

  if(opt->b != 0)
    f4 = create_file_if_not_exists(output_filenames.out4_name);

  if(opt->s_option)
    f5 = create_file_if_not_exists(output_filenames.out5_name);

  struct output_files res = {f1,f2,f3,f4,f5};
  return res;
}

FILE * create_file_if_not_exists(const char * const __restrict filename)
{
  FILE * file = fopen(filename,"wx");
  if (file != NULL) return file;

  fprintf(stderr, "Could not create file %s\n", filename);
  fprintf(stderr, "Please verify it does not already exit.\n");
  exit(-1);
}

FILE * open_file_exit_if_error(const char* file)
{
  FILE * res = fopen(file, "rb");
  if (!res) {
    fprintf(stderr, "Error: couldn't open header file \"%s\".\n",file);
    exit(-1);
  }
  return res;
}

void close_file_exit_if_error(FILE* file , const char* msg)
{
  if (fclose(file) != 0) {
    fprintf(stderr, msg);
    exit(-1);
  }
}

void exit_if_empty_file(FILE * file, const char * message)
{
  rewind(file);
  fseek(file, 0L, SEEK_END);
  if (!ftell(file)) {
    fprintf(stderr, message);
    exit(-1);
  }
  rewind(file);
}
