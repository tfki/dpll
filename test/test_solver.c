#include "test_common.h"

#include <solver/solver.h>

void
test_dpllSolve_emptyCnf()
{
  Cnf cnf;
  AssignmentStack assignment;

  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_SUCCESS(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);
}

void
test_dpllSolve_precalculatedCnf()
{
  // { C , D }, { A }, { -B }, { C }, { D }, { B , -A, D }
  // { 3 , 4 }, { 1 }, { -2 }, { 3 }, { 4 }, { 2 , -1, 4 }

  const uint32_t variables[] = { 1u, 2u, 3u, 4u };
  const bool result[] = { true, false, true, true };
  const size_t variablesCount = 4u;

  const int32_t clause1[] = { 3, 4 };
  const int32_t clause2[] = { 1 };
  const int32_t clause3[] = { -2 };
  const int32_t clause4[] = { 3 };
  const int32_t clause5[] = { 4 };
  const int32_t clause6[] = { 2, -1, 4 };

  const size_t clause1Count = 2u;
  const size_t clause2Count = 1u;
  const size_t clause3Count = 1u;
  const size_t clause4Count = 1u;
  const size_t clause5Count = 1u;
  const size_t clause6Count = 3u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause4, clause4Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause5, clause5Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause6, clause6Count));

  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT_SUCCESS(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  for (size_t i = 0u; i < variablesCount; ++i) {
    bool value;
    TEST_ASSERT_SUCCESS(AssignmentStack_get(&assignment, variables[i], &value));
    TEST_ASSERT_EQ(value, result[i]);
  }

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);
}

void
test_dpllSolve_unsatisfiableCnf()
{
  const int32_t clause1[] = { 1 };
  const int32_t clause2[] = { -1 };

  const size_t clause1Count = 1u;
  const size_t clause2Count = 1u;

  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Count));

  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));
  TEST_ASSERT(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);
}

void
test_dpllSolver_unitPropagation_one_unit_clause()
{
//  Cnf cnf;
//  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));
//
//  ClauseBuffer clauseBuffer;
//  TEST_ASSERT_SUCCESS(ClauseBuffer_create(&clauseBuffer));
//  TEST_ASSERT_SUCCESS(ClauseBuffer_push(&clauseBuffer, -1));
//
//  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clauseBuffer.pData, clauseBuffer.count));
//
//  AssignmentStack result;
//  AssignmentStack_create(&result);
//
//  TEST_ASSERT_SUCCESS(dpllUnitPropagation(&cnf, &result));
//  TEST_ASSERT(result.count == 1);
//  TEST_ASSERT(result.pKeys[0] == 1);
//  TEST_ASSERT(result.pValues[0] == false);
//
//  TEST_ASSERT(cnf.count == 0);
//
//  AssignmentStack_destroy(&result);
//  ClauseBuffer_destroy(&clauseBuffer);
//  Cnf_destroy(&cnf);
}

int
main()
{
  test_dpllSolve_emptyCnf();
  test_dpllSolve_precalculatedCnf();
  test_dpllSolve_unsatisfiableCnf();
  test_dpllSolver_unitPropagation_one_unit_clause();
}
