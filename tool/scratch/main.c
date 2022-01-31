#include <cnf/dimacs.h>
#include <solver/dpll_solver.h>

#include <common/fread.h>
#include <common/bench.h>

int
main()
{
  LOG_INIT();

  char* filename = "res/uf20-01.cnf";
  char* dimacs;
  fReadAll(filename, &dimacs);

  Cnf cnf;
  Cnf_create(&cnf);
  fReadCnf(filename, &cnf);

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  Bench bench;
  Bench_create(&bench);

  Bench_begin(&bench);
  int status = dpllSolve(&cnf, &dpllTrivialPick, &assignment);
  Bench_end(&bench);

  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);

  return status;
}
