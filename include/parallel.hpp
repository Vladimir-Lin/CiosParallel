/****************************************************************************\
 *
 *                         CIOS Parallel System
 *
\****************************************************************************/

#ifndef CIOS_PARALLEL_HPP
#define CIOS_PARALLEL_HPP

// cmake ... -DBUILD_SHARED_LIBS=1 ... => compile shared library
#if defined (WIN32) && defined(BUILD_SHARED_LIBS)
  #if defined (_MSC_VER)
    #pragma warning(disable: 4251)
    #pragma warning(disable: 4819)
  #endif
  #if defined(EXPORT_LIBPARALLEL)
    #define LIBPARALLEL_EXPORT __declspec(dllexport)
  #else
    #define LIBPARALLEL_EXPORT __declspec(dllimport)
  #endif
#else
    #define LIBPARALLEL_EXPORT
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <varargs.h>
#include <time.h>

#include <list>

#include "UUIDs.hpp"
#include "stardate.hpp"

#if !defined(PARALLEL_NAMESPACE)
#define PARALLEL_NAMESPACE CIOS
#endif

// Debug
#ifdef CIOSDEBUG

#endif

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

#pragma pack(push,1)

#pragma pack(pop)

typedef void (*DebugFunction)(const char * fmt,...) ;

extern LIBPARALLEL_EXPORT void setDebugFunction(DebugFunction debugger) ;
extern LIBPARALLEL_EXPORT DebugFunction CiosDebugger ;

// A light version of Mutex
class LIBPARALLEL_EXPORT Locker
{
  public:

    enum Types         {
      Default    = 0   ,
      Normal     = 1   ,
      ErrorCheck = 2   ,
      Recursive  = 3 } ;

    explicit Locker  (void) ;
    virtual ~Locker  (void) ;

    int      type    (void) const ;
    void     setType (Types MT) ;

    int      lock    (void) ;
    int      unlock  (void) ;
    int      locked  (void) ;
    int      trylock (void) ;

  protected:

    void * PrivatePacket ;

    void     release (void) ;

  private:

} ;

class LIBPARALLEL_EXPORT Lockerz
{
  public:

    explicit Lockerz  (void) ;
    virtual ~Lockerz  (void) ;

    Locker & operator [ ] (int64_t     index) ;
    Locker & operator [ ] (std::string key  ) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

class LIBPARALLEL_EXPORT Latcher
{
  public:

    explicit Latcher (Locker * locker) ;
    virtual ~Latcher (void) ;

    Locker * mutex   (void) const ;
    void     relock  (void) ;
    void     unlock  (void) ;

  protected:

  private:

    Locker * lock ;

} ;

class LIBPARALLEL_EXPORT Mutex
{
  public:

    explicit Mutex   (void) ;
    virtual ~Mutex   (void) ;

    int      lock    (void) ;
    int      unlock  (void) ;
    int      locked  (void) ;
    int      trylock (void) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

class LIBPARALLEL_EXPORT Mutexz
{
  public:

    explicit Mutexz (void) ;
    virtual ~Mutexz (void) ;

    Mutex & operator [ ] (int64_t     index) ;
    Mutex & operator [ ] (std::string key  ) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

class LIBPARALLEL_EXPORT Semaphore
{
  public:

    explicit Semaphore  (int amount = 1) ;
    virtual ~Semaphore  (void) ;

    int      setAmount  (int amount) ;
    void     acquire    (int n = 1) ;
    int      available  (void) const ;
    void     release    (int n = 1) ;
    bool     tryAcquire (int n = 1) ;
    bool     tryAcquire (int n, int timeoutMs) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

class LIBPARALLEL_EXPORT Semaphorez
{
  public:

    explicit Semaphorez (void) ;
    virtual ~Semaphorez (void) ;

    Semaphore & operator [ ] (int64_t     index) ;
    Semaphore & operator [ ] (std::string key  ) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

/*****************************************************************************
 *                                                                           *
 *                                 Shared Memory                             *
 *                                                                           *
 *****************************************************************************/

class LIBPARALLEL_EXPORT SharedMemory
{
  public:

    explicit SharedMemory (void) ;
    virtual ~SharedMemory (void) ;

    bool     Create       (std::string  key,int64_t Size) ;
    bool     Create       (std::wstring key,int64_t Size) ;
    bool     Open         (std::string  key,int64_t Size) ;
    bool     Open         (std::wstring key,int64_t Size) ;
    int64_t  MemorySize   (void) ;
    bool     Close        (void) ;
    char *   Memory       (void) ;
    bool     isAttached   (void) const ;
    bool     isCreated    (void) const ;

