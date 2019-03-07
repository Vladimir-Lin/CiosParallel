#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Semaphorez:: Semaphorez (void)
{
  SemaphorezHandler * sh = new SemaphorezHandler ( ) ;
  sh   -> handler = new PrivateSemaphorez ( )        ;
  this -> PrivatePacket = (void *) sh                ;
}

Semaphorez::~Semaphorez (void)
{
  if ( nullptr != this -> PrivatePacket )                                {
    SemaphorezHandler * sh = (SemaphorezHandler *) this -> PrivatePacket ;
    delete sh                                                            ;
    this -> PrivatePacket = nullptr                                      ;
  }                                                                      ;
}

Semaphore & Semaphorez::operator [ ] (int64_t index)
{
  SemaphorezHandler * sh = (SemaphorezHandler *) this -> PrivatePacket ;
  return ( * ( sh -> handler -> GetSemaphore ( index ) ) )             ;
}

Semaphore & Semaphorez::operator [ ] (std::string key)
{
  SemaphorezHandler * ph = (SemaphorezHandler *) this -> PrivatePacket ;
  return ( * ( ph -> handler -> GetSemaphore ( key ) ) )               ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
