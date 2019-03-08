#include "PrivateParallel.hpp"

#pragma pack(push,1)

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

unsigned int SingleThread(void * arg)
{
  PrivateThreadData * ptd = (PrivateThreadData *) arg    ;
  if ( nullptr == ptd ) return 0                         ;
  ////////////////////////////////////////////////////////
  ThreadData * data    = ptd -> threadData               ;
  Thread     * nthread = ptd -> thread                   ;
  ////////////////////////////////////////////////////////
  ::srand              ( (uint32_t) ::time ( nullptr ) ) ;
  ::srand              (            ::rand (         ) ) ;
  ////////////////////////////////////////////////////////
  data    -> AssignId  (                               ) ;
  nthread -> running  = ThreadData::Active               ;
  data    -> Start     (                               ) ;
  nthread -> actualRun (                               ) ;
  data    -> Stop      (                               ) ;
  nthread -> running  = ThreadData::Deactive             ;
  ////////////////////////////////////////////////////////
  ::_endthreadex       ( 0                             ) ;
  ////////////////////////////////////////////////////////
  return 0                                               ;
}

unsigned int MultiThread(void * arg)
{
       printf("MultiThread\n") ;
  PrivateThreadData * ptd = (PrivateThreadData *) arg    ;
  if ( nullptr == ptd ) return 0                         ;
  ////////////////////////////////////////////////////////
  ThreadData * data    = ptd -> threadData               ;
  Thread     * nthread = ptd -> thread                   ;
  ////////////////////////////////////////////////////////
  ::srand              ( (uint32_t) ::time ( nullptr ) ) ;
  ::srand              (            ::rand (         ) ) ;
  ////////////////////////////////////////////////////////
      printf("AssignId\n") ;
  data    -> AssignId  (                               ) ;
      printf("Start\n") ;
  data    -> Start     (                               ) ;
      printf("actualRun\n") ;
  nthread -> actualRun ( data -> Type , data           ) ;
      printf("Stop\n") ;
  data    -> Stop      (                               ) ;
//      printf("cleanup\n") ;
//  nthread -> cleanup   (                               ) ;
  ////////////////////////////////////////////////////////
  ::_endthreadex       ( 0                             ) ;
  ////////////////////////////////////////////////////////
  return 0                                               ;
}

//////////////////////////////////////////////////////////////////////////////

Thread:: Thread        ( void            )
       : Destroyer     (                 )
       , running       ( 0               )
       , StackSize     ( THREADSTACKSIZE )
       , Reservation   ( false           )
       , isContinue    ( true            )
       , Controller    ( nullptr         )
       , PrivatePacket ( nullptr         )
       , lock          ( new Locker ( )  )
{
      printf("Thread::Destroyer\n") ;
  PrivateHandler * ph = new PrivateHandler ( )  ;
  ph   -> handler = new ThreadHandler ( )       ;
  this -> PrivatePacket = (void *) ph           ;
  ph   -> handler -> className = "CIOS::Thread" ;
}

Thread:: Thread        ( int stackSize , bool reservation )
       : Destroyer     (                                  )
       , running       ( 0                                )
       , StackSize     ( stackSize                        )
       , Reservation   ( reservation                      )
       , isContinue    ( true                             )
       , Controller    ( nullptr                          )
       , PrivatePacket ( nullptr                          )
       , lock          ( new Locker ( )                   )
{
    printf("Thread::Destroyer\n") ;
  PrivateHandler * ph = new PrivateHandler ( )  ;
  ph   -> handler = new ThreadHandler ( )       ;
  this -> PrivatePacket = (void *) ph           ;
  ph   -> handler -> className = "CIOS::Thread" ;
}

Thread::~Thread(void)
{
}

bool Thread::Interrupt(void)
{
  this -> isContinue = false ;
  return true                ;
}

bool Thread::Recycling(void)
{
  return true ;
}

bool Thread::Destructor(void)
{
  ////////////////////////////////////////////////////////////////////////////
  if ( nullptr == this -> PrivatePacket ) return false                       ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket            ;
  ////////////////////////////////////////////////////////////////////////////
  #ifdef CIOSDEBUG
  ph -> handler -> DebugReport ( )                                           ;
  #endif
  ////////////////////////////////////////////////////////////////////////////
  finalize ( 5 )                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( nullptr != lock ) delete lock                                         ;
  lock = nullptr                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  #ifdef CIOSDEBUG
  if ( ph -> handler -> className . length ( ) > 0 )                         {
    CiosDebugger ( "Thread `%s` stopped\n"                                   ,
                   ph   -> handler -> className . c_str ( )                ) ;
  }                                                                          ;
  #endif
  ////////////////////////////////////////////////////////////////////////////
  if ( ph -> handler -> ThreadKey . length ( ) > 0 )                         {
    Convoy::remove ( ph -> handler -> ThreadKey , this )                     ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  delete ph -> handler                                                       ;
  delete ph                                                                  ;
  this -> PrivatePacket = nullptr                                            ;
  ////////////////////////////////////////////////////////////////////////////
     printf("%s\n",__FUNCTION__) ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

void * Thread::Register(void *)
{
    printf("%s\n",__FUNCTION__) ;
  return nullptr ;
}

void Thread::LockThread(void)
{
  if ( nullptr == lock ) return ;
  lock -> lock ( )              ;
}

void Thread::UnlockThread (void)
{
  if ( nullptr == lock ) return ;
  lock -> unlock ( )            ;
}

void Thread::sleep(int64_t seconds)
{
  StarDate::sleep ( seconds ) ;
}

void Thread::msleep(int64_t mseconds)
{
  StarDate::msleep ( mseconds ) ;
}

void Thread::usleep(int64_t useconds)
{
  StarDate::usleep ( useconds ) ;
}

void Thread::skip(int64_t  seconds,int64_t intervalus,bool * dropOut)
{
  StarDate::skip ( seconds , intervalus , dropOut ) ;
}

void Thread::mskip(int64_t mseconds,int64_t intervalus,bool * dropOut)
{
  StarDate::mskip ( mseconds , intervalus , dropOut ) ;
}

void Thread::uskip(int64_t useconds,int64_t intervalus,bool * dropOut)
{
  StarDate::uskip ( useconds , intervalus , dropOut ) ;
}

void Thread::setClassName(std::string name)
{
  if ( nullptr == this -> PrivatePacket ) return                  ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket ;
  ph -> handler -> className = name                               ;
}

void Thread::setKeys(std::string t,std::string d)
{
  if ( nullptr == this -> PrivatePacket ) return                  ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket ;
  ph -> handler -> setKeys ( t , d )                              ;
  Convoy::join ( t , this )                                       ;
}

int Thread::setPriority(int priority)
{
  ThreadData * d = drawer ( )          ;
  if ( nullptr == d ) return 0         ;
  return d -> setPriority ( priority ) ;
}

void Thread::start(void)
{
  if ( ThreadData::Active == running ) return                                ;
  ////////////////////////////////////////////////////////////////////////////
  while ( IsContinue ( ) && ( ! recycle ( ) ) )                              {
    StarDate::mskip(100)                                                     ;
  }                                                                          ;
  if ( ! IsContinue ( ) ) return                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( nullptr == lock                  ) return                             ;
  if ( nullptr == this -> PrivatePacket ) return                             ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket            ;
  ////////////////////////////////////////////////////////////////////////////
  ThreadData        * data = ph -> handler -> at ( -1 )                      ;
  PrivateThreadData * ptd  = (PrivateThreadData *) data -> Hidden ( )        ;
  ////////////////////////////////////////////////////////////////////////////
  this -> LockThread   ( )                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  #ifndef DONT_USE_NAMESPACE
  ptd  -> Function     = PARALLEL_NAMESPACE :: SingleThread                  ;
  #else
  ptd  -> Function     = ::SingleThread                                      ;
  #endif
  ////////////////////////////////////////////////////////////////////////////
  ptd  -> thread       = this                                                ;
  ptd  -> threadData   = data                                                ;
  ////////////////////////////////////////////////////////////////////////////
  data -> Running      = ThreadData::Idle                                    ;
  data -> Type         = 0                                                   ;
  data -> Extra        = this                                                ;
  data -> StackSize    = StackSize                                           ;
  data -> Reservation  = Reservation                                         ;
  data -> Arguments    = nullptr                                             ;
  data -> Controller   = this -> Controller                                  ;
  data -> Id = ph -> handler -> LastestId ( ) + 1                            ;
  ////////////////////////////////////////////////////////////////////////////
  data -> Go ( data )                                                        ;
  ////////////////////////////////////////////////////////////////////////////
  this -> UnlockThread ( )                                                   ;
  ////////////////////////////////////////////////////////////////////////////
}

ThreadData * Thread::start(int Type)
{
  return this -> start ( Type , nullptr ) ;
}

ThreadData * Thread::start(int Type,void * arguments)
{
    printf("%s\n",__FUNCTION__) ;
  cleanup ( )                                                                ;
    printf("cleanup() ;\n") ;
  ////////////////////////////////////////////////////////////////////////////
  if ( nullptr == lock                  ) return nullptr                     ;
  if ( nullptr == this -> PrivatePacket ) return nullptr                     ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket            ;
  ////////////////////////////////////////////////////////////////////////////
  ThreadData        * data = new ThreadData ( )                              ;
  PrivateThreadData * ptd  = (PrivateThreadData *) data -> Hidden ( )        ;
  ////////////////////////////////////////////////////////////////////////////
      printf("this -> LockThread\n") ;
  this -> LockThread   ( )                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  #ifndef DONT_USE_NAMESPACE
  ptd  -> Function     = PARALLEL_NAMESPACE :: MultiThread                   ;
  #else
  ptd  -> Function     = ::MultiThread                                       ;
  #endif
  ////////////////////////////////////////////////////////////////////////////
  ptd  -> thread       = this                                                ;
  ptd  -> threadData   = data                                                ;
  ////////////////////////////////////////////////////////////////////////////
  data -> Running      = ThreadData::Idle                                    ;
  data -> Type         = Type                                                ;
  data -> Extra        = this                                                ;
  data -> StackSize    = StackSize                                           ;
  data -> Reservation  = Reservation                                         ;
  data -> Arguments    = arguments                                           ;
  data -> Controller   = this -> Controller                                  ;
  data -> Id = ph -> handler -> LastestId ( ) + 1                            ;
  ////////////////////////////////////////////////////////////////////////////
      printf("data -> Go ( data )\n") ;
  if ( data -> Go ( data ) )                                                 {
    ph -> handler -> append ( data )                                         ;
    printf("ph -> handler -> append\n") ;
  } else                                                                     {
    delete data                                                              ;
    data = nullptr                                                           ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  this -> UnlockThread ( )                                                   ;
    printf("this -> UnlockThread\n") ;
  ////////////////////////////////////////////////////////////////////////////
  return data                                                                ;
}

//    ThreadData  * start       (int Type,VarArgs      & arguments) ;
//    ThreadData  * start       (int Type,XmlValue     & arguments) ;
//    ThreadData  * start       (int Type,ThreadFunction ExternalThread) ;

ThreadData * Thread::drawer(void)
{
  Latcher latch ( lock )                                          ;
  if ( nullptr == this -> PrivatePacket ) return nullptr          ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket ;
  return ph -> handler -> current ( )                             ;
}

ThreadData * Thread::drawer(int Id)
{
  Latcher latch ( lock )                                          ;
  if ( nullptr == this -> PrivatePacket ) return nullptr          ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket ;
  return ph -> handler -> find ( Id )                             ;
}

int Thread::DrawerId(void)
{
  ThreadData * td = this -> drawer ( ) ;
  if ( nullptr == td ) return -1       ;
  return td -> Id                      ;
}

void Thread::quit(void)
{
}

void Thread::suspend(void)
{
  ThreadData * d = this -> drawer ( ) ;
  if ( nullptr == d ) return          ;
  d -> Suspend ( )                    ;
}

void Thread::resume(void)
{
  ThreadData * d = this -> drawer ( ) ;
  if ( nullptr == d ) return          ;
  d -> Resume ( )                     ;
}

void Thread::terminate(void)
{
  ThreadData * d = this -> drawer ( ) ;
  if ( nullptr == d ) return          ;
  d -> TryCancel ( )                  ;
}

void Thread::cleanup(void)
{
  Latcher latch ( lock )                                              ;
  if ( nullptr == this -> PrivatePacket ) return                      ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket     ;
  std::list<ThreadData *> threads                                     ;
  size_t                  cnt                                         ;
  /////////////////////////////////////////////////////////////////////
  cnt = ph -> handler -> GetStatus ( threads , ThreadData::Deactive ) ;
      printf("Remain threads : %d\n",(int)cnt) ;
  /////////////////////////////////////////////////////////////////////
  if ( cnt <= 0 ) return                                              ;
  /////////////////////////////////////////////////////////////////////
  std::list<ThreadData *>::iterator it                                ;
  for ( it = threads . begin ( ) ; it != threads . end ( ) ; ++it )   {
        printf("Join thread : %d\n",(int)(*it)->Id) ;
    (*it) -> Join ( )                                                 ;
       printf("Remove thread : %d\n",(int)(*it)->Id) ;
    ph -> handler -> remove ( (*it) )                                 ;
    delete (*it)                                                      ;
  }                                                                   ;
  /////////////////////////////////////////////////////////////////////
}

bool Thread::proceed(void)
{
  return true ;
}

bool Thread::finalize(int interval)
{
  if ( nullptr == this -> PrivatePacket ) return true                        ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket            ;
  ThreadData     * dd                                                        ;
  size_t           cnt                                                       ;
  ////////////////////////////////////////////////////////////////////////////
  isContinue = false                                                         ;
  while ( ( ThreadData::Idle != running      )                              ||
          ( ph -> handler -> Threads ( ) > 0 )                             ) {
    //////////////////////////////////////////////////////////////////////////
    dd = ph -> handler -> at ( -1 )                                          ;
    switch ( running )                                                       {
      case ThreadData::Idle                                                  :
      case ThreadData::Active                                                :
      break                                                                  ;
      case ThreadData::Deactive                                              :
        dd -> Join ( )                                                       ;
        running = ThreadData::Recycle                                        ;
      break                                                                  ;
      case ThreadData::Recycle                                               :
        running = ThreadData::Idle                                           ;
      break                                                                  ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    LockThread   ( )                                                         ;
    cnt = ph -> handler -> Threads (   )                                     ;
    dd  = ph -> handler -> at      ( 0 )                                     ;
    UnlockThread ( )                                                         ;
    //////////////////////////////////////////////////////////////////////////
    if ( ( cnt > 0 ) && ( nullptr != dd ) )                                  {
      switch ( dd -> Running )                                               {
        case ThreadData::Idle                                                :
        case ThreadData::Active                                              :
        break                                                                ;
        case ThreadData::Deactive                                            :
          dd -> Join ( )                                                     ;
        break                                                                ;
        case ThreadData::Recycle                                             :
          LockThread   ( )                                                   ;
          ph -> handler -> remove ( dd )                                     ;
          delete dd                                                          ;
          UnlockThread ( )                                                   ;
        break                                                                ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    StarDate::msleep ( interval )                                            ;
    ThreadEvent      (          )                                            ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

bool Thread::NotStopped(void)
{
  if ( ! isContinue                  ) return false ;
  if ( nullptr == this -> Controller ) return true  ;
  return ( * ( this -> Controller ) )               ;
}

bool Thread::IsContinue(void)
{
  if ( ! isContinue                      ) return false ;
  if ( nullptr != this -> Controller )                  {
    if ( !  ( * ( this -> Controller ) ) ) return false ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  ThreadData * d = drawer ( )                           ;
  if ( nullptr == d ) return false                      ;
  return d -> canContinue ( )                           ;
}

bool Thread::IsContinue(bool go)
{
  if ( ! go ) return false ;
  return IsContinue ( )    ;
}

bool Thread::IsContinue(int Id)
{
  if ( ! isContinue                      ) return false ;
  if ( nullptr != this -> Controller )                  {
    if ( ! ( *  ( this -> Controller ) ) ) return false ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  ThreadData * d = this -> drawer ( Id )                ;
  if ( nullptr == d                      ) return false ;
  return d -> canContinue ( )                           ;
}

bool Thread::IsContinue(ThreadData * d)
{
  if ( ! isContinue                      ) return false ;
  if ( nullptr == d                      ) return false ;
  if ( ! d -> canContinue ( )            ) return false ;
  ///////////////////////////////////////////////////////
  if ( nullptr != this -> Controller )                  {
    if ( ! ( *  ( this -> Controller ) ) ) return false ;
  }                                                     ;
  ///////////////////////////////////////////////////////
  return true                                           ;
}

bool Thread::IsContinue(bool go,ThreadData * data)
{
  if ( ! go ) return false   ;
  return IsContinue ( data ) ;
}

bool Thread::IsContinue(int action,ThreadData * data)
{
  return IsContinue ( data ) ;
}

int Thread::IsRunning(std::list<ThreadData *> & threads,bool purge)
{
  if ( threads . size ( ) <= 0 ) return 0                            ;
  if ( nullptr == this -> PrivatePacket ) return 0                   ;
  PrivateHandler * ph = ( PrivateHandler *) this -> PrivatePacket    ;
  ////////////////////////////////////////////////////////////////////
  int                               count = 0                        ;
  std::list<ThreadData *>           stopped                          ;
  std::list<ThreadData *>::iterator it                               ;
  ThreadData                      * td                               ;
  int                               R                                ;
  for ( it = threads . begin ( ) ; it != threads . end ( ) ; ++it )  {
    td = (*it)                                                       ;
    R  = td -> Running                                               ;
    switch ( R )                                                     {
      case ThreadData::Idle                                          :
      case ThreadData::Recycle                                       :
      break                                                          ;
      case ThreadData::Active                                        :
        count ++                                                     ;
      break                                                          ;
      case ThreadData::Deactive                                      :
        stopped . push_back ( td )                                   ;
      break                                                          ;
    }                                                                ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  if ( ! purge ) return count                                        ;
  ////////////////////////////////////////////////////////////////////
  for ( it = stopped . begin ( ) ; it != stopped . end ( ) ; ++it )  {
    ph -> handler -> remove ( (*it) )                                ;
  }                                                                  ;
  ////////////////////////////////////////////////////////////////////
  return count                                                       ;
}

int64_t Thread::Elapsed(void)
{
  ThreadData * d = this -> drawer ( ) ;
  if ( nullptr == d ) return false    ;
  return d -> Elapsed ( )             ;
}

int64_t Thread::Elapsed(int Id)
{
  ThreadData * d = this -> drawer ( Id ) ;
  if ( nullptr == d ) return false       ;
  return d -> Elapsed ( )                ;
}

int Thread::exit(int exitcode)
{
  ::ExitThread ( (DWORD) exitcode ) ;
  return exitcode                   ;
}

void Thread::actualRun(void)
{
  run ( ) ;
}

void Thread::actualRun(int Type,ThreadData * data)
{
      printf("actualRun : %d\n",Type) ;
  run ( Type , data ) ;
}

void Thread::actualJoin(void)
{
  ThreadData * d = drawer ( -1 ) ;
  if ( nullptr == d ) return     ;
  d -> Join ( )                  ;
  running = ThreadData::Recycle  ;
}

void Thread::ThreadEvent(void)
{
}

void Thread::run(void)
{
}

void Thread::run(int,ThreadData *)
{
}

bool Thread::detection(void)
{
  if ( finalize ( 5 )              ) return false ;
  if ( ThreadData::Idle == running ) return false ;
  return ( ThreadData::Active == running )        ;
}

bool Thread::recycle(void)
{
  ThreadData * d = drawer ( -1 )                                        ;
  if ( nullptr == d ) return true                                       ;
  this -> running = d -> Running                                        ;
  if     ( ThreadData::Idle     == running ) return true                ;
  if     ( ThreadData::Active   == running ) return false               ;
  if     ( ThreadData::Deactive == running ) actualJoin ( )             ;
  if     ( ThreadData::Recycle  == running ) running = ThreadData::Idle ;
  return ( ThreadData::Idle     == running )                            ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
