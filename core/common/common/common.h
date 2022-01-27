#ifndef DPLLSOLVER_COMMON_H
#define DPLLSOLVER_COMMON_H

#ifdef NDEBUG

#define SANITIZING_ASSERT(c) ((void)0)

#else

#include "log.h"

#ifdef _WIN32
#include <windows.h>
#define SANITIZING_ASSERT(c, fmt, ...)                                                                                                                         \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed! (" #c "); " fmt, __VA_ARGS__);                                                                                                     \
    DebugBreak();                                                                                                                                              \
    ExitProcess(__LINE__);                                                                                                                                     \
  }                                                                                                                                                            \
  ((void)0)

#else
#define SANITIZING_ASSERT(c, fmt, ...)                                                                                                                                   \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed! (" #c "); " fmt, __VA_ARGS__);                                                                                                     \
    exit(__LINE__);                                                                                                                                            \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif

#endif
