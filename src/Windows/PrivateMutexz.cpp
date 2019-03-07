#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

PrivateMutexz:: PrivateMutexz (void )
              : spinLock      (false)
{
}

PrivateMutexz::~PrivateMutexz (void)
{
  spinLock = false                            ;
  /////////////////////////////////////////////
  std::map<int64_t    ,Mutex *>::iterator itz ;
  std::map<std::string,Mutex *>::iterator itx ;
  /////////////////////////////////////////////
  for ( itz  = mutex . begin ( )              ;
        itz != mutex . end   ( )              ;
        ++itz                               ) {
    delete itz -> second                      ;
  }                                           ;
  /////////////////////////////////////////////
  for ( itx  = sutex . begin ( )              ;
        itx != sutex . end   ( )              ;
        ++itx                               ) {
    delete itx -> second                      ;
  }                                           ;
  /////////////////////////////////////////////
  mutex . clear ( )                           ;
  sutex . clear ( )                           ;
}

void PrivateMutexz::SpinLock(void)
{
  while ( spinLock )                        {
    StarDate::uskip ( 1 , 100 , &spinLock ) ;
  }                                         ;
}

Mutex * PrivateMutexz::GetMutex(int64_t index)
{
  SpinLock ( )                           ;
  spinLock = true                        ;
  std::map<int64_t,Mutex *>::iterator it ;
  it = mutex . find ( index )            ;
  if ( it == mutex . end ( ) )           {
    mutex [ index ] = new Mutex ( )      ;
  }                                      ;
  spinLock = false                       ;
  return mutex [ index ]                 ;
}

Mutex * PrivateMutexz::GetMutex(std::string key)
{
  SpinLock ( )                               ;
  spinLock = true                            ;
  std::map<std::string,Mutex *>::iterator it ;
  it = sutex . find ( key )                  ;
  if ( it == sutex . end ( ) )               {
    sutex [ key ] = new Mutex ( )            ;
  }                                          ;
  spinLock = false                           ;
  return sutex [ key ]                       ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
