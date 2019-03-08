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
    std::string             className ;

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

    void         DebugReport   (void) ;

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

typedef struct  {
  HANDLE mutex  ;
  bool   locked ;
} MutexHandler  ;

class PrivateMutexz
{
  public:

    explicit PrivateMutexz (void) ;
    virtual ~PrivateMutexz (void) ;

    Mutex * GetMutex       (int64_t     index) ;
    Mutex * GetMutex       (std::string key  ) ;

    void    SpinLock       (void) ;

  protected:

    std::map<int64_t    ,Mutex *> mutex    ;
    std::map<std::string,Mutex *> sutex    ;
    bool                          spinLock ;

  private:

} ;

typedef struct            {
  PrivateMutexz * handler ;
} MutexzHandler           ;

typedef struct  {
  HANDLE semaphore  ;
  int    amount     ;
  int    acquired   ;
} SemaphoreHandler  ;

class PrivateSemaphorez
{
  public:

    explicit    PrivateSemaphorez (void) ;
    virtual    ~PrivateSemaphorez (void) ;

    Semaphore * GetSemaphore      (int64_t     index) ;
    Semaphore * GetSemaphore      (std::string key  ) ;

    void        SpinLock          (void) ;

  protected:

    std::map<int64_t    ,Semaphore *> mutex    ;
    std::map<std::string,Semaphore *> sutex    ;
    bool                              spinLock ;

  private:

} ;

typedef struct                {
  PrivateSemaphorez * handler ;
} SemaphorezHandler           ;

class PrivateSharedMemory
{
  public:

    int64_t Size     ;
    HANDLE  SHM      ;
    char  * Data     ;
    bool    attached ;
    bool    created  ;

    explicit PrivateSharedMemory (void) ;
    virtual ~PrivateSharedMemory (void) ;

    bool     create              (std::wstring key,int64_t Size) ;
    bool     open                (std::wstring key,int64_t Size) ;
    bool     close               (void) ;

  protected:

  private:

} ;

typedef struct                  {
  PrivateSharedMemory * handler ;
} SharedMemoryHandler           ;

class PrivateCpuUsage
{
  public:

    std::map<CPU::Features,std::string> FeatureNames ;

    FILETIME      m_ftPrevSysKernel  ;
    FILETIME      m_ftPrevSysUser    ;
    FILETIME      m_ftPrevSysIdle    ;
    FILETIME      m_ftPrevProcKernel ;
    FILETIME      m_ftPrevProcUser   ;
    short         m_nCpuUsage        ;
    ULONGLONG     m_dwLastRun        ;
    volatile LONG m_lRunCount        ;

    explicit        PrivateCpuUsage  (void) ;
    virtual        ~PrivateCpuUsage  (void) ;

    int             Processors       (void) ;
    const char    * GetCpuType       (void) ;
    int             CacheLineSize    (void) ;
    bool            FeatureExists    (CPU::Features feature) ;
    bool            HasFeature       (CPU::Features feature) ;
    uint32_t        GetFeatures      (void) ;
    int64_t         Memory           (void) ;
    CPU::PowerState Power            (int & seconds,int & percent) ;
    short           GetUsage         (bool cpu = true) ;
    bool            Tell             (ProcessDetails * process) ;

    ULONGLONG       SubtractTimes    (const FILETIME & ftA, const FILETIME & ftB);
    bool            EnoughTimePassed (void);
    inline bool     IsFirstRun       (void) const { return (m_dwLastRun == 0); }

  protected:

  private:

} ;

typedef struct              {
  PrivateCpuUsage * handler ;
} CpuUsageHandler           ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
