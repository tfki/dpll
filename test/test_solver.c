#include <solver/solver.h>
#include <assert.h>

int32_t
pickAndRemoveTrivial(const cnf* pCnf)
{
  return pCnf->pData[0];
}

void test_dpllSolve_emptyCnf()
{
  cnf cnf0;
  cnf_create(&cnf0);

  assignment assignment0;
  assignment_create(&assignment0);

  assert(!dpll_solve(&cnf0, pickAndRemoveTrivial, &assignment0));
}

void test_dpllSolve_precalculatedCnf()
{
  const int32_t clause0[] = { 10, -13, 11, -14 };
  const int32_t clause1[] = { -10, -11 };
  const int32_t clause2[] = { 16, -13, -11 };
  const size_t clause0_count = 4u;
  const size_t clause1_count = 2u;
  const size_t clause2_count = 3u;

  // TODO

  cnf cnf0;
  cnf_create(&cnf0);

  assignment assignment0;
  assignment_create(&assignment0);

  assert(!dpll_solve(&cnf0, pickAndRemoveTrivial, &assignment0));
}

int
main()
{
  test_dpllSolve_emptyCnf();
}
