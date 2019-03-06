#include "PrivateParallel.hpp"

#pragma pack(push,1)


#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

DWORD WindowsThreadId(void)
{
  return ::GetCurrentThreadId ( ) ;
}


#ifndef DONT_USE_NAMESPACE
}
#endif
