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

    explicit       ThreadData (void) ;
    virtual       ~ThreadData (void) ;

    virtual bool   Recycling  (void) ;
    virtual bool   Destructor (void) ;

    virtual void * Register   (void * package) ;

  protected:

  private:

} ;

class LIBPARALLEL_EXPORT Thread : public Destroyer
{
  public:

    explicit       Thread     (void) ;
    virtual       ~Thread     (void) ;

    virtual bool   Recycling  (void) ;
    virtual bool   Destructor (void) ;

    virtual void * Register   (void * package) ;

  protected:

  private:

} ;

#ifndef DONT_USE_NAMESPACE
}
#endif

#endif
