#include "PrivateParallel.hpp"

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

namespace CPU {

Usage:: Usage(void)
{
  CpuUsageHandler * ch = new CpuUsageHandler ( ) ;
  ch -> handler = new PrivateCpuUsage ( )        ;
  this -> PrivatePacket = (void *) ch            ;
}

Usage::~Usage(void)
{
  if ( nullptr != this -> PrivatePacket )                            {
    CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
    delete ch -> handler                                             ;
    delete ch                                                        ;
    this -> PrivatePacket = nullptr                                  ;
  }                                                                  ;
}

std::string Usage::Cpu(void)
{
  if ( nullptr == this -> PrivatePacket ) return ""                ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return std::string ( ch -> handler -> GetCpuType ( ) )           ;
}

int Usage::Processors(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> Processors ( )                           ;
}

bool Usage::Has(Features feature)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> HasFeature ( feature )                   ;
}

std::string Usage::Name(Features feature)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  if ( ch -> handler -> FeatureExists ( feature ) )                {
    return std::string ( "Unknown" )                               ;
  }                                                                ;
  return ch -> handler -> FeatureNames [ feature ]                 ;
}

int Usage::CacheLineSize(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> CacheLineSize ( )                        ;
}

uint32_t Usage::GetFeatures(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> GetFeatures ( )                          ;
}

int64_t Usage::Timestamp(void)
{
  return StarDate::RDTSC ( ) ;
}

int64_t Usage::Memory(void)
{
  if ( nullptr == this -> PrivatePacket ) return 0                 ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> Memory ( )                               ;
}

short Usage::GetUsage(bool cpu)
{
  if ( nullptr == this -> PrivatePacket ) return Unknown           ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> GetUsage ( cpu )                         ;
}

bool Usage::Tell(CpuDetails * cpu)
{
  if ( NULL == cpu            ) return false               ;
  if ( cpu -> Initialized > 0 ) return true                ;
  //////////////////////////////////////////////////////////
  CPU::Usage usage                                         ;
  std::string cpuType = usage . Cpu ( )                    ;
  cpu -> Initialized = 1                                   ;
  cpu -> Processors  = usage . Processors    ( )           ;
  cpu -> Memory      = usage . Memory        ( )           ;
  cpu -> Cache       = usage . CacheLineSize ( )           ;
  cpu -> Features    = usage . GetFeatures   ( )           ;
  if ( cpuType . length ( ) > 0 )                          {
    ::strcpy_s ( cpu -> Name , 128 , cpuType . c_str ( ) ) ;
  }                                                        ;
  //////////////////////////////////////////////////////////
  cpu -> Initialized = 2                                   ;
  return true                                              ;
}

bool Usage::Tell(ProcessDetails * process)
{
  if ( nullptr == this -> PrivatePacket ) return Unknown           ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> Tell ( process )                         ;
}

PowerState Usage::Power (int & seconds,int & percent)
{
  if ( nullptr == this -> PrivatePacket ) return Unknown           ;
  CpuUsageHandler * ch = (CpuUsageHandler *) this -> PrivatePacket ;
  return ch -> handler -> Power ( seconds , percent )              ;
}

}

#ifndef DONT_USE_NAMESPACE
}
#endif
