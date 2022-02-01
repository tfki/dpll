#ifndef DPLLSOLVER_SANITIZE_H
#define DPLLSOLVER_SANITIZE_H

#include "debugbreak.h"

#include <stdbool.h>

#ifdef NDEBUG

#define SANITIZE_ASSERT(c, ...) ((void)0)

#else

#include "log.h"

#define SANITIZE_ASSERT(c, ...)                                                                                                                                \
  if (!(c)) {                                                                                                                                                  \
    LOGE("Assertion failed! (" #c "); " __VA_ARGS__);                                                                                                          \
    DEBUG_BREAK();                                                                                                                                              \
    exit(1);                                                                                                                                     \
  }                                                                                                                                                            \
  ((void)0)

#endif

#define SANITIZE_PARAMETER_POINTER(p) SANITIZE_ASSERT(p, "Parameter " #p " must be a valid pointer!")
#define SANITIZE_PARAMETER_BOOL(b) SANITIZE_ASSERT((((b) == true) || ((b) == false)), "Parameter " #b " must be a \"normalized\" bool!")

#endif
