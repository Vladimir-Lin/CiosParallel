#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

PrivateLockerz:: PrivateLockerz (void )
               : spinLock       (false)
{
}

PrivateLockerz::~PrivateLockerz (void )
{
  spinLock = false                             ;
  //////////////////////////////////////////////
  std::map<int64_t    ,Locker *>::iterator itz ;
  std::map<std::string,Locker *>::iterator itx ;
  //////////////////////////////////////////////
  for ( itz  = mutex . begin ( )               ;
        itz != mutex . end   ( )               ;
        ++itz                                ) {
    delete itz -> second                       ;
  }                                            ;
  //////////////////////////////////////////////
  for ( itx  = sutex . begin ( )               ;
        itx != sutex . end   ( )               ;
        ++itx                                ) {
    delete itx -> second                       ;
  }                                            ;
  //////////////////////////////////////////////
  mutex . clear ( )                            ;
  sutex . clear ( )                            ;
}

void PrivateLockerz::SpinLock(void)
{
  while ( spinLock )                        {
    StarDate::uskip ( 1 , 100 , &spinLock ) ;
  }                                         ;
}

Locker * PrivateLockerz::GetLocker(int64_t index)
{
  SpinLock ( )                            ;
  spinLock = true                         ;
  std::map<int64_t,Locker *>::iterator it ;
  it = mutex . find ( index )             ;
  if ( it == mutex . end ( ) )            {
    mutex [ index ] = new Locker ( )      ;
  }                                       ;
  spinLock = false                        ;
  return mutex [ index ]                  ;
}

Locker * PrivateLockerz::GetLocker(std::string key)
{
  SpinLock ( )                                ;
  spinLock = true                             ;
  std::map<std::string,Locker *>::iterator it ;
  it = sutex . find ( key )                   ;
  if ( it == sutex . end ( ) )                {
    sutex [ key ] = new Locker ( )            ;
  }                                           ;
  spinLock = false                            ;
  return sutex [ key ]                        ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
