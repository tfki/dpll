#ifndef DPLLSOLVER_SANITIZE_H
#define DPLLSOLVER_SANITIZE_H

#include <stdbool.h>

#ifdef NDEBUG

#define SANITIZE_ASSERT(c, ...) ((void)0)

#else

#include "log.h"

#ifdef _WIN32
#include <windows.h>
#define SANITIZE_ASSERT(c, ...)                                                                                                                                \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed! (" #c "); " __VA_ARGS__);                                                                                                          \
    DebugBreak();                                                                                                                                              \
    ExitProcess(__LINE__);                                                                                                                                     \
  }                                                                                                                                                            \
  ((void)0)

#else
#define SANITIZE_ASSERT(c, ...)                                                                                                                                \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed! (" #c "); " __VA_ARGS__);                                                                                                          \
    exit(__LINE__);                                                                                                                                            \
  }                                                                                                                                                            \
  ((void)0)

#endif

#endif

#define SANITIZE_PARAMETER_POINTER(p) SANITIZE_ASSERT(p, "Parameter " #p " must be a valid pointer!")
#define SANITIZE_PARAMETER_BOOL(b) SANITIZE_ASSERT((((b) == true) || ((b) == false)), "Parameter " #b " must be a \"normalized\" bool!")

#endif
