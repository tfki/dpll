
#include <cnf/dimacs.h>
#include <common/freadall.h>
#include <common/log.h>
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

  LOGF("no formatting");
  LOGE("format a %%d: %d", 1);
  LOGW("format a %%s: %s", "Hallo log");
  LOGI("just print an info ...");
  LOGD("just print a debug ...");

  return dpllSolve(&cnf, &dpllTrivialPick, &assignment);
}
