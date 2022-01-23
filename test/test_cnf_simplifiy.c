#include <cnf/cnf.h>
#include <solver/solver.h>
#include <assert.h>

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

  for (int i = 0; i < cnf.count; ++i) {
    assert(cnf.pData[i] == resultCnf.pData[i]);
  }
}

void
test_Cnf_simplify_withTrueClause()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1, 1);

  int32_t clause[] = { 1, -2, 3 };
  size_t clauseCount = 3;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause, clauseCount);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == 0);
}

void
test_Cnf_simplify_withFalseClause()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1, 0);

  int32_t clause[] = { 1, -2, 3 };
  size_t clause1Count = 3;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause, clause1Count);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == (cnf.count - 1));
}

void
test_Cnf_simplify_withMixedClauses()
{
  Assignment assignment;
  Assignment_create(&assignment);
  Assignment_set(&assignment, 1, 0);
  Assignment_set(&assignment, 5, 1);

  // this clause should have the 1 omitted
  int32_t clause1[] = { 1, -2, 3 };
  size_t clause1Count = 3;

  // this clause should be absent from the result
  int32_t clause2[] = { 3, -4, 5 };
  size_t clause2Count = 3;

  Cnf cnf;
  Cnf_create(&cnf);
  Cnf_pushClause(&cnf, clause1, clause1Count);
  Cnf_pushClause(&cnf, clause2, clause2Count);

  Cnf resultCnf;
  Cnf_create(&resultCnf);

  Cnf_simplify(&cnf, &assignment, &resultCnf);

  assert(resultCnf.count == 4);
  assert(resultCnf.pData[0] == 0);
  assert(resultCnf.pData[1] == -2);
  assert(resultCnf.pData[2] == 3);
  assert(resultCnf.pData[0] == 0);
}

int
main()
{
  test_Cnf_simplify_withEmptyAssignment();
  test_Cnf_simplify_withTrueClause();
  test_Cnf_simplify_withFalseClause();
  test_Cnf_simplify_withMixedClauses();
}
