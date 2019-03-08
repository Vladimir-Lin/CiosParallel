#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

PrivateSharedMemoryz:: PrivateSharedMemoryz (void )
                     : spinLock             (false)
{
}

PrivateSharedMemoryz::~PrivateSharedMemoryz (void)
{
  spinLock = false                                   ;
  ////////////////////////////////////////////////////
  std::map<int64_t    ,SharedMemory *>::iterator itz ;
  std::map<std::string,SharedMemory *>::iterator itx ;
  ////////////////////////////////////////////////////
  for ( itz  = mutex . begin ( )                     ;
        itz != mutex . end   ( )                     ;
        ++itz                                      ) {
    delete itz -> second                             ;
  }                                                  ;
  ////////////////////////////////////////////////////
  for ( itx  = sutex . begin ( )                     ;
        itx != sutex . end   ( )                     ;
        ++itx                                      ) {
    delete itx -> second                             ;
  }                                                  ;
  ////////////////////////////////////////////////////
  mutex . clear ( )                                  ;
  sutex . clear ( )                                  ;
}

void PrivateSharedMemoryz::SpinLock(void)
{
  while ( spinLock )                        {
    StarDate::uskip ( 1 , 100 , &spinLock ) ;
  }                                         ;
}

SharedMemory * PrivateSharedMemoryz::GetSharedMemory(int64_t index)
{
  SpinLock ( )                                  ;
  spinLock = true                               ;
  std::map<int64_t,SharedMemory *>::iterator it ;
  it = mutex . find ( index )                   ;
  if ( it == mutex . end ( ) )                  {
    mutex [ index ] = new SharedMemory ( )      ;
  }                                             ;
  spinLock = false                              ;
  return mutex [ index ]                        ;
}

SharedMemory * PrivateSharedMemoryz::GetSharedMemory(std::string key)
{
  SpinLock ( )                                      ;
  spinLock = true                                   ;
  std::map<std::string,SharedMemory *>::iterator it ;
  it = sutex . find ( key )                         ;
  if ( it == sutex . end ( ) )                      {
    sutex [ key ] = new SharedMemory ( )            ;
  }                                                 ;
  spinLock = false                                  ;
  return sutex [ key ]                              ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
