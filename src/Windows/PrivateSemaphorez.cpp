#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

PrivateSemaphorez:: PrivateSemaphorez (void )
                  : spinLock          (false)
{
}

PrivateSemaphorez::~PrivateSemaphorez (void)
{
  spinLock = false                                ;
  /////////////////////////////////////////////////
  std::map<int64_t    ,Semaphore *>::iterator itz ;
  std::map<std::string,Semaphore *>::iterator itx ;
  /////////////////////////////////////////////////
  for ( itz  = mutex . begin ( )                  ;
        itz != mutex . end   ( )                  ;
        ++itz                                   ) {
    delete itz -> second                          ;
  }                                               ;
  /////////////////////////////////////////////////
  for ( itx  = sutex . begin ( )                  ;
        itx != sutex . end   ( )                  ;
        ++itx                                   ) {
    delete itx -> second                          ;
  }                                               ;
  /////////////////////////////////////////////////
  mutex . clear ( )                               ;
  sutex . clear ( )                               ;
}

void PrivateSemaphorez::SpinLock(void)
{
  while ( spinLock )                        {
    StarDate::uskip ( 1 , 100 , &spinLock ) ;
  }                                         ;
}

Semaphore * PrivateSemaphorez::GetSemaphore(int64_t index)
{
  SpinLock ( )                               ;
  spinLock = true                            ;
  std::map<int64_t,Semaphore *>::iterator it ;
  it = mutex . find ( index )                ;
  if ( it == mutex . end ( ) )               {
    mutex [ index ] = new Semaphore ( )      ;
  }                                          ;
  spinLock = false                           ;
  return mutex [ index ]                     ;
}

Semaphore * PrivateSemaphorez::GetSemaphore(std::string key)
{
  SpinLock ( )                                   ;
  spinLock = true                                ;
  std::map<std::string,Semaphore *>::iterator it ;
  it = sutex . find ( key )                      ;
  if ( it == sutex . end ( ) )                   {
    sutex [ key ] = new Semaphore ( )            ;
  }                                              ;
  spinLock = false                               ;
  return sutex [ key ]                           ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
