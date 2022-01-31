#include <cnf/dimacs.h>
#include <common/common.h>
#include <common/freadall.h>
#include <common/log.h>
#include <solver/dpll_solver.h>

int
main()
{
  LOG_INIT();

  char* filename = "res/uf20-01.cnf";
  char* dimacs;
  freadall(filename, &dimacs);

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

  int status = dpllSolve(&cnf, &dpllTrivialPick, &assignment);
  Cnf_destroy(&cnf);
  AssignmentStack_destroy(&assignment);

  return status;
}
