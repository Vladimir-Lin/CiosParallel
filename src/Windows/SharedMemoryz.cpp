#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

SharedMemoryz:: SharedMemoryz (void)
{
  SharedMemoryzHandler * sh = new SharedMemoryzHandler ( ) ;
  sh   -> handler = new PrivateSharedMemoryz ( )           ;
  this -> PrivatePacket = (void *) sh                      ;
}

SharedMemoryz::~SharedMemoryz (void)
{
  if ( nullptr != this -> PrivatePacket )                                    {
    SharedMemoryzHandler * sh = (SharedMemoryzHandler *) this->PrivatePacket ;
    delete sh                                                                ;
    this -> PrivatePacket = nullptr                                          ;
  }                                                                          ;
}

SharedMemory & SharedMemoryz::operator [ ] (int64_t index)
{
  SharedMemoryzHandler * sh = (SharedMemoryzHandler *) this -> PrivatePacket ;
  return ( * ( sh -> handler -> GetSharedMemory ( index ) ) )                ;
}

SharedMemory & SharedMemoryz::operator [ ] (std::string key)
{
  SharedMemoryzHandler * ph = (SharedMemoryzHandler *) this -> PrivatePacket ;
  return ( * ( ph -> handler -> GetSharedMemory ( key ) ) )                  ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
