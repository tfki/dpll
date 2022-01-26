
#include <cnf/dimacs.h>
#include <common/freadall.h>
#include <solver/dpll_solver.h>

int
main()
{
  char* filename = "res/SPLOT-3CNF-FM-5000-500-0.30-SAT-1.dimacs";
  char* dimacs;
  freadall(filename, &dimacs);

  // printf("%s", dimacs);

  Cnf cnf;
  Cnf_create(&cnf);

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  parseDimacs(dimacs, &cnf);

  return dpllSolve(&cnf, &dpllTrivialPick, &assignment);
}
