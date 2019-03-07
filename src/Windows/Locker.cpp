#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Locker:: Locker(void)
{
  LockerStructure * ls = new LockerStructure ( ) ;
  this -> PrivatePacket = (void *) ls            ;
  ls   -> locked        = false                  ;
  ls   -> initialized   = false                  ;
  setType ( Default )                            ;
}

Locker::~Locker(void)
{
  this -> release ( )                                                ;
  if ( nullptr != this -> PrivatePacket )                            {
    LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
    delete ls                                                        ;
    this -> PrivatePacket = nullptr                                  ;
  }                                                                  ;
}

void Locker::release(void)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return                                      ;
  if ( ls -> initialized )                                         {
    ::DeleteCriticalSection ( & ( ls -> mutex ) )                  ;
    ls -> initialized = false                                      ;
  }                                                                ;
}

int Locker::type(void) const
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return -1                                   ;
  return ls -> type                                                ;
}

void Locker::setType(Types MT)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return                                      ;
  if ( ! ls -> initialized )                                       {
    ::InitializeCriticalSection ( & ( ls -> mutex ) )              ;
    ls -> initialized = true                                       ;
  }                                                                ;
  ls -> type = (int) MT                                            ;
}

int Locker::lock(void)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return 0                                    ;
  if ( ls -> locked  ) return 0                                    ;
  ls -> locked = true                                              ;
  ::EnterCriticalSection ( & ( ls -> mutex ) )                     ;
  return 1                                                         ;
}

int Locker::unlock(void)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls  ) return 0                                   ;
  if ( ! ls -> locked ) return 0                                   ;
  ::LeaveCriticalSection ( & ( ls -> mutex ) )                     ;
  ls -> locked = false                                             ;
  return 1                                                         ;
}

int Locker::locked(void)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return 0                                    ;
  return ( ( ls -> locked ) ? 1 : 0 )                              ;
}

int Locker::trylock(void)
{
  LockerStructure * ls = (LockerStructure *) this -> PrivatePacket ;
  if ( nullptr == ls ) return 0                                    ;
  if ( ls -> locked  ) return 0                                    ;
  return this -> lock ( )                                          ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