  protected :

    void * PrivatePacket ;

  private:

} ;

/****************************************************************************
 *                                                                          *
 *                                 Shared Memory                            *
 *                                                                          *
 ****************************************************************************/

#pragma pack(push,1)

typedef struct            { // Total 1024 bytes
  int32_t  Initialized    ; // Details was initialized
  int32_t  Processors     ; // Machine processors
  int64_t  Memory         ; // Total memory size
  int64_t  Cache          ; // Cache line size
  uint64_t Features       ; // CPU features
  char     Name   [ 224 ] ; // CPU name
  char     Buffer [ 768 ] ; // For internal use only
} CpuDetails              ;

typedef struct                 { // Total 1024 bytes
  int32_t  occupied            ; // This ProcessDetails space is occupied
  char     application [ 256 ] ; // Application name
  uint64_t instance            ; // Instance number
  int32_t  openmp              ; // OpenMP process number
  int64_t  peak                ; // Peak working memory size
  int64_t  working             ; // Working memory size
  int64_t  page                ; // Page memory file usage
  int32_t  machine             ; // Machine CPU usage, 0 ~ 10000
  int32_t  process             ; // Process CPU usage, 0 ~ 10000
  int64_t  timestamp           ; // RDTSC, Time Stamp Counter
  int64_t  registered          ; // Registered startup Time Stamp Counter
  int64_t  updated             ; // Lastest updated Time Stamp Counter
  int32_t  port                ; // Communication TCP port
  int32_t  interval            ; // Update interval
  int32_t  load                ; // A number between 0 ~ 100 specifies physical memory is in use
  int64_t  total               ; // total actual physical memory
  int64_t  available           ; // available physical memory
  int64_t  virtualTotal        ; // total actual virtual memory
  int64_t  virtualAvailable    ; // total available virtual memory
  int32_t  offset              ; // private data offset
  uint64_t flags               ; // Process Flags
  char     buffer      [ 640 ] ; // For internal use only
} ProcessDetails               ;

#define PROCESS_DETAILS_OFFSET 640

#pragma pack(pop)

namespace CPU
{

typedef enum              {
  _RDTSC     = 0x00000001 ,
  _ALTIVEC   = 0x00000002 ,
  _MMX       = 0x00000004 ,
  _3DNOW     = 0x00000008 ,
  _SSE       = 0x00000010 ,
  _SSE2      = 0x00000020 ,
  _SSE3      = 0x00000040 ,
  _SSE41     = 0x00000100 ,
  _SSE42     = 0x00000200 }
  Features                ;

typedef enum              {
  Unknown    = 0          , // < cannot determine power status
  OnBattery  = 1          , // < Not plugged in, running on the battery
  NoBattery  = 2          , // < Plugged in, no battery available
  Charging   = 3          , // < Plugged in, charging battery
  Charged    = 4          } // < Plugged in, battery charged
  PowerState              ;

class LIBPARALLEL_EXPORT Usage
{
  public:

    explicit     Usage            (void) ;
    virtual     ~Usage            (void) ;

    static bool  Tell             (CpuDetails * details) ;

    std::string  Cpu              (void) ;
    int          Processors       (void) ;
    int64_t      Memory           (void) ;
    bool         Has              (Features feature) ;
    std::string  Name             (Features feature) ;
    int          CacheLineSize    (void) ;
    uint32_t     GetFeatures      (void) ;
    int64_t      Timestamp        (void) ; // RDTSC, Time Stamp Counter
    short        GetUsage         (bool cpu = true) ;
    bool         Tell             (ProcessDetails * process) ;

    /**
     *  Get the current power supply details.
     *
     *  secs : Seconds of battery life left. You can pass a NULL here if
     *         you don't care. Will return -1 if we can't determine a
     *         value, or we're not running on a battery.
     *
     *  pct : Percentage of battery life left, between 0 and 100. You can
     *        pass a NULL here if you don't care. Will return -1 if we
     *        can't determine a value, or we're not running on a battery.
     *
     *  return: The state of the battery (if any).
     */

    PowerState   Power (int & seconds,int & percent) ;

  protected:

    void * PrivatePacket ;

  private:

} ;

}

class LIBPARALLEL_EXPORT ThreadData : public Destroyer
{
  public:

    typedef enum ThreadState  {
                 Idle     = 0 ,
                 Active   = 1 ,
                 Deactive = 2 ,
                 Recycle  = 3 }
                 States       ;

