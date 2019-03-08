#include "parallel.hpp"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma pack(push,1)


#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

void PrintDebugger(const char * fmt,...)
{
  va_list args             ;
  va_start ( args , fmt  ) ;
  vprintf  ( fmt  , args ) ;
  va_end   ( args        ) ;
}

DebugFunction CiosDebugger = PrintDebugger ;

void setDebugFunction(DebugFunction debugger)
{
  CiosDebugger = debugger ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
