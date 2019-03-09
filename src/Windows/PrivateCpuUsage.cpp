#include "PrivateParallel.hpp"

#include <Psapi.h>
#include <intrin.h>

//////////////////////////////////////////////////////////////////////////////

inline int haveCPUID(void)
{
  int has_CPUID = 0 ;
/* *INDENT-OFF* */
#if defined(__GNUC__) && defined(i386)
    __asm__ (
"        pushfl                      # Get original EFLAGS             \n"
"        popl    %%eax                                                 \n"
"        movl    %%eax,%%ecx                                           \n"
"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
"        pushl   %%eax               # Save new EFLAGS value on stack  \n"
"        popfl                       # Replace current EFLAGS value    \n"
"        pushfl                      # Get new EFLAGS                  \n"
"        popl    %%eax               # Store new EFLAGS in EAX         \n"
"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
"        jz      1f                  # Processor=80486                 \n"
"        movl    $1,%0               # We have CPUID support           \n"
"1:                                                                    \n"
    : "=m" (has_CPUID)
    :
    : "%eax", "%ecx"
    );
#elif defined(__GNUC__) && defined(__x86_64__)
/* Technically, if this is being compiled under __x86_64__ then it has
   CPUid by definition.  But it's nice to be able to prove it.  :)      */
    __asm__ (
"        pushfq                      # Get original EFLAGS             \n"
"        popq    %%rax                                                 \n"
"        movq    %%rax,%%rcx                                           \n"
"        xorl    $0x200000,%%eax     # Flip ID bit in EFLAGS           \n"
"        pushq   %%rax               # Save new EFLAGS value on stack  \n"
"        popfq                       # Replace current EFLAGS value    \n"
"        pushfq                      # Get new EFLAGS                  \n"
"        popq    %%rax               # Store new EFLAGS in EAX         \n"
"        xorl    %%ecx,%%eax         # Can not toggle ID bit,          \n"
"        jz      1f                  # Processor=80486                 \n"
"        movl    $1,%0               # We have CPUID support           \n"
"1:                                                                    \n"
    : "=m" (has_CPUID)
    :
    : "%rax", "%rcx"
    ) ;
#elif defined(CIOS_X64)
    has_CPUID = 1 ;
#elif defined(CIOS_X86) || defined(__WATCOMC__)
    __asm {
        pushfd                      ; Get original EFLAGS
        pop     eax
        mov     ecx, eax
        xor     eax, 200000h        ; Flip ID bit in EFLAGS
        push    eax                 ; Save new EFLAGS value on stack
        popfd                       ; Replace current EFLAGS value
        pushfd                      ; Get new EFLAGS
        pop     eax                 ; Store new EFLAGS in EAX
        xor     eax, ecx            ; Can not toggle ID bit,
        jz      done                ; Processor=80486
        mov     has_CPUID,1         ; We have CPUID support
done:
  }
#elif defined(__sun) && defined(__i386)
    __asm (
"       pushfl                 \n"
"       popl    %eax           \n"
"       movl    %eax,%ecx      \n"
"       xorl    $0x200000,%eax \n"
"       pushl   %eax           \n"
"       popfl                  \n"
"       pushfl                 \n"
"       popl    %eax           \n"
"       xorl    %ecx,%eax      \n"
"       jz      1f             \n"
"       movl    $1,-8(%ebp)    \n"
"1:                            \n"
    );
#elif defined(__sun) && defined(__amd64)
    __asm (
"       pushfq                 \n"
"       popq    %rax           \n"
"       movq    %rax,%rcx      \n"
"       xorl    $0x200000,%eax \n"
"       pushq   %rax           \n"
"       popfq                  \n"
"       pushfq                 \n"
"       popq    %rax           \n"
"       xorl    %ecx,%eax      \n"
"       jz      1f             \n"
"       movl    $1,-8(%rbp)    \n"
"1:                            \n"
    );
#endif
/* *INDENT-ON* */
  return has_CPUID;
}

//////////////////////////////////////////////////////////////////////////////

