#include "PrivateParallel.hpp"

#pragma pack(push,1)


#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

unsigned int SingleThread(void * arg)
{
/*
  Thread     * nthread  = (Thread     *) arg                   ;
  ThreadData * data     = (ThreadData *) & ( nthread -> Data ) ;
  ::srand                 ( ::time ( NULL ) )                  ;
  ::srand                 ( ::rand (      ) )                  ;
  nthread -> Data . ThreadID = ThreadData::ThreadId ( )        ;
  nthread -> running  = ThreadData::Active                     ;
  nthread -> Data . Start (                 )                  ;
  nthread -> actualRun    (                 )                  ;
  nthread -> Data . Stop  (                 )                  ;
  nthread -> running  = ThreadData::Deactive                   ;
  ::_endthreadex          ( 0               )                  ;
*/
  return 0                                                     ;
}

unsigned int MultiThread(void * arg)
{
/*
  ThreadData * data    = (ThreadData *) arg         ;
  Thread     * nthread = (Thread     *) data->Extra ;
  ::srand              ( ::time ( NULL )     )      ;
  ::srand              ( ::rand (      )     )      ;
  data    -> ThreadID = ThreadData::ThreadId ( )    ;
  data    -> Start     (                     )      ;
  nthread -> actualRun ( data -> Type , data )      ;
  data    -> Stop      (                     )      ;
  ::_endthreadex       ( 0                   )      ;
*/
  return 0                                          ;
}

Thread:: Thread(void)
       : running     ( 0               )
       , isContinue  ( true            )
       , StackSize   ( THREADSTACKSIZE )
       , Reservation ( false           )
{
}

Thread:: Thread      ( int stackSize , bool reservation )
       : running     ( 0                                )
       , StackSize   ( stackSize                        )
       , isContinue  ( true                             )
       , Reservation ( reservation                      )
{
//  Data . Properties [ "Class" ] = QVariant(QString("N::Thread")) ;
//  tsLocker = new QMutex ( )                                      ;
}

Thread::~Thread(void)
{
}

bool Thread::Recycling(void)
{
  return true ;
}

bool Thread::Destructor(void)
{
  return true ;
/*
#ifdef TRACECLASSES
QString cns = Data.Properties["Class"].toString()           ;
N::printf(QString("%1(%2) => %3").arg(FunctionString).arg(cns).arg(running),true,true) ;
if ( AllThreads . count ( ) > 0 )                           {
  for (int i=0;i<AllThreads.count();i++)                    {
    ThreadData * d = AllThreads[i]                          ;
    N::printf(QString("%1 => %2(%3)").arg(d->Id).arg(d->Type).arg(d->Running),true,true) ;
  }                                                         ;
}                                                           ;
N::printf ( QString("%1 finalize").arg(cns) , true , true ) ;
#endif
finalize ( 5 )  ;
delete tsLocker ;
#ifdef TRACECLASSES
N::printf(QString("%1 stopped").arg(cns),true,true)         ;
#endif
*/
}

void * Thread::Register(void *)
{
  return nullptr ;
}

int Thread::setPriority(int priority)
{
  return 0 ;
//  return Data . setPriority ( priority ) ;
}

void Thread::start(void)
{
/*
    nDropOut ( running == 1 )                 ;
    ///////////////////////////////////////////
    while (!recycle()) N::Time::skip(100)     ;
    ///////////////////////////////////////////
    Data . Running     = 0                    ;
    Data . Extra       = this                 ;
    Data . Function    = Thread::SingleThread ;
    Data . StackSize   = StackSize            ;
    Data . Reservation = Reservation          ;
    ///////////////////////////////////////////
    Data . Go ( this )                        ;
*/
}

ThreadData * Thread::start(int Type)
{
  return nullptr ;
}

//    ThreadData  * start       (int Type,VarArgs      & arguments) ;
//    ThreadData  * start       (int Type,XmlValue     & arguments) ;
//    ThreadData  * start       (int Type,ThreadFunction ExternalThread) ;

ThreadData * Thread::drawer(void)
{
//  if ( Data . isSelf ( ) ) return &Data                                     ;
/*
  ThreadData * ti = NULL                                                    ;
  tsLocker -> lock   ( )                                                    ;
  for (int i = 0 ; ( NULL == ti ) && ( i < AllThreads . count ( ) ) ; i++ ) {
    if ( AllThreads [ i ] -> isSelf ( ) )                                   {
      ti = AllThreads [ i ]                                                 ;
    }                                                                       ;
  }                                                                         ;
  tsLocker -> unlock ( )                                                    ;
  return ti                                                                 ;
*/
  return nullptr ;
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
}

void Thread::resume(void)
{
}

void Thread::terminate(void)
{
}

void Thread::cleanup(void)
{
/*
    QList<N::ThreadData *> Dead                         ;
    tsLocker -> lock   ( )                              ;
    for (int i=0;i<AllThreads.count();i++)              {
      if (AllThreads[i]->Running==ThreadData::Deactive) {
        Dead << AllThreads[i]                           ;
      }                                                 ;
    }                                                   ;
    tsLocker -> unlock ( )                              ;
    if (Dead.count()<=0) return                         ;
    for (int i=0;i<Dead.count();i++)                    {
      N::ThreadData * data = Dead[i]                    ;
      int           index                               ;
      data -> Join ( )                                  ;
      tsLocker -> lock   ( )                            ;
      index = AllThreads.indexOf(data)                  ;
      if (index>=0)                                     {
        AllThreads . takeAt ( index )                   ;
        delete data                                     ;
      }                                                 ;
      tsLocker -> unlock ( )                            ;
    }                                                   ;
*/
}

