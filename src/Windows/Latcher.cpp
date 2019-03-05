#include "parallel.hpp"

#include <Windows.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma pack(push,1)

typedef struct                 {
  CRITICAL_SECTION mutex       ;
  bool             locked      ;
  bool             initialized ;
  int              type        ;
} LockerStructure              ;

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Latcher:: Latcher ( Locker * locker )
        : lock    (          locker )
{
  if ( nullptr != this -> lock ) {
    this -> lock -> lock ( )     ;
  }                              ;
}

Latcher::~Latcher(void)
{
  if ( nullptr != this -> lock ) {
    this -> lock -> unlock ( )   ;
  }                              ;
}

Locker * Latcher::mutex(void) const
{
  return this -> lock ;
}

void Latcher::relock(void)
{
  if ( nullptr == this -> lock ) return ;
  this -> lock -> lock ( )              ;
}

void Latcher::unlock(void)
{
  if ( nullptr == this -> lock ) return ;
  this -> lock -> unlock ( )            ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
