#include "test_common.h"

#include <cnf/cnf.h>
#include <solver/solver.h>

void
test_Cnf_simplify_withEmptyAssignment()
{
  AssignmentStack emptyAssignment;
  AssignmentStack_create(&emptyAssignment);

  const int32_t clause1[] = { 1, -2, 3 };
  const int32_t clause2[] = { 5, 3, -1 };
  const int32_t clause3[] = { 4, 2, -3 };
  const size_t clause1Count = 3u;
  const size_t clause2Count = 3u;
  const size_t clause3Count = 3u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Count));

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  TEST_ASSERT_SUCCESS(Cnf_simplify(&cnf, &emptyAssignment, &resultCnf));
  TEST_ASSERT_EQ(cnf.count, resultCnf.count);

  for (size_t i = 0u; i < cnf.count; ++i)
    TEST_ASSERT_EQ(cnf.pData[i], resultCnf.pData[i]);

  AssignmentStack_destroy(&emptyAssignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withTrueClause()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, true));

  const int32_t clause[] = { 1, -2, 3 };
  const size_t clauseCount = 3u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clauseCount));

  Cnf resultCnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&resultCnf));
  TEST_ASSERT_SUCCESS(Cnf_simplify(&cnf, &assignment, &resultCnf));

  TEST_ASSERT_EQ(resultCnf.count, 0u);

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withFalseClause()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, false));

  const int32_t clause[] = { 1, -2, 3 };
  const size_t clause1Count = 3u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause, clause1Count));

  Cnf resultCnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&resultCnf));
  TEST_ASSERT_SUCCESS(Cnf_simplify(&cnf, &assignment, &resultCnf));

  TEST_ASSERT_EQ(resultCnf.count, cnf.count - 1u);

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withMixedClauses()
{
  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 1u, false));
  TEST_ASSERT_SUCCESS(AssignmentStack_push(&assignment, 5u, true));

  // this clause should have the 1 omitted
  const int32_t clause1[] = { 1, -2, 3 };
  const size_t clause1Count = 3u;

  // this clause should be absent from the result
  const int32_t clause2[] = { 3, -4, 5 };
  const size_t clause2Count = 3u;

  // this clause should be unchanged int the result
  const int32_t clause3[] = { 6, -7, 8 };
  const size_t clause3Count = 3u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Count));

  Cnf resultCnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&resultCnf));
  TEST_ASSERT_SUCCESS(Cnf_simplify(&cnf, &assignment, &resultCnf));

  TEST_ASSERT_EQ(resultCnf.count, 8u);
  TEST_ASSERT_EQ(resultCnf.pData[0u], 0);
  TEST_ASSERT_EQ(resultCnf.pData[1u], -2);
  TEST_ASSERT_EQ(resultCnf.pData[2u], 3);
  TEST_ASSERT_EQ(resultCnf.pData[3u], 0);
  TEST_ASSERT_EQ(resultCnf.pData[4u], 6);
  TEST_ASSERT_EQ(resultCnf.pData[5u], -7);
  TEST_ASSERT_EQ(resultCnf.pData[6u], 8);
  TEST_ASSERT_EQ(resultCnf.pData[7u], 0);

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

int
main()
{
  test_Cnf_simplify_withEmptyAssignment();
  test_Cnf_simplify_withTrueClause();
  test_Cnf_simplify_withFalseClause();
  test_Cnf_simplify_withMixedClauses();
}