bool Thread::proceed(void)
{
  return true ;
}

bool Thread::finalize(int interval)
{
  isContinue = false                             ;
/*
  while ( ( ThreadData::Idle != running )       ||
          ( AllThreads . count ( ) > 0  )      ) {
    //////////////////////////////////////////////
    switch ( running )                           {
      case ThreadData::Idle                      :
      case ThreadData::Active                    :
      break                                      ;
      case ThreadData::Deactive                  :
        Data . Join ( )                          ;
        running = ThreadData::Recycle            ;
      break                                      ;
      case ThreadData::Recycle                   :
        running = ThreadData::Idle               ;
      break                                      ;
    }                                            ;
    //////////////////////////////////////////////
    if ( AllThreads . count ( ) > 0 )            {
      ThreadData * data = NULL                   ;
      tsLocker -> lock   ( )                     ;
      data = AllThreads . last ( )               ;
      tsLocker -> unlock ( )                     ;
      if ( NotNull ( data ) )                    {
        int tid                                  ;
        switch ( data -> Running )               {
          case ThreadData::Idle                  :
          case ThreadData::Active                :
          break                                  ;
          case ThreadData::Deactive              :
            data -> Join ( )                     ;
          break                                  ;
          case ThreadData::Recycle               :
            tsLocker -> lock   ( )               ;
            tid = AllThreads . indexOf ( data )  ;
            if ( tid >= 0 )                      {
              AllThreads . takeAt ( tid )        ;
            }                                    ;
            tsLocker -> unlock ( )               ;
            delete data                          ;
          break                                  ;
        }                                        ;
      } else                                     {
        tsLocker -> lock   ( )                   ;
        AllThreads . takeFirst ( )               ;
        tsLocker -> unlock ( )                   ;
      }                                          ;
    }                                            ;
    //////////////////////////////////////////////
    Time :: msleep ( interval )                  ;
    ThreadEvent    (          )                  ;
  }                                              ;
*/
  return true                                    ;
}

bool Thread::NotStopped(void)
{
  return false ;
/*
  nKickOut ( ! isContinue              , false ) ;
  nKickOut ( NULL == Data . Controller , true  ) ;
  return * ( Data . Controller                 ) ;
*/
}

bool Thread::IsContinue(void)
{
  if ( ! isContinue                   ) return false ;
//  if ( NULL != Data . Controller )                   {
//    if ( ! ( *( Data . Controller ) ) ) return false ;
//  }                                                  ;
  ////////////////////////////////////////////////////
  ThreadData * d = drawer ( )                        ;
  if ( nullptr == d      ) return false              ;
  if ( ! d -> isContinue ) return false              ;
  if ( nullptr != d -> Controller )                  {
    if ( ! ( *( d -> Controller   ) ) ) return false ;
  }                                                  ;
  return true                                        ;
}

bool Thread::IsContinue(bool go)
{
  if ( ! go ) return false ;
  return IsContinue ( )    ;
}

bool Thread::IsContinue(int Id)
{
  if ( ! isContinue                   ) return false     ;
/*
  if ( NULL != Data . Controller )                       {
    if ( ! ( *( Data . Controller ) ) ) return false     ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  for (int i = 0 ; i < AllThreads . count ( ) ; i++ )    {
    if ( Id == AllThreads [ i ] -> Id )                  {
      ThreadData * d = AllThreads [ i ]                  ;
      if ( ! d -> isContinue ) return false              ;
      if ( NULL != d -> Controller )                     {
        if ( ! ( *( d -> Controller   ) ) ) return false ;
      }                                                  ;
      return true                                        ;
    }                                                    ;
  }                                                      ;
  ////////////////////////////////////////////////////////
  return ( Id == Data . Id )                             ;
*/
  return false ;
}

bool Thread::IsContinue(ThreadData * d)
{
  if ( ! isContinue      ) return false              ;
  if ( nullptr == d      ) return false              ;
  if ( ! d -> isContinue ) return false              ;
  ////////////////////////////////////////////////////
  if ( nullptr != d -> Controller )                  {
    if ( ! ( *( d -> Controller   ) ) ) return false ;
  }                                                  ;
  ////////////////////////////////////////////////////
//  if ( nullptr != Data . Controller )                {
//    if ( ! ( *( Data . Controller ) ) ) return false ;
//  }                                                  ;
  ////////////////////////////////////////////////////
  return true                                        ;
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

//    int           IsRunning   (QList<ThreadData *> & threads,bool purge = false) ;
int Thread::exit(int exitcode)
{
  ::ExitThread ( exitcode ) ;
  return exitcode           ;
}

void Thread::actualRun(void)
{
  run ( ) ;
}

void Thread::actualRun(int Type,ThreadData * data)
{
  run ( Type , data ) ;
}

void Thread::actualJoin(void)
{
}

void Thread::ThreadEvent(void)
{
}

void Thread::run(void)
{
}

void Thread::run(int Type,ThreadData * Data)
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
  if     ( ThreadData::Idle     == running ) return true                ;
  if     ( ThreadData::Active   == running ) return false               ;
  if     ( ThreadData::Deactive == running ) actualJoin ( )             ;
  if     ( ThreadData::Recycle  == running ) running = ThreadData::Idle ;
  return ( ThreadData::Idle     == running )                            ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
