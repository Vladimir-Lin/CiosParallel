#include "PrivateParallel.hpp"

#pragma pack(push,1)

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

SharedMemory:: SharedMemory (void)
{
  SharedMemoryHandler * ph = new SharedMemoryHandler ( ) ;
  ph   -> handler = new PrivateSharedMemory ( )          ;
  this -> PrivatePacket = (void *) ph                    ;
}

SharedMemory::~SharedMemory (void)
{
  if ( nullptr != this -> PrivatePacket )                                    {
    SharedMemoryHandler * ph  = (SharedMemoryHandler *)this -> PrivatePacket ;
    PrivateSharedMemory * psm = ph -> handler                                ;
    if ( nullptr != psm )                                                    {
      psm -> close ( )                                                       ;
      delete psm                                                             ;
    }                                                                        ;
    delete ph                                                                ;
    this -> PrivatePacket = nullptr                                          ;
  }                                                                          ;
}

bool SharedMemory::Create(std::string key,int64_t Size)
{
  std::wstring ws ( key . begin ( ) , key . end ( ) ) ;
  return this -> Create ( ws , Size )                 ;
}

bool SharedMemory::Create(std::wstring key,int64_t Size)
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> create ( key , Size )                                       ;
}

bool SharedMemory::Open(std::string key,int64_t Size)
{
  std::wstring ws ( key . begin ( ) , key . end ( ) ) ;
  return this -> Open ( ws , Size )                   ;
}

bool SharedMemory::Open(std::wstring key,int64_t Size)
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> open ( key , Size )                                         ;
}

int64_t SharedMemory::MemorySize(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0                          ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return 0                          ;
  return psm -> Size                                                        ;
}

bool SharedMemory::Close(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> close ( )                                                   ;
}

char * SharedMemory::Memory(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> Data                                                        ;
}

bool SharedMemory::isAttached(void) const
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> attached                                                    ;
}

bool SharedMemory::isCreated(void) const
{
  if ( nullptr == this -> PrivatePacket ) return false                      ;
  SharedMemoryHandler * ph  = (SharedMemoryHandler *) this -> PrivatePacket ;
  PrivateSharedMemory * psm = ph -> handler                                 ;
  if ( nullptr == psm                   ) return false                      ;
  return psm -> created                                                     ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
