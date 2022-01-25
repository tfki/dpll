#include "test_common.h"

#include <cnf/cnf.h>

static int
test_Cnf_create()
{
  Cnf cnf;

  TEST_ASSERT(!Cnf_create(&cnf));
  TEST_ASSERT(cnf.count == 0u);
  TEST_ASSERT(cnf.capacity == 1024u);
  TEST_ASSERT(cnf.pData != NULL);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_pushClause()
{
  const int32_t rawCnf[] = { 0, 10, -13, 11, -14, 0, -10, -11, 0, 16, -13, -11, 0 };
  const size_t rawCnfCount = 13u;

  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const int32_t clause2[] = { 16, -13, -11 };
  const size_t clause0Count = 4u;
  const size_t clause1Count = 2u;
  const size_t clause2Count = 3u;

  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause0, clause0Count));
  TEST_ASSERT(cnf.count == 6u);

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT(cnf.count == 9u);

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT(cnf.count == 13u);

  for (size_t i = 0u; i < rawCnfCount; ++i)
    TEST_ASSERT(cnf.pData[i] == rawCnf[i]);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_capacityOverflow()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  // push first clause separately, as it will also insert leading zero
  // so count will be +=3
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause, clauseCount));

  const size_t testCnfCapacity = cnf.capacity;
  for (size_t i = cnf.count; i + 2u < testCnfCapacity; i += 2u)
    TEST_ASSERT(!Cnf_pushClause(&cnf, clause, clauseCount));

  TEST_ASSERT(cnf.capacity == 1024u);
  TEST_ASSERT(cnf.count == 1023u);

  TEST_ASSERT(!Cnf_pushClause(&cnf, clause, clauseCount));
  TEST_ASSERT(cnf.capacity == 2048u);
  TEST_ASSERT(cnf.count == 1025u);

  Cnf_destroy(&cnf);
  return 0;
}

static int
test_Cnf_copy()
{
  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const size_t clause0Count = 4u;
  const size_t clause1Count = 2u;

  Cnf testCnf;
  TEST_ASSERT(!Cnf_create(&testCnf));
  TEST_ASSERT(!Cnf_pushClause(&testCnf, clause0, clause0Count));
  TEST_ASSERT(!Cnf_pushClause(&testCnf, clause1, clause1Count));

  Cnf copyCnf;
  TEST_ASSERT(!Cnf_copy(&copyCnf, &testCnf));

  for (size_t i = 0u; i < testCnf.count; ++i)
    TEST_ASSERT(testCnf.pData[i] == copyCnf.pData[i]);

  Cnf_destroy(&testCnf);
  Cnf_destroy(&copyCnf);
  return 0;
}

static int
test_Cnf_destroy()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf testCnf;
  TEST_ASSERT(!Cnf_create(&testCnf));
  TEST_ASSERT(!Cnf_pushClause(&testCnf, clause, clauseCount));

  Cnf_destroy(&testCnf);
  TEST_ASSERT(testCnf.pData == NULL);
  TEST_ASSERT(testCnf.capacity == 0u);
  TEST_ASSERT(testCnf.count == 0u);

  return 0;
}

static void
test_Cnf_Swap()
{
  const int32_t cnf1clause1[] = {1, 2};
  const size_t cnf1clause1Count = 2u;
  const int32_t cnf1clause2[] = {3, -4, 5};
  const size_t cnf1clause2Count = 3u;

  Cnf cnf1;
  TEST_ASSERT(!Cnf_create(&cnf1));
  TEST_ASSERT(!Cnf_pushClause(&cnf1, cnf1clause1, cnf1clause1Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf1, cnf1clause2, cnf1clause2Count));

  TEST_ASSERT(cnf1.count == 8u);

  const int32_t cnf2clause1[] = {7, 8};
  const size_t cnf2clause1Count = 2u;

  Cnf cnf2;
  TEST_ASSERT(!Cnf_create(&cnf2));
  TEST_ASSERT(!Cnf_pushClause(&cnf2, cnf2clause1, cnf2clause1Count));

  TEST_ASSERT(cnf2.count == 4u);

  Cnf_swap(&cnf1, &cnf2);

  TEST_ASSERT(cnf1.count == 4u);
  TEST_ASSERT(cnf1.pData[0] == 0);
  TEST_ASSERT(cnf1.pData[1] == 7);
  TEST_ASSERT(cnf1.pData[2] == 8);
  TEST_ASSERT(cnf1.pData[3] == 0);

  TEST_ASSERT(cnf2.count == 8u);
  TEST_ASSERT(cnf2.pData[0] == 0);
  TEST_ASSERT(cnf2.pData[1] == 1);
  TEST_ASSERT(cnf2.pData[2] == 2);
  TEST_ASSERT(cnf2.pData[3] == 0);
  TEST_ASSERT(cnf2.pData[4] == 3);
  TEST_ASSERT(cnf2.pData[5] == -4);
  TEST_ASSERT(cnf2.pData[6] == 5);
  TEST_ASSERT(cnf2.pData[7] == 0);

  Cnf_destroy(&cnf1);
  Cnf_destroy(&cnf2);
}

int
main()
{
  test_Cnf_create();
  test_Cnf_pushClause();
  test_Cnf_capacityOverflow();
  test_Cnf_copy();
  test_Cnf_destroy();
  test_Cnf_Swap();
}
