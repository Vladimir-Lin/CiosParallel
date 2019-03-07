#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Mutex:: Mutex (void)
{
  MutexHandler * mh = new MutexHandler ( )                     ;
  mh   -> mutex  = ::CreateMutex ( nullptr , FALSE , nullptr ) ;
  mh   -> locked = false                                       ;
  this -> PrivatePacket = (void *) mh                          ;
}

Mutex::~Mutex (void)
{
  if ( nullptr != this -> PrivatePacket )                      {
    MutexHandler * mh = (MutexHandler *) this -> PrivatePacket ;
    ::CloseHandle ( mh -> mutex )                              ;
    delete mh                                                  ;
    this -> PrivatePacket = nullptr                            ;
  }                                                            ;
}

int Mutex::lock(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0           ;
  MutexHandler * mh = (MutexHandler *) this -> PrivatePacket ;
  mh -> locked = true                                        ;
  ::WaitForSingleObject ( mh -> mutex , INFINITE )           ;
  return 1                                                   ;
}

int Mutex::unlock(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0           ;
  MutexHandler * mh = (MutexHandler *) this -> PrivatePacket ;
  ::ReleaseMutex ( mh -> mutex )                             ;
  mh -> locked = false                                       ;
  return 1                                                   ;
}

int Mutex::locked(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0           ;
  MutexHandler * mh = (MutexHandler *) this -> PrivatePacket ;
  return ( mh -> locked ) ? 1 : 0                            ;
}

int Mutex::trylock(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0           ;
  MutexHandler * mh = (MutexHandler *) this -> PrivatePacket ;
  if ( mh -> locked ) return 0                               ;
  return this -> lock ( )                                    ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
