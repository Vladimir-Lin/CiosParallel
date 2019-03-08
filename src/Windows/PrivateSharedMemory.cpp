#include "PrivateParallel.hpp"

#pragma pack(push,1)

#pragma pack(pop)

#ifndef DONT_USE_NAMESPACE
namespace PARALLEL_NAMESPACE {
#endif

PrivateSharedMemory:: PrivateSharedMemory (void)
{
  Size     = 0       ;
  SHM      = nullptr ;
  Data     = nullptr ;
  attached = false   ;
  created  = false   ;
}

PrivateSharedMemory::~PrivateSharedMemory (void)
{
  this -> close ( ) ;
}

bool PrivateSharedMemory::create(std::wstring key,int64_t S)
{
  LPCWSTR             K = reinterpret_cast<LPCWSTR> ( key . c_str ( ) )      ;
  SECURITY_ATTRIBUTES sa                                                     ;
  SECURITY_DESCRIPTOR SD                                                     ;
  ////////////////////////////////////////////////////////////////////////////
  ::InitializeSecurityDescriptor ( &SD , SECURITY_DESCRIPTOR_REVISION )      ;
  ::SetSecurityDescriptorDacl    ( &SD , TRUE , (PACL)NULL,FALSE      )      ;
  ////////////////////////////////////////////////////////////////////////////
  ZeroMemory                     ( &sa , sizeof(sa)                   )      ;
  sa . nLength              = sizeof(sa)                                     ;
  sa . bInheritHandle       = FALSE                                          ;
  sa . lpSecurityDescriptor = &SD                                            ;
  ////////////////////////////////////////////////////////////////////////////
  this -> SHM = CreateFileMappingW                                           (
                  INVALID_HANDLE_VALUE                                       ,
                  &sa                                                        ,
                  PAGE_READWRITE                                             ,
                  0                                                          ,
                  (DWORD) S                                                  ,
                  K                                                        ) ;
  if ( this -> SHM != NULL )                                                 {
    this -> Data = (char *)::MapViewOfFile                                   (
                     this -> SHM                                             ,
                     FILE_MAP_ALL_ACCESS                                     ,
                     0                                                       ,
                     0                                                       ,
                     (DWORD) S                                             ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  bool success                                                               ;
  success = ( ( nullptr != this -> SHM ) && ( nullptr != this -> Data ) )    ;
  this -> Size    = success ? S : 0                                          ;
  if ( success ) ::memset ( this -> Data , 0 , (size_t) this -> Size )       ;
  this -> attached = success                                                 ;
  this -> created  = success                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  return success                                                             ;
}

bool PrivateSharedMemory::open(std::wstring key,int64_t S)
{
  LPCWSTR K = reinterpret_cast<LPCWSTR> ( key . c_str ( ) )                  ;
  ////////////////////////////////////////////////////////////////////////////
  this -> SHM = OpenFileMappingW                                             (
                  FILE_MAP_ALL_ACCESS                                        ,
                  FALSE                                                      ,
                  K                                                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  if ( nullptr != this -> SHM )                                              {
    this -> Data = (char *) ::MapViewOfFile                                  (
                                this -> SHM                                  ,
                                FILE_MAP_ALL_ACCESS                          ,
                                0                                            ,
                                0                                            ,
                                (DWORD) S                                  ) ;
    if ( nullptr == this -> Data )                                           {
      ::CloseHandle ( this -> SHM )                                          ;
      this -> SHM  = nullptr                                                 ;
      this -> Data = nullptr                                                 ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  bool success                                                               ;
  success = ( ( nullptr != this -> SHM ) && ( nullptr != this -> Data ) )    ;
  this -> Size     = success ? S : 0                                         ;
  this -> attached = success                                                 ;
  this -> created  = false                                                   ;
  ////////////////////////////////////////////////////////////////////////////
  return success                                                             ;
}

bool PrivateSharedMemory::close(void)
{
  if ( ( nullptr != this -> SHM ) && ( nullptr != this -> Data ) ) {
    ::UnmapViewOfFile ( this -> Data )                             ;
    ::CloseHandle     ( this -> SHM  )                             ;
  }                                                                ;
  //////////////////////////////////////////////////////////////////
  this -> SHM      = nullptr                                       ;
  this -> Data     = nullptr                                       ;
  this -> attached = false                                         ;
  this -> created  = false                                         ;
  //////////////////////////////////////////////////////////////////
  return true                                                      ;
}

#ifndef DONT_USE_NAMESPACE
}
#endif
