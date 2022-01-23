#include <solver/solver.h>
#include <assert.h>

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
  // const int32_t clause0[] = { 10, -13, 11, -14 };
  // const int32_t clause1[] = { -10, -11 };
  // const int32_t clause2[] = { 16, -13, -11 };
  // const size_t clause0Count = 4u;
  // const size_t clause1Count = 2u;
  // const size_t clause2Count = 3u;

  // TODO

  Cnf cnf;
  Cnf_create(&cnf);

  Assignment assignment;
  Assignment_create(&assignment);

  assert(!dpllSolve(&cnf, dpllTrivialPick, &assignment));
}

int
main()
{
  test_dpllSolve_emptyCnf();
  test_dpllSolve_precalculatedCnf();
}
