#ifndef DPLLSOLVER_DEBUGBREAK_H
#define DPLLSOLVER_DEBUGBREAK_H

#ifdef NDEBUG

#define DEBUG_BREAK() ((void)0)

#else

#ifdef _WIN32
#include <windows.h>
#define DEBUG_BREAK() DebugBreak()

#else
#define DEBUG_BREAK() ((void)0)

#endif

#endif

#define ON_ERROR(e) if(!(e))

#endif
