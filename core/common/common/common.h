#ifndef DPLLSOLVER_COMMON_H
#define DPLLSOLVER_COMMON_H

#ifdef NDEBUG

#define SANITIZING_ASSERT(c) ((void)0)

#else

#ifdef _WIN32
#include <windows.h>
#define SANITIZING_ASSERT(c)                                                                                                                                   \
  if (!(c)) {                                                                                                                                                  \
    DebugBreak();                                                                                                                                              \
    ExitProcess(__LINE__);                                                                                                                                     \
  }                                                                                                                                                            \
  ((void)0)

#else

#define SANITIZING_ASSERT(c)                                                                                                                                   \
  if (!(c)) {                                                                                                                                                  \
    exit(__LINE__);                                                                                                                                            \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif

#endif
