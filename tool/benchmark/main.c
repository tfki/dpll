#include <solver/solver.h>

int
main()
{
  Cnf cnf;
  Cnf_create(&cnf);

  // TODO create complex cnf

  Assignment assignment;
  Assignment_create(&assignment);

  dpllSolve(&cnf, dpllTrivialPick, &assignment);


}
