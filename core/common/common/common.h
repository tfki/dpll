#ifndef DPLLSOLVER_COMMON_H
#define DPLLSOLVER_COMMON_H

#include <stdio.h>

#ifdef NDEBUG // TODO replace with SANITIZING compiler flag if
              //      the performance drops to much in debug mode

#define SANITIZING_ASSERT(c) ((void)0)

#else

#define SANITIZING_ASSERT(c)                                                                                                                                   \
  if (!(c)) {                                                                                                                                                  \
    printf("assert failed in file %s on line %d", __FILE__, __LINE__);                                                                                         \
    exit(-1);                                                                                                                                                  \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif
