#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

ThreadData:: ThreadData    ( void    )
           : Destroyer     (         )
           , Id            ( 0       )
           , Uuid          ( 0       )
           , Type          ( 0       )
           , Priority      ( 0       )
           , Status        ( 0       )
           , Running       ( Idle    )
           , StackSize     ( 0       )
           , Reservation   ( false   )
           , isContinue    ( true    )
           , Controller    ( nullptr )
           , Arguments     ( nullptr )
           , Extra         ( nullptr )
           , PrivatePacket ( nullptr )
{
  PrivateThreadData * ptd = new PrivateThreadData ( ) ;
  this -> PrivatePacket = (void *) ptd                ;
  ptd  -> Thread        = nullptr                     ;
  ptd  -> ThreadID      = 0                           ;
  ptd  -> ParentID      = 0                           ;
  ptd  -> dwThreadID    = 0                           ;
  ptd  -> Function      = nullptr                     ;
  ptd  -> Data          = nullptr                     ;
  ptd  -> thread        = nullptr                     ;
  ptd  -> threadData    = nullptr                     ;
}

ThreadData::~ThreadData(void)
{
}

bool ThreadData::Recycling(void)
{
  return true ;
}

bool ThreadData::Destructor(void)
{
  ///////////////////////////////////////////////////////////////////////
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  delete ptd                                                            ;
  this -> PrivatePacket = nullptr                                       ;
  ///////////////////////////////////////////////////////////////////////
  return true                                                           ;
}

void * ThreadData::Register(void *)
{
  return nullptr ;
}

void * ThreadData::Hidden(void)
{
  return this -> PrivatePacket ;
}

void ThreadData::Start(void)
{
  this -> Running = Active ;
}

void ThreadData::Stop(void)
{
  this -> Running = Deactive ;
}

void ThreadData::Join(void)
{
  if ( Deactive != this -> Running ) return                               ;
  /////////////////////////////////////////////////////////////////////////
  if ( nullptr != this -> PrivatePacket )                                 {
    PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
    if ( nullptr != ptd -> Thread )                                       {
      ::WaitForSingleObject ( ptd -> Thread , INFINITE )                  ;
    }                                                                     ;
  }                                                                       ;
  /////////////////////////////////////////////////////////////////////////
  this -> Running = Recycle                                               ;
}

bool ThreadData::YieldThread(void)
{
  return (bool) ::SwitchToThread ( ) ;
}

bool ThreadData::Suspend(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////
  DWORD r = ::SuspendThread ( ptd -> Thread )                           ;
  if ( r == ( (DWORD) -1 ) ) return false                               ;
  return true                                                           ;
}

bool ThreadData::Resume(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////
  DWORD r = ::ResumeThread  ( ptd -> Thread )                           ;
  if ( r == ( (DWORD) -1 ) ) return false                               ;
  return true                                                           ;
}

bool ThreadData::Cancel(int exitCode)
{
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////
  return (bool) ::TerminateThread ( ptd -> Thread , exitCode )          ;
}

void ThreadData::TryCancel(void)
{
  if ( ! this -> isAlive ( ) ) return ;
  this -> Cancel ( 0 )                ;
}

int ThreadData::setPriority(int priority)
{
  if ( nullptr == this -> PrivatePacket ) return this -> Priority       ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  if ( nullptr == ptd -> Thread ) return this -> Priority               ;
  ///////////////////////////////////////////////////////////////////////
  ::SetThreadPriority  ( ptd -> Thread , priority )                     ;
  this -> Priority = ::GetThreadPriority ( ptd -> Thread )              ;
  return this -> Priority                                               ;
}

bool ThreadData::isAlive(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  if ( nullptr == ptd -> Thread ) return false                          ;
  ///////////////////////////////////////////////////////////////////////
  DWORD result = WaitForSingleObject ( ptd -> Thread , 0 )              ;
  if ( WAIT_OBJECT_0 == result ) return false                           ;
  ///////////////////////////////////////////////////////////////////////
  return true                                                           ;
}

bool ThreadData::isSelf(void)
{
  if ( nullptr == this -> PrivatePacket ) return false                  ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////
  return ( WindowsThreadId ( ) == ptd -> ThreadID )                     ;
}

bool ThreadData::isEqual(int32_t id)
{
  return ( id == this -> Id ) ;
}

bool ThreadData::canContinue (void)
{
  if ( ! this -> isContinue          ) return false ;
  if ( nullptr == this -> Controller ) return true  ;
  return * ( this -> Controller )                   ;
}

void ThreadData::AssignId(void)
{
  if ( nullptr == this -> PrivatePacket ) return                        ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
  ///////////////////////////////////////////////////////////////////////
  ptd -> ThreadID = WindowsThreadId ( )                                 ;
}

bool ThreadData::Run(void * data)
{
  Latcher latch ( &lock )                                                    ;
  if ( nullptr == this -> PrivatePacket ) return false                       ;
  PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket      ;
  int ss = 0                                                                 ;
  if ( this -> Reservation ) ss = this -> StackSize                          ;
  ptd -> ParentID = WindowsThreadId ( )                                      ;
  ptd -> Data     = data                                                     ;
  #ifdef CIOS_X64
  ptd -> Thread = (HANDLE) ::_beginthreadex                                  (
                      NULL                                                   ,
                      ss                                                     ,
                      ptd -> Function                                        ,
                      (LPVOID) ptd                                           ,
                      0                                                      ,
                      & ptd -> dwThreadID                                  ) ;
  #elif CIOS_X86
  ptd -> Thread = (HANDLE) ::_beginthreadex                                  (
                      NULL                                                   ,
                      ss                                                     ,
                      (unsigned int (__stdcall *)(void *)) ptd -> Function   ,
                      (LPVOID) ptd                                           ,
                      0                                                      ,
                      & ptd -> dwThreadID                                  ) ;
  #endif
  return  ( nullptr != ptd -> Thread )                                       ;
}

bool ThreadData::Go(void * data)
{
  if ( Active == this -> Running ) return false ;
  if ( nullptr != this -> PrivatePacket )                                 {
    PrivateThreadData * ptd = (PrivateThreadData *) this -> PrivatePacket ;
    ///////////////////////////////////////////////////////////////////////
    do                                                                    {
      if ( ! Run ( data ) ) StarDate::usleep ( 1500 )                     ;
    } while ( nullptr == ptd -> Thread )                                  ;
    ///////////////////////////////////////////////////////////////////////
  }                                                                       ;
  this -> Status = 0                                                      ;
  return true                                                             ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
