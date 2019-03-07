/****************************************************************************\
 *
 *                         CIOS Parallel System
 *
\****************************************************************************/

#ifndef PRIVATE_PARALLEL_HPP
#define PRIVATE_PARALLEL_HPP

#include "parallel.hpp"

#define WIN32_LEAN_AND_MEAN
#include <io.h>
#include <direct.h>
#include <process.h>
#include <Windows.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <list>
#include <map>

#if !defined(PARALLEL_NAMESPACE)
#define PARALLEL_NAMESPACE CIOS
#endif

// Debug
#ifdef CIOSDEBUG

#endif

#pragma pack(push,1)

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

#pragma pack(push,1)

#pragma pack(pop)

typedef struct                 {
  CRITICAL_SECTION mutex       ;
  bool             locked      ;
  bool             initialized ;
  int              type        ;
} LockerStructure              ;

#define THREADSTACKSIZE ( 16 * 1024 * 1024 )
typedef unsigned int (*ThreadFunction)(LPVOID arg) ;

typedef struct                  {
  HANDLE             Thread     ;
  DWORD              ThreadID   ;
  DWORD              ParentID   ;
  unsigned int       dwThreadID ;
  ThreadFunction     Function   ;
  void             * Data       ;
  class Thread     * thread     ;
  class ThreadData * threadData ;
  std::string        Key        ;
} PrivateThreadData             ;

extern DWORD WindowsThreadId(void) ;

class ThreadHandler
{
  public:

    ThreadData              Data      ;
    std::list<ThreadData *> All       ;
    std::string             ThreadKey ;
    std::string             DataKey   ;

    explicit     ThreadHandler (void) ;
    virtual     ~ThreadHandler (void) ;

    size_t       Threads       (void) ;
    int32_t      LastestId     (void) ;

    void         setKeys       (std::string thread,std::string data) ;

    ThreadData * at            (int index) ;
    ThreadData * current       (void) ;
    ThreadData * find          (int Id) ;
    size_t       remove        (ThreadData * thread) ;

    size_t       append        (ThreadData * thread) ;

    //    int           IsRunning   (QList<ThreadData *> & threads,bool purge = false) ;

    size_t       GetStatus     (std::list<ThreadData *> & threads ,
                                ThreadData::ThreadState   state ) ;

  protected:

  private:

} ;

typedef struct            {
  ThreadHandler * handler ;
} PrivateHandler          ;

class PrivateLockerz
{
  public:

    explicit PrivateLockerz (void) ;
    virtual ~PrivateLockerz (void) ;

    Locker * GetLocker      (int64_t index) ;
    Locker * GetLocker      (std::string key) ;

    void     SpinLock       (void) ;

  protected:

    std::map<int64_t    ,Locker *> mutex    ;
    std::map<std::string,Locker *> sutex    ;
    bool                           spinLock ;

  private:

} ;

typedef struct             {
  PrivateLockerz * handler ;
} LockerzHandler           ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
