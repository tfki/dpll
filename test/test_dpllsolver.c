#include <solver/solver.h>

#include <common/test.h>

static void
test_dpllSolve_emptyCnf()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  AssignmentStack assignment;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&assignment));

  TEST_ASSERT_SUCCESS(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

static void
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

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

static void
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

  TEST_ASSERT_FAILURE(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  AssignmentStack_destroy(&assignment);
  Cnf_destroy(&cnf);
}

static void
test_dpllSolver_unitPropagation_no_unitclause()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 3, 4 };
  size_t clause1Size = 2u;
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));

  AssignmentStack result;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&result));

  TEST_ASSERT_SUCCESS(dpllUnitPropagation(&cnf, &result));
  TEST_ASSERT_EQ(result.count, 0u);

  TEST_ASSERT_EQ(cnf.count, 4u);

  AssignmentStack_destroy(&result);
  Cnf_destroy(&cnf);
}

static void
test_dpllSolver_unitPropagation_with_mixed_clauses()
{
  Cnf cnf;
  TEST_ASSERT_SUCCESS(Cnf_create(&cnf));

  int32_t clause1[] = { 1 };
  size_t clause1Size = 1u;
  int32_t clause2[] = { -2 };
  size_t clause2Size = 1u;
  int32_t clause3[] = { 3, 4 };
  size_t clause3Size = 2u;
  int32_t clause4[] = { 2, 5 };
  size_t clause4Size = 2u;
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause1, clause1Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause2, clause2Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause3, clause3Size));
  TEST_ASSERT_SUCCESS(Cnf_pushClause(&cnf, clause4, clause4Size));

  AssignmentStack result;
  TEST_ASSERT_SUCCESS(AssignmentStack_create(&result));

  TEST_ASSERT_SUCCESS(dpllUnitPropagation(&cnf, &result));
  TEST_ASSERT_EQ(result.count, 3u);

  bool value;
  TEST_ASSERT_SUCCESS(AssignmentStack_get(&result, 1u, &value));
  TEST_ASSERT_TRUE(value);

  TEST_ASSERT_SUCCESS(AssignmentStack_get(&result, 2u, &value));
  TEST_ASSERT_FALSE(value);

  TEST_ASSERT_SUCCESS(AssignmentStack_get(&result, 5u, &value));
  TEST_ASSERT_TRUE(value);

  TEST_ASSERT_FAILURE(AssignmentStack_get(&result, 3u, &value));
  TEST_ASSERT_FAILURE(AssignmentStack_get(&result, 4u, &value));

  TEST_ASSERT_EQ(cnf.count, 4u);

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&result);
}

int
main()
{
  LOG_TEST_INIT();
  test_dpllSolve_emptyCnf();
  test_dpllSolve_precalculatedCnf();
  test_dpllSolve_unsatisfiableCnf();
  test_dpllSolver_unitPropagation_no_unitclause();
  test_dpllSolver_unitPropagation_with_mixed_clauses();
}
