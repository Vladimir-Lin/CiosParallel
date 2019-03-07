#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Mutexz:: Mutexz (void)
{
  MutexzHandler * mh = new MutexzHandler ( ) ;
  mh   -> handler = new PrivateMutexz ( )    ;
  this -> PrivatePacket = (void *) mh        ;
}

Mutexz::~Mutexz (void)
{
  if ( nullptr != this -> PrivatePacket )                        {
    MutexzHandler * mh = (MutexzHandler *) this -> PrivatePacket ;
    delete mh                                                    ;
    this -> PrivatePacket = nullptr                              ;
  }                                                              ;
}

Mutex & Mutexz::operator [ ] (int64_t index)
{
  MutexzHandler * mh = (MutexzHandler *) this -> PrivatePacket ;
  return ( * ( mh -> handler -> GetMutex ( index ) ) )         ;
}

Mutex & Mutexz::operator [ ] (std::string key)
{
  MutexzHandler * mh = (MutexzHandler *) this -> PrivatePacket ;
  return ( * ( mh -> handler -> GetMutex ( key ) ) )           ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
