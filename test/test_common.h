#ifndef DPLLSOLVER_TEST_COMMON_H
#define DPLLSOLVER_TEST_COMMON_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <common/log.h>

#define TEST_ASSERT(c)                                                                                                                                         \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed: " #c);                                                                                                                             \
    exit(1);                                                                                                                                                   \
  }                                                                                                                                                            \
  ((void)0)

#define TEST_ASSERT_SUCCESS(status) TEST_ASSERT(!(status))
#define TEST_ASSERT_FAILURE(status) TEST_ASSERT(status)

#define TEST_ASSERT_TRUE(c) TEST_ASSERT(c)
#define TEST_ASSERT_FALSE(c) TEST_ASSERT(!(c))

#define TEST_ASSERT_EQ(v0, v1) TEST_ASSERT((v0) == (v1))

#define TEST_ASSERT_NULL(p) TEST_ASSERT((p) == NULL)
#define TEST_ASSERT_NON_NULL(p) TEST_ASSERT((p) != NULL)

#endif
