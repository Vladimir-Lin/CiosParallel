#include "parallel.hpp"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma pack(push,1)


#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

ThreadData:: ThreadData(void)
{
}

ThreadData::~ThreadData(void)
{
}

bool ThreadData::Recycling(void)
{
  return true ;
}

bool ThreadData::Destructor(void)
{
  return true ;
}

void * ThreadData::Register(void *)
{
  return nullptr ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
