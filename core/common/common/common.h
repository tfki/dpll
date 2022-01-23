#ifndef DPLLSOLVER_COMMON_H
#define DPLLSOLVER_COMMON_H

#ifdef NDEBUG // TODO replace with SANITIZING compiler flag if
              //      the performance drops to much in debug mode

#define SANITIZING_ASSERT(c) ((void)0)

#else

#define SANITIZING_ASSERT(c)                                                                                                                                   \
  if (!(c)) {                                                                                                                                                  \
    exit(-1);                                                                                                                                                  \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif
