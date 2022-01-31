#ifndef DPLLSOLVER_BENCH_H
#define DPLLSOLVER_BENCH_H

#include "sanitize.h"

#include <time.h>

typedef struct Bench
{
  time_t begin, end;
} Bench;

void
Bench_create(Bench* pBench)
{
  SANITIZE_PARAMETER_POINTER(pBench);
  pBench->begin = -1;
  pBench->end = -1;
}

int
Bench_begin(Bench* pBench)
{
  SANITIZE_PARAMETER_POINTER(pBench);
  pBench->begin = clock();
  return pBench->begin == -1 ? 1 : 0;
}

int
Bench_end(Bench* pBench)
{
  SANITIZE_PARAMETER_POINTER(pBench);
  pBench->end = clock();
  return pBench->end == -1 ? 1 : 0;
}

void
Bench_seconds(Bench* pBench, double* seconds)
{
  SANITIZE_PARAMETER_POINTER(pBench);
  SANITIZE_ASSERT(pBench->end >= pBench->begin, "Parameter pBench has invalid fields!");
  const time_t diff = pBench->end - pBench->begin;
  const double diffd = (double)diff;
  *seconds = diffd / ((double)CLOCKS_PER_SEC);
}

#endif