#if defined(__GNUC__) && defined(i386)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushl %%ebx        \n" \
"        cpuid              \n" \
"        movl %%ebx, %%esi  \n" \
"        popl %%ebx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif defined(__GNUC__) && defined(__x86_64__)
#define cpuid(func, a, b, c, d) \
    __asm__ __volatile__ ( \
"        pushq %%rbx        \n" \
"        cpuid              \n" \
"        movq %%rbx, %%rsi  \n" \
"        popq %%rbx         \n" : \
            "=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#elif defined(CIOS_X64)
#define cpuid(func, a, b, c, d) \
   {                             \
     int registers[4]      ;     \
     __cpuid(registers, 0) ;     \
     a = registers[0]      ;     \
     b = registers[1]      ;     \
     c = registers[2]      ;     \
     d = registers[3]      ;     \
   }
#elif defined(CIOS_X86) || defined(__WATCOMC__)
#define cpuid(func, a, b, c, d) \
    __asm { \
        __asm mov eax, func \
        __asm cpuid \
        __asm mov a, eax \
        __asm mov b, ebx \
        __asm mov c, ecx \
        __asm mov d, edx \
    }
#else
#define cpuid(func, a, b, c, d) \
    a = b = c = d = 0
#endif

//////////////////////////////////////////////////////////////////////////////

inline int getCPUIDFeatures(void)
{
  int features = 0       ;
  int a, b, c, d         ;
  cpuid(0, a, b, c, d)   ;
  if (a >= 1)            {
    cpuid(1, a, b, c, d) ;
    features = d         ;
  }                      ;
  return features        ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveRDTSC(void)
{
  if ( haveCPUID ( ) )                           {
    return ( getCPUIDFeatures ( ) & 0x00000010 ) ;
  }                                              ;
  return 0                                       ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveAltiVec(void)
{
  volatile int altivec = 0;
#if (defined(__MACOSX__) && (defined(__ppc__) || defined(__ppc64__))) || (defined(__OpenBSD__) && defined(__powerpc__))
#ifdef __OpenBSD__
    int selectors[2] = { CTL_MACHDEP, CPU_ALTIVEC };
#else
    int selectors[2] = { CTL_HW, HW_VECTORUNIT };
#endif
    int hasVectorUnit = 0;
    size_t length = sizeof(hasVectorUnit);
    int error = sysctl(selectors, 2, &hasVectorUnit, &length, NULL, 0);
    if (0 == error)
        altivec = (hasVectorUnit != 0);
#elif SDL_ALTIVEC_BLITTERS && HAVE_SETJMP
    void (*handler) (int sig);
    handler = signal(SIGILL, illegal_instruction);
    if (setjmp(jmpbuf) == 0) {
        asm volatile ("mtspr 256, %0\n\t" "vand %%v0, %%v0, %%v0"::"r" (-1));
        altivec = 1;
    }
    signal(SIGILL, handler);
#endif
  return altivec ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveMMX(void)
{
  if ( haveCPUID ( ) )                           {
    return ( getCPUIDFeatures ( ) & 0x00800000 ) ;
  }                                              ;
  return 0                                       ;
}

//////////////////////////////////////////////////////////////////////////////

inline int have3DNow(void)
{
  if (haveCPUID())                  {
    int a, b, c, d                  ;
    cpuid(0x80000000, a, b, c, d)   ;
    if (a >= 0x80000001)            {
      cpuid(0x80000001, a, b, c, d) ;
      return (d & 0x80000000)       ;
    }                               ;
  }                                 ;
  return 0                          ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveSSE(void)
{
  if (haveCPUID())                           {
    return (getCPUIDFeatures() & 0x02000000) ;
  }                                          ;
  return 0                                   ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveSSE2(void)
{
  if (haveCPUID())                           {
    return (getCPUIDFeatures() & 0x04000000) ;
  }                                          ;
  return 0                                   ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveSSE3(void)
{
  if (haveCPUID())            {
    int a, b, c, d            ;
    cpuid(0, a, b, c, d)      ;
    if (a >= 1)               {
      cpuid(1, a, b, c, d)    ;
      return (c & 0x00000001) ;
    }                         ;
  }                           ;
  return 0                    ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveSSE41(void)
{
  if (haveCPUID())            {
    int a, b, c, d            ;
    cpuid(1, a, b, c, d)      ;
    if (a >= 1)               {
      cpuid(1, a, b, c, d)    ;
      return (c & 0x00080000) ;
    }                         ;
  }                           ;
  return 0                    ;
}

//////////////////////////////////////////////////////////////////////////////

inline int haveSSE42(void)
{
  if (haveCPUID())            {
    int a, b, c, d            ;
    cpuid(1, a, b, c, d)      ;
    if (a >= 1)               {
      cpuid(1, a, b, c, d)    ;
      return (c & 0x00100000) ;
    }                         ;
  }                           ;
  return 0                    ;
}

//////////////////////////////////////////////////////////////////////////////

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

//////////////////////////////////////////////////////////////////////////////

#define CACHELINESIZE 128

static int      CPUs           = 0                                           ;
static uint32_t features       = 0xFFFFFFFF                                  ;
static char     CpuType [ 13 ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0 }               ;

//////////////////////////////////////////////////////////////////////////////

PrivateCpuUsage:: PrivateCpuUsage(void)
{
  m_nCpuUsage = -1                                      ;
  m_dwLastRun =  0                                      ;
  m_lRunCount =  0                                      ;
  ///////////////////////////////////////////////////////
  ZeroMemory ( &m_ftPrevSysKernel  , sizeof(FILETIME) ) ;
  ZeroMemory ( &m_ftPrevSysUser    , sizeof(FILETIME) ) ;
  ZeroMemory ( &m_ftPrevSysIdle    , sizeof(FILETIME) ) ;
  ZeroMemory ( &m_ftPrevProcKernel , sizeof(FILETIME) ) ;
  ZeroMemory ( &m_ftPrevProcUser   , sizeof(FILETIME) ) ;
  ///////////////////////////////////////////////////////
  FeatureNames [ CPU::_RDTSC   ] = "RDTSC"              ;
  FeatureNames [ CPU::_ALTIVEC ] = "ALTIVEC"            ;
  FeatureNames [ CPU::_MMX     ] = "MMX"                ;
  FeatureNames [ CPU::_3DNOW   ] = "3DNOW"              ;
  FeatureNames [ CPU::_SSE     ] = "SSE"                ;
  FeatureNames [ CPU::_SSE2    ] = "SSE2"               ;
  FeatureNames [ CPU::_SSE3    ] = "SSE3"               ;
  FeatureNames [ CPU::_SSE41   ] = "SSE41"              ;
  FeatureNames [ CPU::_SSE42   ] = "SSE42"              ;
}

PrivateCpuUsage::~PrivateCpuUsage(void)
{
}

int PrivateCpuUsage::Processors(void)
{
  if ( CPUs > 0 ) return CPUs ;
  #if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_ONLN)
  if (CPUs <= 0)                                   {
    CPUs = (int)sysconf(_SC_NPROCESSORS_ONLN)      ;
  }                                                ;
  #endif
  #ifdef HAVE_SYSCTLBYNAME
  if (CPUs <= 0)                                   {
    size_t size = sizeof(CPUs)                     ;
    sysctlbyname("hw.ncpu", &CPUs, &size, NULL, 0) ;
  }                                                ;
  #endif
  #ifdef CIOS_X64
  if (CPUs <= 0)                                   {
    SYSTEM_INFO info                               ;
    ::GetSystemInfo ( &info )                      ;
    CPUs = info . dwNumberOfProcessors             ;
  }                                                ;
  #endif
  #ifdef CIOS_X86
  if (CPUs <= 0)                                   {
    SYSTEM_INFO info                               ;
    ::GetSystemInfo ( &info )                      ;
    CPUs = info . dwNumberOfProcessors             ;
  }                                                ;
  #endif
  /* There has to be at least 1, right? :) */
  if (CPUs <= 0) CPUs = 1                          ;
  return CPUs                                      ;
}

const char * PrivateCpuUsage::GetCpuType(void)
{
  if ( CpuType[0] > 0 ) return CpuType        ;
  int i = 0                                   ;
  int a, b, c, d                              ;
  if ( haveCPUID ( ) )                        {
    cpuid(0x00000000, a, b, c, d)             ;
    CpuType [i++] = (char)(b & 0xff); b >>= 8 ;
    CpuType [i++] = (char)(b & 0xff); b >>= 8 ;
    CpuType [i++] = (char)(b & 0xff); b >>= 8 ;
    CpuType [i++] = (char)(b & 0xff); b >>= 8 ;
    CpuType [i++] = (char)(d & 0xff); d >>= 8 ;
    CpuType [i++] = (char)(d & 0xff); d >>= 8 ;
    CpuType [i++] = (char)(d & 0xff); d >>= 8 ;
    CpuType [i++] = (char)(d & 0xff); d >>= 8 ;
    CpuType [i++] = (char)(c & 0xff); c >>= 8 ;
    CpuType [i++] = (char)(c & 0xff); c >>= 8 ;
    CpuType [i++] = (char)(c & 0xff); c >>= 8 ;
    CpuType [i++] = (char)(c & 0xff); c >>= 8 ;
  }                                           ;
  if ( 0 == CpuType [ 0 ] )                   {
    ::strcpy_s ( CpuType , 13 , "Unknown" )   ;
  }                                           ;
  return CpuType                              ;
}

int PrivateCpuUsage::CacheLineSize(void)
{
  const char * cpuType = GetCpuType ( )    ;
  if (strcmp(cpuType,"GenuineIntel") == 0) {
    int a, b, c, d                         ;
    cpuid(0x00000001, a, b, c, d)          ;
    return (((b >> 8) & 0xff) * 8)         ;
  } else
  if (strcmp(cpuType,"AuthenticAMD") == 0) {
    int a, b, c, d                         ;
    cpuid(0x80000005, a, b, c, d)          ;
    return (c & 0xff)                      ;
  }                                        ;
  return CACHELINESIZE                     ;
}

bool PrivateCpuUsage::FeatureExists(CPU::Features feature)
{
  if ( CPU::_RDTSC   == feature ) return true ;
  if ( CPU::_ALTIVEC == feature ) return true ;
  if ( CPU::_MMX     == feature ) return true ;
  if ( CPU::_3DNOW   == feature ) return true ;
  if ( CPU::_SSE     == feature ) return true ;
  if ( CPU::_SSE2    == feature ) return true ;
  if ( CPU::_SSE3    == feature ) return true ;
  if ( CPU::_SSE41   == feature ) return true ;
  if ( CPU::_SSE42   == feature ) return true ;
  return false                                ;
}

bool PrivateCpuUsage::HasFeature(CPU::Features feature)
{
  return ( ( features & (uint32_t) feature ) == (uint32_t) feature ) ;
}

uint32_t PrivateCpuUsage::GetFeatures(void)
{
  if ( features != 0xFFFFFFFF ) return features    ;
  features = 0                                     ;
  //////////////////////////////////////////////////
  if ( haveRDTSC   ( ) ) features |= CPU::_RDTSC   ;
  if ( haveAltiVec ( ) ) features |= CPU::_ALTIVEC ;
  if ( haveMMX     ( ) ) features |= CPU::_MMX     ;
  if ( have3DNow   ( ) ) features |= CPU::_3DNOW   ;
  if ( haveSSE     ( ) ) features |= CPU::_SSE     ;
  if ( haveSSE2    ( ) ) features |= CPU::_SSE2    ;
  if ( haveSSE3    ( ) ) features |= CPU::_SSE3    ;
  if ( haveSSE41   ( ) ) features |= CPU::_SSE41   ;
  if ( haveSSE42   ( ) ) features |= CPU::_SSE42   ;
  //////////////////////////////////////////////////
  return features                                  ;
}

int64_t PrivateCpuUsage::Memory(void)
{
  MEMORYSTATUSEX MSS                             ;
  BOOL           R                               ;
  ::memset ( &MSS , 0 , sizeof(MEMORYSTATUSEX) ) ;
  MSS . dwLength = sizeof(MEMORYSTATUSEX)        ;
  R = ::GlobalMemoryStatusEx ( &MSS )            ;
  if ( R != 0 )                                  {
    return (int64_t) MSS . ullTotalPhys          ;
  }                                              ;
  ////////////////////////////////////////////////
  return 0                                       ;
}

ULONGLONG PrivateCpuUsage::SubtractTimes(const FILETIME & ftA,const FILETIME & ftB)
{
  LARGE_INTEGER a,b               ;
  a.LowPart  = ftA.dwLowDateTime  ;
  a.HighPart = ftA.dwHighDateTime ;
  b.LowPart  = ftB.dwLowDateTime  ;
  b.HighPart = ftB.dwHighDateTime ;
  return a.QuadPart - b.QuadPart  ;
}

bool PrivateCpuUsage::EnoughTimePassed(void)
{
  const int minElapsedMS = 250;
  ULONGLONG dwCurrentTickCount = ::GetTickCount64();
  return ( dwCurrentTickCount - m_dwLastRun ) > minElapsedMS ;
}

CPU::PowerState PrivateCpuUsage::Power(int & seconds,int & percent)
{
  CPU::PowerState     ps      = CPU::Unknown           ;
  bool                details = false                  ;
  SYSTEM_POWER_STATUS status                           ;
  if ( ! ::GetSystemPowerStatus ( &status ) )          {
    return CPU::Unknown                                ;
  } else
  if ( status . BatteryFlag == 0xFF   )                { /* unknown state */
    return CPU::Unknown                                ;
  } else
  if ( status . BatteryFlag & ( 1 << 7 ) )             { /* no battery */
    return CPU::NoBattery                              ;
  } else
  if ( status . BatteryFlag & ( 1 << 3 ) )             { /* charging */
    ps      = CPU::Charging                            ;
    details = true                                     ;
  } else
  if ( status . ACLineStatus == 1 )                    {
    ps      = CPU::Charged                             ;
    details = true                                     ;
  } else                                               {
    ps      = CPU::OnBattery                           ;
    details = true                                     ;
  }                                                    ;
  //////////////////////////////////////////////////////
  percent = -1                                         ;
  seconds = -1                                         ;
  if (details)                                         {
    const int pct  = (int) status . BatteryLifePercent ;
    const int secs = (int) status . BatteryLifeTime    ;
    if ( pct != 255 )                                  { /* 255 == unknown */
      percent = ( pct > 100 ) ? 100 : pct              ; /* clamp between 0%, 100% */
    }                                                  ;
    if ( secs != 0xFFFFFFFF )                          { /* ((DWORD)-1) == unknown */
      seconds = secs                                   ;
    }                                                  ;
  }                                                    ;
  //////////////////////////////////////////////////////
  return ps                                            ;
}

short PrivateCpuUsage::GetUsage(bool cpu)
{
  short nCpuCopy = m_nCpuUsage                                               ;
  ////////////////////////////////////////////////////////////////////////////
  if ( 1 == ::InterlockedIncrement ( &m_lRunCount ) )                        {
    //////////////////////////////////////////////////////////////////////////
    if ( ! EnoughTimePassed ( ) )                                            {
      ::InterlockedDecrement ( &m_lRunCount )                                ;
      return nCpuCopy                                                        ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    FILETIME ftSysIdle                                                       ;
    FILETIME ftSysKernel                                                     ;
    FILETIME ftSysUser                                                       ;
    FILETIME ftProcCreation                                                  ;
    FILETIME ftProcExit                                                      ;
    FILETIME ftProcKernel                                                    ;
    FILETIME ftProcUser                                                      ;
    //////////////////////////////////////////////////////////////////////////
    bool     notget = true                                                   ;
    bool     getcpu = true                                                   ;
    bool     getpro = true                                                   ;
    //////////////////////////////////////////////////////////////////////////
    if ( cpu )                                                               {
      getcpu = ::GetSystemTimes  ( &ftSysIdle , &ftSysKernel , &ftSysUser  ) ;
      getpro = true                                                          ;
    } else                                                                   {
      getcpu = ::GetSystemTimes  ( &ftSysIdle , &ftSysKernel , &ftSysUser  ) ;
      getpro = ::GetProcessTimes ( ::GetCurrentProcess ( )                   ,
                                   &ftProcCreation                           ,
                                   &ftProcExit                               ,
                                   &ftProcKernel                             ,
                                   &ftProcUser                             ) ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    notget = ( ! getcpu ) || ( ! getpro )                                    ;
    if ( notget )                                                            {
      ::InterlockedDecrement ( &m_lRunCount )                                ;
      return nCpuCopy                                                        ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( 0 != m_dwLastRun )                                                  {
      if ( cpu )                                                             {
        ULONGLONG ftSysKernelDiff  = SubtractTimes ( ftSysKernel , m_ftPrevSysKernel ) ;
        ULONGLONG ftSysUserDiff    = SubtractTimes ( ftSysUser   , m_ftPrevSysUser   ) ;
        ULONGLONG ftSysIdleDiff    = SubtractTimes ( ftSysIdle   , m_ftPrevSysIdle   ) ;
        ULONGLONG nTotalSys        = ftSysKernelDiff  + ftSysUserDiff         ;
        ULONGLONG nTotalProc       = nTotalSys        - ftSysIdleDiff         ;
        if ( nTotalSys > 0 )                                                 {
          m_nCpuUsage = (short) ( ( 10000.0 * nTotalProc ) / nTotalSys )     ;
        }                                                                    ;
      } else                                                                 {
        ULONGLONG ftSysKernelDiff  = SubtractTimes ( ftSysKernel  , m_ftPrevSysKernel  ) ;
        ULONGLONG ftSysUserDiff    = SubtractTimes ( ftSysUser    , m_ftPrevSysUser    ) ;
        ULONGLONG ftProcKernelDiff = SubtractTimes ( ftProcKernel , m_ftPrevProcKernel ) ;
        ULONGLONG ftProcUserDiff   = SubtractTimes ( ftProcUser   , m_ftPrevProcUser   ) ;
        ULONGLONG nTotalSys        = ftSysKernelDiff  + ftSysUserDiff        ;
        ULONGLONG nTotalProc       = ftProcKernelDiff + ftProcUserDiff       ;
        if ( nTotalSys > 0 )                                                 {
          m_nCpuUsage = (short) ( ( 10000.0 * nTotalProc ) / nTotalSys )     ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    m_ftPrevSysKernel  = ftSysKernel                                         ;
    m_ftPrevSysUser    = ftSysUser                                           ;
    m_ftPrevSysIdle    = ftSysIdle                                           ;
    m_ftPrevProcKernel = ftProcKernel                                        ;
    m_ftPrevProcUser   = ftProcUser                                          ;
    m_dwLastRun        = ::GetTickCount64 ( )                                ;
    //////////////////////////////////////////////////////////////////////////
    nCpuCopy = m_nCpuUsage                                                   ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  ::InterlockedDecrement ( &m_lRunCount )                                    ;
  return nCpuCopy                                                            ;
}

#pragma pack(push,1)

typedef struct           {
  HANDLE Handle          ;
  DWORD  ProcessId       ;
} WindowsProcessInternal ;

#pragma pack(pop)

bool PrivateCpuUsage::Tell(ProcessDetails * process)
{
  if ( nullptr == process ) return false                                     ;
  ////////////////////////////////////////////////////////////////////////////
  WindowsProcessInternal * wpi    = nullptr                                  ;
  int offset = ( sizeof(ProcessDetails) - PROCESS_DETAILS_OFFSET )           ;
  if ( process -> occupied <= 1 )                                            {
    wpi = (WindowsProcessInternal *) ( ( (char *) process ) + offset )       ;
    process -> offset     = offset                                           ;
    process -> occupied  |= 3                                                ;
    process -> registered = StarDate::RDTSC       ( )                        ;
    wpi     -> Handle     = ::GetCurrentProcess   ( )                        ;
    wpi     -> ProcessId  = ::GetCurrentProcessId ( )                        ;
    process -> instance   = (uint64_t) wpi -> ProcessId                      ;
  } else                                                                     {
    offset = process -> offset                                               ;
    wpi = (WindowsProcessInternal *) ( ( (char *) process ) + offset )       ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  process -> openmp    = Processors      ( )                                 ;
  process -> timestamp = StarDate::RDTSC ( )                                 ;
  process -> updated   = StarDate::RDTSC ( )                                 ;
  ////////////////////////////////////////////////////////////////////////////
  PROCESS_MEMORY_COUNTERS_EX pmc                                             ;
  BOOL                       R                                               ;
  R = GetProcessMemoryInfo                                                   (
        wpi -> Handle                                                        ,
        (PROCESS_MEMORY_COUNTERS *) &pmc                                     ,
        sizeof(PROCESS_MEMORY_COUNTERS_EX)                                 ) ;
  if ( R != 0 )                                                              {
    process -> peak    = pmc . PeakWorkingSetSize                            ;
    process -> working = pmc . WorkingSetSize                                ;
    process -> page    = pmc . PrivateUsage                                  ;
  } else return false                                                        ;
  ////////////////////////////////////////////////////////////////////////////
  // Get Windows Memory Status
  ////////////////////////////////////////////////////////////////////////////
  MEMORYSTATUSEX MSS                                                         ;
  ::memset ( &MSS , 0 , sizeof(MEMORYSTATUSEX) )                             ;
  MSS . dwLength = sizeof(MEMORYSTATUSEX)                                    ;
  R = ::GlobalMemoryStatusEx ( &MSS )                                        ;
  ////////////////////////////////////////////////////////////////////////////
  if ( 0 == R )                                                              {
    ErrorCode = ::GetLastError ( )                                           ;
    return false                                                             ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  process -> load             = (int32_t) MSS . dwMemoryLoad                 ;
  process -> total            = (int64_t) MSS . ullTotalPhys                 ;
  process -> available        = (int64_t) MSS . ullAvailPhys                 ;
  process -> virtualTotal     = (int64_t) MSS . ullTotalVirtual              ;
  process -> virtualAvailable = (int64_t) MSS . ullAvailVirtual              ;
  ////////////////////////////////////////////////////////////////////////////
  return true                                                                ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
