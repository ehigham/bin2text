#ifndef UTILS_DISPATCHER_HH__
#define UTILS_DISPATCHER_HH__

#define static_dispatch(str, fun, ...) fun(str, ##__VA_ARGS__)

#endif
