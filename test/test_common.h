#ifndef DPLLSOLVER_TEST_COMMON_H
#define DPLLSOLVER_TEST_COMMON_H

#include <assert.h>
#include <stdlib.h>

#ifndef NDEBUG // TODO replace with SANITIZING compiler flag if
              //      the performance drops to much in debug mode

#define TEST_ASSERT(c) assert(c)

#else

#define TEST_ASSERT(c)                                                                                                                                         \
  if (!(c)) {                                                                                                                                                  \
    exit(__LINE__);                                                                                                                                                  \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif
