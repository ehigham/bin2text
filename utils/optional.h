#ifndef UTILS_OPTIONAL_HH__
#define UTILS_OPTIONAL_HH__


#define make_optional(T)  \
struct utils_optional_##T \
{                         \
  bool exist;             \
  T * data;               \
};                       


#endif