    int32_t        Id          ;
    uint64_t       Uuid        ;
    int32_t        Type        ;
    int32_t        Priority    ;
    int64_t        Status      ;
    int32_t        Running     ;
    int32_t        StackSize   ;
    int64_t        StartTime   ;
    bool           Reservation ;
    bool           isContinue  ;
    bool         * Controller  ;
    void         * Arguments   ;
    void         * Extra       ;

    explicit       ThreadData  (void) ;
    virtual       ~ThreadData  (void) ;

    virtual bool   Recycling   (void) ;
    virtual bool   Destructor  (void) ;

    virtual void * Register    (void * package) ;

    void           Start       (void) ;
    void           Stop        (void) ;
    void           Join        (void) ;
    bool           YieldThread (void) ;
    bool           Suspend     (void) ;
    bool           Resume      (void) ;
    bool           Cancel      (int exitCode=0) ;
    void           TryCancel   (void) ;

    int            setPriority (int priority) ;

    bool           isAlive     (void) ;
    bool           isSelf      (void) ;
    bool           isEqual     (int32_t Id) ;
    bool           canContinue (void) ;
    int64_t        Elapsed     (void) const ;

    void           AssignId    (void) ;

    bool           Run         (void * data) ;
    bool           Go          (void * data) ;

    void         * Hidden      (void) ;

  protected:

    void         * PrivatePacket ;

  private:

    Locker         lock ;

} ;

class LIBPARALLEL_EXPORT Thread : public Destroyer
{
  public:

    int                   running     ;
    int                   StackSize   ;
    bool                  Reservation ;
    bool                  isContinue  ;
    bool                * Controller  ;

    explicit              Thread       (int stackSize,bool reservation) ;
    explicit              Thread       (void) ;
    virtual              ~Thread       (void) ;

    virtual bool          Recycling    (void) ;
    virtual bool          Destructor   (void) ;

    virtual void        * Register     (void * package) ;

    void                  setClassName (std::string name) ;
    void                  setKeys      (std::string thread,std::string data) ;
    int                   setPriority  (int priority) ;

    virtual void          start        (void) ;
    virtual ThreadData  * start        (int Type) ;
    virtual ThreadData  * start        (int Type,void * arguments) ;

//    virtual ThreadData  * start       (int Type,VarArgs  & arguments) ;
//    virtual ThreadData  * start       (int Type,XmlValue & arguments) ;

    virtual ThreadData  * drawer       (void) ;
    virtual ThreadData  * drawer       (int Id) ;
    virtual int           DrawerId     (void) ;

    virtual void          quit         (void) ;
    virtual void          suspend      (void) ;
    virtual void          resume       (void) ;
    virtual void          terminate    (void) ;
    virtual void          cleanup      (void) ;
    virtual bool          proceed      (void) ;
    virtual bool          finalize     (int interval = 20) ;

    virtual bool          NotStopped   (void) ;

    virtual bool          IsContinue   (void) ;
    virtual bool          IsContinue   (bool go) ;
    virtual bool          IsContinue   (int Id) ;
    virtual bool          IsContinue   (ThreadData * data) ;
    virtual bool          IsContinue   (bool go,ThreadData * data) ;
    virtual bool          IsContinue   (int action,ThreadData * data) ;

    virtual int           IsRunning    (std::list<ThreadData *> & threads,bool purge = false) ;
    virtual int           exit         (int exitcode) ;

    int64_t               Elapsed     (void) ;
    int64_t               Elapsed     (int Id) ;

    void                  actualRun    (void) ;
    void                  actualRun    (int Type,ThreadData * data) ;
    void                  actualJoin   (void) ;

  protected:

    void   * PrivatePacket ;
    Locker * lock          ;

    virtual void          ThreadEvent  (void) ;
    virtual void          run          (void) ;

    #ifndef DONT_USE_NAMESPACE
    virtual void          run          (int Type,PARALLEL_NAMESPACE::ThreadData * Data) ;
    #else
    virtual void          run          (int Type,ThreadData * Data) ;
    #endif

    virtual bool          detection    (void) ;
    virtual bool          recycle      (void) ;

    void                  sleep        (int64_t  seconds) ;
    void                  msleep       (int64_t mseconds) ;
    void                  usleep       (int64_t useconds) ;

    void                  skip         (int64_t  seconds,int64_t intervalus=10000,bool * dropOut=nullptr) ;
    void                  mskip        (int64_t mseconds,int64_t intervalus=1000 ,bool * dropOut=nullptr) ;
    void                  uskip        (int64_t useconds,int64_t intervalus=10   ,bool * dropOut=nullptr) ;

  private:

    void                  LockThread   (void) ;
    void                  UnlockThread (void) ;

} ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
