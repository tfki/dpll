
#include <cnf/dimacs.h>
#include <solver/dpll_solver.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
freadall(const char* fileName, char** raw)
{
  char buffer[1024u];
  FILE* file = fopen(fileName, "rb");

  if (!file)
    return -1;

  fseek(file, 0l, SEEK_END);
  const size_t fileSize = ftell(file);
  fseek(file, 0l, SEEK_SET);

  *raw = malloc(fileSize + 1);
  if (!*raw) {
    printf("penis");
    return 1;
  }

  size_t currentPos = 0u;
  size_t numRead = fread(*raw, fileSize, 1u, file);

  (*raw)[fileSize] = '\0';
  return 0;
}

int
main()
{
  char* filename = "SPLOT-3CNF-FM-5000-500-0.30-SAT-1.dimacs";
  char* dimacs;
  size_t size;
  freadall(filename, &dimacs);

  Cnf cnf;
  Cnf_create(&cnf);

  AssignmentStack assignment;
  AssignmentStack_create(&assignment);

  parseDimacs(dimacs, &cnf);

  return dpllSolve(&cnf, &dpllTrivialPick, &assignment);
}
