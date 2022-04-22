#ifndef DETECT_LEAKS_HPP__
#define DETECT_LEAKS_HPP__

#define _CRTDBG_MAP_ALLOC

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <crtdbg.h>

#define new new(_CLIENT_BLOCK,__FILE__, __LINE__)

// The following macros set and clear, respectively, given bits
// of the C runtime library debug flag, as specified by a bitmask.
#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
    _CrtSetDbgFlag((a) | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
    _CrtSetDbgFlag(~(a) & _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

void start_detecting();

#endif // DETECT_LEAKS_HPP__
