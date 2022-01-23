#include <assert.h>
#include <cnf/cnf.h>
#include <solver/solver.h>

void
test_Cnf_simplify_withEmptyAssignment()
{
  Assignment emptyAssignment;
  Assignment_create(&emptyAssignment);

  int32_t clause1[] = { 1, -2, 3 };
  int32_t clause2[] = { 5, 3, -1 };
  int32_t clause3[] = { 4, 2, -3 };
  size_t clause1Count = 3u;
  size_t clause2Count = 3u;
  size_t clause3Count = 3u;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause1, clause1Count);
  Cnf_pushClause(&cnf, clause2, clause2Count);
  Cnf_pushClause(&cnf, clause3, clause3Count);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &emptyAssignment, &resultCnf);

  assert(cnf.count == resultCnf.count);

  for (size_t i = 0u; i < cnf.count; ++i) {
    assert(cnf.pData[i] == resultCnf.pData[i]);
  }

  Assignment_destroy(&emptyAssignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withTrueClause()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1u, 1);

  int32_t clause[] = { 1, -2, 3 };
  size_t clauseCount = 3u;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause, clauseCount);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == 0u);

  Assignment_destroy(&assignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withFalseClause()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1u, 0);

  int32_t clause[] = { 1, -2, 3 };
  size_t clause1Count = 3u;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause, clause1Count);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == (cnf.count - 1u));

  Assignment_destroy(&assignment);
  Cnf_destroy(&cnf);
  Cnf_destroy(&resultCnf);
}

void
test_Cnf_simplify_withMixedClauses()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1u, 0);
  Assignment_set(&assignment, 5u, 1);

  // this clause should have the 1 omitted
  int32_t clause1[] = { 1, -2, 3 };
  size_t clause1Count = 3u;

  // this clause should be absent from the result
  int32_t clause2[] = { 3, -4, 5 };
  size_t clause2Count = 3u;

  // this clause should be unchanged int the result
  int32_t clause3[] = { 6, -7, 8 };
  size_t clause3Count = 3u;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause1, clause1Count);
  Cnf_pushClause(&cnf, clause2, clause2Count);
  Cnf_pushClause(&cnf, clause3, clause3Count);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == 8u);
  assert(resultCnf.pData[0u] == 0);
  assert(resultCnf.pData[1u] == -2);
  assert(resultCnf.pData[2u] == 3);
  assert(resultCnf.pData[3u] == 0);
  assert(resultCnf.pData[4u] == 6);
  assert(resultCnf.pData[5u] == -7);
  assert(resultCnf.pData[6u] == 8);
  assert(resultCnf.pData[7u] == 0);

  Assignment_destroy(&assignment);
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
