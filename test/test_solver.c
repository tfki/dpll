#include <assert.h>
#include <solver/solver.h>

void
test_dpllSolve_emptyCnf()
{
  Cnf cnf;
  Cnf_create(&cnf);

  Assignment assignment;
  Assignment_create(&assignment);

  assert(!dpllSolve(&cnf, dpllTrivialPick, &assignment));
}

void
test_dpllSolve_precalculatedCnf()
{
  // { C , D }, { A }, { -B }, { C }, { D }, { B , -A, D }
  // { 3 , 4 }, { 1 }, { -2 }, { 3 }, { 4 }, { 2 , -1, 4 }

  const uint32_t variables[] = { 1u, 2u, 3u, 4u };
  const int8_t result[] = { 1, 0, 1, 1 };
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
  assert(!Cnf_pushClause(&cnf, clause1, clause1Count));
  assert(!Cnf_pushClause(&cnf, clause2, clause2Count));
  assert(!Cnf_pushClause(&cnf, clause3, clause3Count));
  assert(!Cnf_pushClause(&cnf, clause4, clause4Count));
  assert(!Cnf_pushClause(&cnf, clause5, clause5Count));
  assert(!Cnf_pushClause(&cnf, clause6, clause6Count));

  Assignment assignment;
  Assignment_create(&assignment);

  assert(!dpllSolve(&cnf, dpllTrivialPick, &assignment));

  for (size_t i = 0u; i < variablesCount; ++i) {
    int8_t value;
    assert(!Assignment_get(&assignment, variables[i], &value));
    assert(value == result[i]);
  }

  Assignment_destroy(&assignment);
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
  assert(!Cnf_pushClause(&cnf, clause1, clause1Count));
  assert(!Cnf_pushClause(&cnf, clause2, clause2Count));

  Assignment assignment;
  Assignment_create(&assignment);

  assert(dpllSolve(&cnf, dpllTrivialPick, &assignment));

  Assignment_destroy(&assignment);
  Cnf_destroy(&cnf);
}

int
main()
{
  test_dpllSolve_emptyCnf();
  test_dpllSolve_precalculatedCnf();
  test_dpllSolve_unsatisfiableCnf();
}
