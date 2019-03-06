/****************************************************************************\
 *
 *                         CIOS Parallel System
 *
\****************************************************************************/

#ifndef PRIVATE_PARALLEL_HPP
#define PRIVATE_PARALLEL_HPP

#include "parallel.hpp"

#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <direct.h>
#include <process.h>
#include <Windows.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#if !defined(PARALLEL_NAMESPACE)
#define PARALLEL_NAMESPACE CIOS
#endif

// Debug
#ifdef CIOSDEBUG

#endif

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

#pragma pack(push,1)

#pragma pack(pop)

#define THREADSTACKSIZE ( 16 * 1024 * 1024 )
typedef unsigned int (*ThreadFunction)(LPVOID arg) ;

typedef struct              {
  HANDLE         Thread     ;
  DWORD          ThreadID   ;
  unsigned int   dwThreadID ;
  ThreadFunction Function   ;
  void         * Data       ;
} PrivateThreadData         ;

extern DWORD WindowsThreadId(void) ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
