#include "PrivateParallel.hpp"

#pragma pack(push,1)

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

DWORD WindowsThreadId(void)
{
  return ::GetCurrentThreadId ( ) ;
}

ThreadHandler:: ThreadHandler (void)
{
}

ThreadHandler::~ThreadHandler (void)
{
}

void ThreadHandler::setKeys(std::string t,std::string d)
{
  this -> ThreadKey = t ;
  this -> DataKey   = d ;
}

size_t ThreadHandler::Threads(void)
{
  size_t cnt = this -> All . size ( )     ;
  if ( this -> Data . isAlive ( ) ) cnt++ ;
  return cnt                              ;
}

int32_t ThreadHandler::LastestId(void)
{
  int32_t Id = Data . Id                                    ;
  int32_t v                                                 ;
  std::list<ThreadData *>::iterator it                      ;
  for ( it = All . begin ( ) ; it != All . end ( ) ; ++it ) {
    v = (*it) -> Id                                         ;
    if ( v > Id ) Id = v                                    ;
  }                                                         ;
  return Id                                                 ;
}

ThreadData * ThreadHandler::at(int index)
{
  if ( index <  0              ) return & ( this -> Data ) ;
  if ( index >= All . size ( ) ) return nullptr            ;
  std::list<ThreadData *>::iterator it                     ;
  it = All . begin ( )                                     ;
  if ( index == 0 ) return (*it)                           ;
  it = std::next ( All . begin ( ) , index - 1 )           ;
  if ( it == All . end ( ) ) return nullptr                ;
  return (*it)                                             ;
}

ThreadData * ThreadHandler::current(void)
{
  if ( Data . isSelf ( ) ) return ( & Data )                ;
  std::list<ThreadData *>::iterator it                      ;
  for ( it = All . begin ( ) ; it != All . end ( ) ; ++it ) {
    if ( (*it) -> isSelf ( ) ) return (*it)                 ;
  }                                                         ;
  return nullptr                                            ;
}

ThreadData * ThreadHandler::find(int Id)
{
  if ( Data . isEqual ( Id ) ) return ( & Data )            ;
  std::list<ThreadData *>::iterator it                      ;
  for ( it = All . begin ( ) ; it != All . end ( ) ; ++it ) {
    if ( (*it) -> isEqual ( Id ) ) return (*it)             ;
  }                                                         ;
  return nullptr                                            ;
}

size_t ThreadHandler::append(ThreadData * thread)
{
  if ( nullptr != thread )              {
    if ( DataKey . length ( ) > 0 )     {
      Convoy::join ( DataKey , thread ) ;
    }                                   ;
    this -> All . push_back ( thread )  ;
  }                                     ;
  return this -> Threads ( )            ;
}

size_t ThreadHandler::remove(ThreadData * thread)
{
  if ( nullptr != thread )          {
    this -> All . remove ( thread ) ;
  }                                 ;
  return this -> Threads ( )        ;
}

size_t ThreadHandler::GetStatus            (
         std::list<ThreadData *> & threads ,
         ThreadData::ThreadState   state   )
{
  std::list<ThreadData *>::iterator it                             ;
  for ( it = All . begin ( ) ; it != All . end ( ) ; ++it )        {
    if ( state == ( (ThreadData::ThreadState) (*it) -> Running ) ) {
      threads . push_back ( *it )                                  ;
    }                                                              ;
  }                                                                ;
  return threads . size ( )                                        ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
