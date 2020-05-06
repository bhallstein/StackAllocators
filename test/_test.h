#include <cstdio>
#include <cassert>

#define p_assert(x) do {      \
    printf("%60s", #x);       \
    assert(x);                \
    printf(" - PASS :)\n");   \
  } while (false)

#define p_header(s) do {      \
    printf("\n");             \
    printf("- %s\n", s);      \
  } while (false)

