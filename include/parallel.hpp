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
#include <time.h>

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
    bool                  isContinue  ;
    bool                  Reservation ;

//    ThreadData            Data        ;
//    QList<ThreadData *>   AllThreads  ;

    explicit              Thread      (int stackSize,bool reservation) ;
    explicit              Thread      (void) ;
    virtual              ~Thread      (void) ;

    virtual bool          Recycling   (void) ;
    virtual bool          Destructor  (void) ;

    virtual void        * Register    (void * package) ;

    int                   setPriority (int priority) ;

    virtual void          start       (void) ;
    virtual ThreadData  * start       (int Type) ;
//    virtual ThreadData  * start       (int Type,VarArgs      & arguments) ;
//    virtual ThreadData  * start       (int Type,XmlValue     & arguments) ;
//    virtual ThreadData  * start       (int Type,ThreadFunction ExternalThread) ;
    virtual ThreadData  * drawer      (void) ;
    virtual int           DrawerId    (void) ;
    virtual void          quit        (void) ;
    virtual void          suspend     (void) ;
    virtual void          resume      (void) ;
    virtual void          terminate   (void) ;
    virtual void          cleanup     (void) ;
    virtual bool          proceed     (void) ;
    virtual bool          finalize    (int interval = 20) ;
    virtual bool          NotStopped  (void) ;
    virtual bool          IsContinue  (void) ;
    virtual bool          IsContinue  (bool go) ;
    virtual bool          IsContinue  (int Id) ;
    virtual bool          IsContinue  (ThreadData * data) ;
    virtual bool          IsContinue  (bool go,ThreadData * data) ;
    virtual bool          IsContinue  (int action,ThreadData * data) ;
//    virtual int           IsRunning   (QList<ThreadData *> & threads,bool purge = false) ;
    virtual int           exit        (int exitcode) ;

    void                  actualRun   (void) ;
    void                  actualRun   (int Type,ThreadData * data) ;
    void                  actualJoin  (void) ;

  protected:

    void * PrivatePacket ;

    virtual void          ThreadEvent (void) ;
    virtual void          run         (void) ;
    virtual void          run         (int Type,ThreadData * Data) ;

    virtual bool          detection   (void) ;
    virtual bool          recycle     (void) ;

  private:

    Locker lock ;

} ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
