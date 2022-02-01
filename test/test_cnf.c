#include <cnf/cnf.h>

#include <common/test.h>

#include <string.h>

static void
test_Cnf_create()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_EQ(cnf.count, 0u);
  TEST_ASSERT_EQ(cnf.capacity, 1024u);
  TEST_ASSERT_NON_NULL(cnf.pData);
  Cnf_destroy(&cnf);
}

static void
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
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause0, clause0Count));
  TEST_ASSERT_EQ(cnf.count, 6u);

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT_EQ(cnf.count, 9u);

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT_EQ(cnf.count, 13u);

  for (size_t i = 0u; i < rawCnfCount; ++i)
    TEST_ASSERT_EQ(cnf.pData[i], rawCnf[i]);

  Cnf_destroy(&cnf);
}

static void
test_Cnf_capacityOverflow()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  // push first clause separately, as it will also insert leading zero
  // so count will be +=3
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clauseCount));

  const size_t testCnfCapacity = cnf.capacity;
  for (size_t i = cnf.count; i + 2u < testCnfCapacity; i += 2u)
    TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clauseCount));

  TEST_ASSERT_EQ(cnf.capacity, 1024u);
  TEST_ASSERT_EQ(cnf.count, 1023u);

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clauseCount));
  TEST_ASSERT_EQ(cnf.capacity, 2048u);
  TEST_ASSERT_EQ(cnf.count, 1025u);

  Cnf_destroy(&cnf);
}

static void
test_Cnf_copy()
{
  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const size_t clause0Count = 4u;
  const size_t clause1Count = 2u;

  Cnf testCnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&testCnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&testCnf, clause0, clause0Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&testCnf, clause1, clause1Count));

  Cnf copyCnf;
  TEST_ASSERT_SUCCESS(Cnf_copy(&copyCnf, &testCnf));

  for (size_t i = 0u; i < testCnf.count; ++i)
    TEST_ASSERT_EQ(testCnf.pData[i], copyCnf.pData[i]);

  Cnf_destroy(&testCnf);
  Cnf_destroy(&copyCnf);
}

static void
test_Cnf_destroy()
{
  const int32_t clause[] = { 1 };
  const size_t clauseCount = 1u;

  Cnf testCnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&testCnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&testCnf, clause, clauseCount));

  Cnf_destroy(&testCnf);
  TEST_ASSERT_NULL(testCnf.pData);
  TEST_ASSERT_EQ(testCnf.capacity, 0u);
  TEST_ASSERT_EQ(testCnf.count, 0u);
}

static void
test_Cnf_Swap()
{
  const int32_t cnf1Clause1[] = { 1, 2 };
  const size_t cnf1Clause1Count = 2u;
  const int32_t cnf1Clause2[] = { 3, -4, 5 };
  const size_t cnf1Clause2Count = 3u;

  Cnf cnf1;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf1));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf1, cnf1Clause1, cnf1Clause1Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf1, cnf1Clause2, cnf1Clause2Count));

  TEST_ASSERT_EQ(cnf1.count, 8u);

  const int32_t cnf2Clause1[] = { 7, 8 };
  const size_t cnf2Clause1Count = 2u;

  Cnf cnf2;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf2));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf2, cnf2Clause1, cnf2Clause1Count));

  TEST_ASSERT_EQ(cnf2.count, 4u);

  Cnf_swap(&cnf1, &cnf2);

  TEST_ASSERT_EQ(cnf1.count, 4u);
  TEST_ASSERT_EQ(cnf1.pData[0], 0);
  TEST_ASSERT_EQ(cnf1.pData[1], 7);
  TEST_ASSERT_EQ(cnf1.pData[2], 8);
  TEST_ASSERT_EQ(cnf1.pData[3], 0);

  TEST_ASSERT_EQ(cnf2.count, 8u);
  TEST_ASSERT_EQ(cnf2.pData[0], 0);
  TEST_ASSERT_EQ(cnf2.pData[1], 1);
  TEST_ASSERT_EQ(cnf2.pData[2], 2);
  TEST_ASSERT_EQ(cnf2.pData[3], 0);
  TEST_ASSERT_EQ(cnf2.pData[4], 3);
  TEST_ASSERT_EQ(cnf2.pData[5], -4);
  TEST_ASSERT_EQ(cnf2.pData[6], 5);
  TEST_ASSERT_EQ(cnf2.pData[7], 0);

  Cnf_destroy(&cnf1);
  Cnf_destroy(&cnf2);
}

static void
test_Cnf_toStr_emptyClause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, NULL, 0u));

  char* cnfStr;
  TEST_ASSERT_SUCCESS(Cnf_toStr(&cnf, &cnfStr));

  char* expectedCnfStr = "()";

  TEST_ASSERT_EQ(strcmp(cnfStr, expectedCnfStr), 0);

  Cnf_destroy(&cnf);
}

static void
test_Cnf_toStr_singleClause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause[] = { 1, -2, 4, -3 };
  size_t clauseSize = 4;

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clauseSize));

  char* cnfStr;
  TEST_ASSERT_SUCCESS(Cnf_toStr(&cnf, &cnfStr));

  char* expectedCnfStr = "(1 OR -2 OR 4 OR -3)";

  TEST_ASSERT_EQ(strcmp(cnfStr, expectedCnfStr), 0);

  Cnf_destroy(&cnf);
}

static void
test_Cnf_toStr_emptyCnf()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  char* cnfStr;
  TEST_ASSERT_SUCCESS(Cnf_toStr(&cnf, &cnfStr));

  char* expectedCnfStr = "";

  TEST_ASSERT_EQ(strcmp(cnfStr, expectedCnfStr), 0);

  Cnf_destroy(&cnf);
}

static void
test_Cnf_toStr_mixedClauses()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 1, -2, 4, -3 };
  size_t clause1Size = 4;
  int32_t* clause2 = NULL;
  size_t clause2Size = 0;
  int32_t clause3[] = { 2, -5, 3, 1 };
  size_t clause3Size = 4;

  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Size));

  char* cnfStr;
  TEST_ASSERT_SUCCESS(Cnf_toStr(&cnf, &cnfStr));

  char* expectedCnfStr = "(1 OR -2 OR 4 OR -3) AND () AND (2 OR -5 OR 3 OR 1)";

  TEST_ASSERT_EQ(strcmp(cnfStr, expectedCnfStr), 0);

  Cnf_destroy(&cnf);
}

int
main()
{
  LOG_TEST_INIT();
  test_Cnf_create();
  test_Cnf_pushClause();
  test_Cnf_capacityOverflow();
  test_Cnf_copy();
  test_Cnf_destroy();
  test_Cnf_Swap();
  test_Cnf_toStr_emptyClause();
  test_Cnf_toStr_singleClause();
  test_Cnf_toStr_emptyCnf();
  test_Cnf_toStr_mixedClauses();
}
