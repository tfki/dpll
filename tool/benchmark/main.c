#include <solver/solver.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>

int
main()
{
  Cnf cnf;
  Cnf_create(&cnf);

  // TODO create complex cnf

  Assignment assignment;
  Assignment_create(&assignment);


  float start = ((float)clock()) / CLOCKS_PER_SEC;

  assert(!dpllSolve(&cnf, dpllTrivialPick, &assignment));

  float end = ((float)clock()) / CLOCKS_PER_SEC;
  float time = end - start;

  printf("time to solve: %f sec.", time);
}
