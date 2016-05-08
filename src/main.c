
#include "utils/dispatcher.h"
#include "utils/optional.h"
#include "utils/traits.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

struct bar
{
  uint8_t dummy;
};
struct bar2
{
  uint8_t dummy;
};

#define get_dummy(X) _Generic((X), \
  struct bar:  get_dummy1,         \
  struct bar2: get_dummy2          \
)(X) 

void get_dummy1(struct bar a) {printf("dummy1\n");} 
void get_dummy2(struct bar2 b) {printf("dummy2\n");} 

bool trait_Comparable_struct_bar_cmp() { return true;}

int main(int argc, const char *argv[])
{
  struct bar myBar;
  struct bar2 myBar2;

  static_dispatch(myBar, get_dummy);
  static_dispatch(myBar2, get_dummy);

  traits_struct_dispatch(Comparable, bar,cmp);

  
  return 0;
}
