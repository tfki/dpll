#ifndef DPLLSOLVER_LOG_H
#define DPLLSOLVER_LOG_H

#include <stdio.h>

#ifndef LOG_LEVEL
#define LOG_LEVEL 4
#endif

// optional macro for speeding up the print function
#define LOG_INIT()                                                                                                                                             \
  setvbuf(stdout, NULL, _IOFBF, 2048u);                                                                                                                        \
  setvbuf(stderr, NULL, _IONBF, 0u)

#define LOG(iobuf, lvl, ...)                                                                                                                                   \
  fprintf(iobuf, "%-5s : %s (%d)\n", lvl, __FILE__, __LINE__);                                                                                                 \
  fprintf(iobuf, "      : " __VA_ARGS__);                                                                                                                      \
  fprintf(iobuf, "\n")

#if 1 <= LOG_LEVEL
#define LOGF(...) LOG(stderr, "Fatal", __VA_ARGS__)
#else
#define LOGF(...) ((void)0)
#endif

#if 2 <= LOG_LEVEL
#define LOGE(...) LOG(stderr, "Error", __VA_ARGS__)
#else
#define LOGE(...) ((void)0)
#endif

#if 3 <= LOG_LEVEL
#define LOGW(...) LOG(stdout, "Warn", __VA_ARGS__)
#else
#define LOGW(...) ((void)0)
#endif

#if 4 <= LOG_LEVEL
#define LOGI(...) LOG(stdout, "Info", __VA_ARGS__)
#else
#define LOGI(...) ((void)0)
#endif

#if 5 <= LOG_LEVEL
#define LOGD(...) LOG(stdout, "Debug", __VA_ARGS__)
#else
#define LOGD(...) ((void)0)
#endif

#endif
