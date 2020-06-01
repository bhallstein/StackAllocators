#include "_test.h"

#define __PSA_PSIZE_TYPE uint8_t
  // The size of our objects does not exceed 256 bytes
#include "../StackAllocator.h"
#include "../StackAllocator_Expand.h"
#include "../StackAllocator_Pop.h"
#include "../StackAllocator_PopAndExpand.h"


void test_StackAllocator() {
  StackAllocator sa(35);
  p_file_header("StackAllocator.h");

  uint8_t *x = (uint8_t*) sa.allocate<double>();    // 8 => 8
  uint8_t *y = (uint8_t*) sa.allocate<long long>(); // 8 => 16
  uint8_t *z = (uint8_t*) sa.allocate<uint16_t>();  // 2 => 18
  uint8_t *a = (uint8_t*) sa.allocate<long long>(); // 8 => 26
  uint8_t *c = (uint8_t*) sa.allocate<uint16_t>();  // 2 => 28
  uint8_t *b = (uint8_t*) sa.allocate<long long>(); // 8 - should return NULL

  printf("- Testing basic allocation:\n");
  p_assert(y - x == sizeof(double));
  p_assert(z - y == sizeof(long long));
  p_assert(a - z == sizeof(uint16_t));

  printf("- Testing allocation fails when full:\n");
  p_assert(b == NULL);

  printf("\n");
}


void test_StackAllocator_Expand() {
  StackAllocator_Expand sa(35);
  p_file_header("StackAllocator_Expand.h");

  uint8_t *x = (uint8_t*) sa.allocate<double>();    // 8
  uint8_t *y = (uint8_t*) sa.allocate<long long>();      // 8
  uint8_t *z = (uint8_t*) sa.allocate<uint16_t>();  // 2
  uint8_t *a = (uint8_t*) sa.allocate<long long>();      // 8 => 26
  uint8_t *c = (uint8_t*) sa.allocate<uint16_t>();  // 2 => 28
  uint8_t *b = (uint8_t*) sa.allocate<long long>();      // 8 - start new allocator

  printf("- Testing basic allocation:\n");
  p_assert(y - x == sizeof(double));
  p_assert(z - y == sizeof(long long));
  p_assert(a - z == sizeof(uint16_t));
  p_assert(c - a == sizeof(long long));
  p_assert(b - c != sizeof(uint16_t));
  printf("      (b-c is %ld)\n", b-a);

  printf("\n");
}


void test_StackAllocator_Pop() {
  StackAllocator_Pop sa(36);
  p_file_header("StackAllocator_Pop.h");

  int psize = sizeof(__PSA_PSIZE_TYPE);

  uint8_t *x = (uint8_t*) sa.allocate<double>();    // 9
  uint8_t *y = (uint8_t*) sa.allocate<long long>();    // 9
  uint8_t *z = (uint8_t*) sa.allocate<uint16_t>();  // 3
  uint8_t *a = (uint8_t*) sa.allocate<int>();      // 5 => 26
  uint8_t *c = (uint8_t*) sa.allocate<uint16_t>();  // 3 => 29
  uint8_t *b = (uint8_t*) sa.allocate<long long>();    // 9 - should return NULL

  printf("- Testing basic allocation:\n");
  p_assert(y - x == sizeof(double) + psize);
  p_assert(z - y == sizeof(long long) + psize);
  p_assert(a - z == sizeof(uint16_t) + psize);

  printf("- Testing allocation fails when full:\n");
  p_assert(b == NULL);

  printf("- Testing reuse:\n");
  sa.pop();  // Pop A off the stack
  sa.pop();  // Pop C off the stack
  uint8_t *a2 = (uint8_t*) sa.allocate<int>();
  p_assert(a2 == a);

  printf("- Testing popping to (and beyond) empty:\n");
  for (int i=0; i < 100; ++i) sa.pop();  // pop a silly number of times
  uint8_t *x2 = (uint8_t*) sa.allocate<char>();    // x2 should == x
  uint8_t *y2 = (uint8_t*) sa.allocate<int>();    // diff. should be 5
  p_assert(x2 == x);
  p_assert(y2 - x2 == sizeof(char) + psize);

  printf("\n");
}


void test_StackAllocator_PopAndExpand() {
  StackAllocator_PopAndExpand sa(36);
  p_file_header("StackAllocator_PopAndExpand.h");

  int psize = sizeof(__PSA_PSIZE_TYPE);

  uint8_t *x = (uint8_t*) sa.allocate<double>();    // 9
  uint8_t *y = (uint8_t*) sa.allocate<long long>();    // 9
  uint8_t *z = (uint8_t*) sa.allocate<uint16_t>();  // 3
  uint8_t *a = (uint8_t*) sa.allocate<int>();      // 5 => 26
  uint8_t *c = (uint8_t*) sa.allocate<uint16_t>();  // 3 => 29
  uint8_t *b = (uint8_t*) sa.allocate<long long>();    // 9 - start new allocator

  printf("- Testing basic allocation:\n");
  p_assert(y - x == sizeof(double) + psize);
  p_assert(z - y == sizeof(long long) + psize);
  p_assert(a - z == sizeof(uint16_t) + psize);
  p_assert(c - a == sizeof(int) + psize);
  p_assert(b - c != sizeof(uint16_t) + psize);
  printf("      (b-c is %ld)\n", b-a);

  printf("- Testing destruction of empty PSAs:\n");
  sa.pop();  // Pop A, B, C off the stack
  sa.pop();
  sa.pop();
  uint8_t *a2 = (uint8_t*) sa.allocate<int>();
  p_assert(a2 == a);

  printf("- Testing non-destruction of *first* empty PSA:\n");
  for (int i=0; i < 100; ++i) sa.pop();  // pop a silly number of times

  uint8_t *x2 = (uint8_t*) sa.allocate<char>();    // x2 should == x
  uint8_t *y2 = (uint8_t*) sa.allocate<int>();    // diff. should be 5
  p_assert(x2 == x);
  p_assert(y2 - x2 == sizeof(char) + psize);

  printf("\n");
}


int main() {
  test_StackAllocator();
  test_StackAllocator_Expand();
  test_StackAllocator_Pop();
  test_StackAllocator_PopAndExpand();

  return 0;
}
