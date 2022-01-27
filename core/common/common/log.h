#ifndef DPLLSOLVER_LOG_H
#define DPLLSOLVER_LOG_H

#include <stdio.h>

#define LOG_LEVEL_FATAL 0b00001
#define LOG_LEVEL_ERROR 0b00011
#define LOG_LEVEL_WARNING 0b00111
#define LOG_LEVEL_INFO 0b01111
#define LOG_LEVEL_DEBUG 0b11111

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif

#define LOG(lvl, fmt, ...)                                                                                                                                     \
  printf("%-5s : %s (%d)\n", lvl, __FILE__, __LINE__);                                                                                                         \
  printf("      : " fmt "\n", __VA_ARGS__);                                                                                                                     \
  fflush(stdout)\

#if LOG_LEVEL_FATAL <= LOG_LEVEL
#define LOGF(fmt, ...) LOG("Fatal", fmt, __VA_ARGS__)
#else
#define LOGF(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL_ERROR <= LOG_LEVEL
#define LOGE(fmt, ...) LOG("Error", fmt, __VA_ARGS__)
#else
#define LOGE(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL_WARNING <= LOG_LEVEL
#define LOGW(fmt, ...) LOG("Warn", fmt, __VA_ARGS__)
#else
#define LOGW(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL_INFO <= LOG_LEVEL
#define LOGI(fmt, ...) LOG("Info", fmt, __VA_ARGS__)
#else
#define LOGI(fmt, ...) ((void)0)
#endif

#if LOG_LEVEL_DEBUG <= LOG_LEVEL
#define LOGD(fmt, ...) LOG("Debug", fmt, __VA_ARGS__)
#else
#define LOGD(fmt, ...) ((void)0)
#endif

#endif
