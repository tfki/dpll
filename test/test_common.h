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
    printf("assert failed in file %s on line %d", __FILE__, __LINE__);                                                                                         \
    exit(__LINE__);                                                                                                                                            \
  }                                                                                                                                                            \
  ((void)0)

#endif

#define TEST_ASSERT_SUCCESS(status) TEST_ASSERT(!(status))
#define TEST_ASSERT_FAILURE(status) TEST_ASSERT(status)

#define TEST_ASSERT_TRUE(c) TEST_ASSERT(c)
#define TEST_ASSERT_FALSE(c) TEST_ASSERT(!(c))

#define TEST_ASSERT_EQ(v0,v1) TEST_ASSERT((v0)==(v1))
#define TEST_ASSERT_NEQ(v0,v1) TEST_ASSERT((v0)!=(v1))

#define TEST_ASSERT_NULL(p) TEST_ASSERT((p)==NULL)
#define TEST_ASSERT_NON_NULL(p) TEST_ASSERT((p)!=NULL)

#endif
