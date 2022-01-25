#include "test_common.h"

#include <solver/solver.h>

void
test_dpllSolve_emptyCnf()
{
  Cnf cnf;
  Cnf_create(&cnf);

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  TEST_ASSERT(!dpllSolve(&cnf, dpllTrivialPick, &assignment));

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

void
test_dpllSolve_precalculatedCnf()
{
  // { C , D }, { A }, { -B }, { C }, { D }, { B , -A, D }
  // { 3 , 4 }, { 1 }, { -2 }, { 3 }, { 4 }, { 2 , -1, 4 }

  const uint32_t variables[] = { 1u, 2u, 3u, 4u };
  const bool result[] = { 1, 0, 1, 1 };
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
  Cnf_create(&cnf);
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause3, clause3Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause4, clause4Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause5, clause5Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause6, clause6Count));

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  TEST_ASSERT(!dpllSolve(&cnf, dpllTrivialPick, &assignment));

  for (size_t i = 0u; i < variablesCount; ++i) {
    bool value;
    TEST_ASSERT(!AssignmentStack_get(&assignment, variables[i], &value));
    TEST_ASSERT(value == result[i]);
  }

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

void
test_dpllSolve_unsatisfiableCnf()
{
  const int32_t clause1[] = { 1 };
  const int32_t clause2[] = { -1 };

  const size_t clause1Count = 1u;
  const size_t clause2Count = 1u;

  Cnf cnf;
  Cnf_create(&cnf);
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Count));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Count));

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  TEST_ASSERT(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

void
test_dpllSolver_unitPropagation_no_unitclause()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  int32_t clause1[] = {3, 4};
  size_t clause1Size = 2;
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Size));

  AssignmentStack result;
  AssignmentStack_create(&result);

  TEST_ASSERT(!dpllUnitPropagation(&cnf, &result));
  TEST_ASSERT(result.count == 0u);

  TEST_ASSERT(cnf.count == 4u);

  AssignmentStack_destroy(&result);
  Cnf_destroy(&cnf);
}

void
test_dpllSolver_unitPropagation_with_mixed_clauses()
{
  Cnf cnf;
  TEST_ASSERT(!Cnf_create(&cnf));

  int32_t clause1[] = {1};
  size_t clause1Size = 1;
  int32_t clause2[] = {-2};
  size_t clause2Size = 1;
  int32_t clause3[] = {3, 4};
  size_t clause3Size = 2;
  int32_t clause4[] = {2, 5};
  size_t clause4Size = 2;
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause2, clause2Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause3, clause3Size));
  TEST_ASSERT(!Cnf_pushClause(&cnf, clause4, clause4Size));

  AssignmentStack result;
  AssignmentStack_create(&result);

  TEST_ASSERT(!dpllUnitPropagation(&cnf, &result));
  TEST_ASSERT(result.count == 3);

  bool value;
  TEST_ASSERT(!AssignmentStack_get(&result, 1u, &value));
  TEST_ASSERT(value);

  TEST_ASSERT(!AssignmentStack_get(&result, 2u, &value));
  TEST_ASSERT(!value);

  TEST_ASSERT(!AssignmentStack_get(&result, 5u, &value));
  TEST_ASSERT(value);

  TEST_ASSERT(AssignmentStack_get(&result, 3u, &value));
  TEST_ASSERT(AssignmentStack_get(&result, 4u, &value));

  TEST_ASSERT(cnf.count == 4);

  AssignmentStack_destroy(&result);
  Cnf_destroy(&cnf);
}

int
main()
{
  test_dpllSolve_emptyCnf();
  test_dpllSolve_precalculatedCnf();
  test_dpllSolve_unsatisfiableCnf();
  test_dpllSolver_unitPropagation_no_unitclause();
  test_dpllSolver_unitPropagation_with_mixed_clauses();
}
