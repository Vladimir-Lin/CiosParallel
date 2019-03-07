#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Lockerz:: Lockerz(void)
{
  LockerzHandler * ph = new LockerzHandler ( ) ;
  ph   -> handler = new PrivateLockerz ( )     ;
  this -> PrivatePacket = (void *) ph          ;
}

Lockerz::~Lockerz(void)
{
  if ( nullptr != this -> PrivatePacket )                          {
    LockerzHandler * ph = (LockerzHandler *) this -> PrivatePacket ;
    delete ph                                                      ;
    this -> PrivatePacket = nullptr                                ;
  }                                                                ;
}

Locker & Lockerz::operator [ ] (int64_t index)
{
  LockerzHandler * ph = (LockerzHandler *) this -> PrivatePacket ;
  return ( * ( ph->handler->GetLocker(index) ) )                 ;
}

Locker & Lockerz::operator [ ] (std::string key)
{
  LockerzHandler * ph = (LockerzHandler *) this -> PrivatePacket ;
  return ( * ( ph->handler->GetLocker(key) ) )                   ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
