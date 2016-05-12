#ifndef TUPLE_HH__
#define TUPLE_HH__

struct tuple
{
  int32_t tuples[8];
  double avg;
};

struct var
{
  size_t count;
  double avg;
  size_t *tuple_indexes_max;
  size_t size_tuple_max;
  size_t *tuple_indexes_min;
  size_t size_tuple_min;
};

#endif

