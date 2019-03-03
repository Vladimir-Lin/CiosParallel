#include "parallel.hpp"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma pack(push,1)


#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Thread:: Thread(void)
{
}

Thread::~Thread(void)
{
}

bool Thread::Recycling(void)
{
  return true ;
}

bool Thread::Destructor(void)
{
  return true ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
