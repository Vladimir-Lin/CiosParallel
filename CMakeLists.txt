cmake_minimum_required(VERSION 3.0)
project(libparallel)
include(CheckIncludeFile)

option(NONAMESPACE "Don't use namespace in the header" off)

string(TIMESTAMP COMPILE_DATETIME "%Y/%m/%d %H:%M:%S")

if(NONAMESPACE)
  add_definitions("-DDONT_USE_NAMESPACE=1")
endif(NONAMESPACE)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions("-DCIOSDEBUG=1")
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
else()
  set(CMAKE_BUILD_TYPE Release)
endif()

# Handling compilers in different platforms

# using Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
endif()

# using GCC
if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
endif()

# using Intel C++
if(CMAKE_CXX_COMPILER_ID STREQUAL Intel)
endif()

# using Visual Studio C++
if(MSVC)
  # message( ${CMAKE_CXX_COMPILER} )
  add_definitions("/wd4819")
  add_definitions("/nologo")
  if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(TARGET_ARCH x64)
    add_definitions("-DCIOS_X64=1")
  elseif("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
    set(TARGET_ARCH x86)
    add_definitions("-DCIOS_X86=1")
  endif()
endif(MSVC)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UUIDS REQUIRED UUIDs)
pkg_check_modules(STARDATE REQUIRED libstardate)

include_directories(include)
include_directories (${UUIDS_INCLUDE_DIRS})
include_directories (${STARDATE_INCLUDE_DIRS})
link_directories(${UUIDS_LIBRARY_DIRS})
link_directories(${STARDATE_LIBRARY_DIRS})

# Header
set(HEADERS include/parallel.hpp)
set(HEADERS src/Windows/PrivateParallel.hpp)

# Source codes
set(CORES
  src/parallel.cpp
  src/Windows/Locker.cpp
  src/Windows/Latcher.cpp
  src/Windows/ThreadData.cpp
  src/Windows/Thread.cpp
  src/Windows/PrivateParallel.cpp)

# Handling pkg-config file
configure_file(libparallel.pc.in libparallel.pc @ONLY)

if(BUILD_SHARED_LIBS)
  add_library(libparallel SHARED ${CORES} ${HEADERS})
#  target_link_libraries(libparallel m)
  add_definitions("-DBUILD_SHARED_LIBS=1")
  add_definitions("-DEXPORT_LIBPARALLEL=1")
  install(TARGETS libparallel
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
  install(FILES include/parallel.hpp DESTINATION include)
  install(FILES ${CMAKE_BINARY_DIR}/libparallel.pc DESTINATION lib/packages)
else(BUILD_SHARED_LIBS)
  add_library(libparallel STATIC ${CORES} ${HEADERS})
#  target_link_libraries(libnations m)
  install(TARGETS libparallel DESTINATION lib)
  install(FILES include/parallel.hpp DESTINATION include)
  install(FILES ${CMAKE_BINARY_DIR}/libparallel.pc DESTINATION lib/packages)
endif(BUILD_SHARED_LIBS)

target_link_libraries(libparallel ${UUIDS_LIBRARIES})
target_link_libraries(libparallel ${STARDATE_LIBRARIES})
target_compile_options(libparallel PUBLIC ${UUIDS_CFLAGS_OTHER})
target_compile_options(libparallel PUBLIC ${STARDATE_CFLAGS_OTHER})
