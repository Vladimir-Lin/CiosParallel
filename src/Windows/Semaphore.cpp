#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

Semaphore:: Semaphore(int amount)
{
  SemaphoreHandler * sh = new SemaphoreHandler ( )                         ;
  sh   -> semaphore = ::CreateSemaphore ( nullptr , 0 , amount , nullptr ) ;
  sh   -> amount    = amount                                               ;
  sh   -> acquired  = 0                                                    ;
  this -> PrivatePacket = (void *) sh                                      ;
}

Semaphore::~Semaphore(void)
{
  if ( nullptr != this -> PrivatePacket )                                {
    SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
    if ( sh -> acquired > 0 )                                            {
      ::ReleaseSemaphore ( sh -> semaphore , sh -> acquired , nullptr )  ;
    }                                                                    ;
    ::CloseHandle ( sh -> semaphore )                                    ;
    delete sh                                                            ;
    this -> PrivatePacket = nullptr                                      ;
  }                                                                      ;
}

int Semaphore::setAmount(int amount)
{
  if ( nullptr == this -> PrivatePacket ) return 0                       ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket   ;
  if ( sh -> acquired > 0 ) return sh -> amount                          ;
  ::CloseHandle ( sh -> semaphore )                                      ;
  sh -> semaphore = ::CreateSemaphore ( nullptr , 0 , amount , nullptr ) ;
  sh -> amount    = amount                                               ;
  sh -> acquired  = 0                                                    ;
  return sh -> amount                                                    ;
}

void Semaphore::acquire(int n)
{
  if ( nullptr == this -> PrivatePacket ) return                       ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
  for (int i = 0 ; i < n ; i++ )                                       {
    sh -> acquired ++                                                  ;
    ::WaitForSingleObject ( sh -> semaphore , INFINITE )               ;
  }                                                                    ;
}

int Semaphore::available(void) const
{
  if ( nullptr == this -> PrivatePacket ) return 0                     ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
  return ( sh -> amount - sh -> acquired )                             ;
}

void Semaphore::release(int n)
{
  if ( nullptr == this -> PrivatePacket ) return                       ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
  for (int i = 0 ; i < n ; i++ )                                       {
    ::ReleaseSemaphore ( sh -> semaphore , 1 , nullptr )               ;
    sh -> acquired --                                                  ;
  }                                                                    ;
}

bool Semaphore::tryAcquire(int n)
{
  if ( nullptr == this -> PrivatePacket ) return false                 ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
  for (int i = 0 ; i < n ; i++ )                                       {
    if ( sh -> acquired >= sh -> amount ) return false                 ;
    sh -> acquired ++                                                  ;
    ::WaitForSingleObject ( sh -> semaphore , INFINITE )               ;
  }                                                                    ;
  return true                                                          ;
}

bool Semaphore::tryAcquire(int n,int timeoutMs)
{
  if ( nullptr == this -> PrivatePacket ) return false                 ;
  SemaphoreHandler * sh = ( SemaphoreHandler * ) this -> PrivatePacket ;
  int64_t ts = StarDate::ustamp ( )                                    ;
  ts += ( timeoutMs * 1000 )                                           ;
  for (int i = 0 ; i < n ; i++ )                                       {
    if ( sh -> acquired >= sh -> amount )                              {
      while ( ( ts > StarDate::ustamp ( )      )                      &&
              ( sh -> acquired >= sh -> amount )                     ) {
        StarDate::usleep ( 100 )                                       ;
      }                                                                ;
      if ( sh -> acquired < sh -> amount )                             {
        sh -> acquired ++                                              ;
        ::WaitForSingleObject ( sh -> semaphore , INFINITE )           ;
      } else return false                                              ;
    } else                                                             {
      sh -> acquired ++                                                ;
      ::WaitForSingleObject   ( sh -> semaphore , INFINITE )           ;
    }                                                                  ;
  }                                                                    ;
  return true                                                          ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
