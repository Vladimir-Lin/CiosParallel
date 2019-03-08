# Author : Foxman A.K.A. Vladimir Lin <lin.vladimir@gmail.com>
# Lastest update : 2019/03/03 18:51

QT          -= gui
TARGET       = libparallel
TEMPLATE     = lib
DEFINES     += QT_DEPRECATED_WARNINGS
DEFINES     += EXPORT_LIBPARALLEL

SRCROOT      = ../
SRCPATH      = $$SRCROOT/src

INCLUDEPATH  = $$SRCROOT/include
INCLUDEPATH += $$SRCPATH/Windows

HEADERS     += $$SRCROOT/include/parallel.hpp
HEADERS     += $$SRCPATH/Windows/PrivateParallel.hpp

SOURCES     += $$SRCPATH/parallel.cpp
SOURCES     += $$SRCPATH/Windows/Locker.cpp
SOURCES     += $$SRCPATH/Windows/Lockerz.cpp
SOURCES     += $$SRCPATH/Windows/Latcher.cpp
SOURCES     += $$SRCPATH/Windows/Mutex.cpp
SOURCES     += $$SRCPATH/Windows/Mutexz.cpp
SOURCES     += $$SRCPATH/Windows/Semaphore.cpp
SOURCES     += $$SRCPATH/Windows/Semaphorez.cpp
SOURCES     += $$SRCPATH/Windows/ThreadData.cpp
SOURCES     += $$SRCPATH/Windows/Thread.cpp
SOURCES     += $$SRCPATH/Windows/SharedMemory.cpp
SOURCES     += $$SRCPATH/Windows/SharedMemoryz.cpp
SOURCES     += $$SRCPATH/Windows/CpuUsage.cpp
SOURCES     += $$SRCPATH/Windows/PrivateParallel.cpp
SOURCES     += $$SRCPATH/Windows/PrivateLockerz.cpp
SOURCES     += $$SRCPATH/Windows/PrivateMutexz.cpp
SOURCES     += $$SRCPATH/Windows/PrivateSemaphorez.cpp
SOURCES     += $$SRCPATH/Windows/PrivateSharedMemory.cpp
SOURCES     += $$SRCPATH/Windows/PrivateSharedMemoryz.cpp
SOURCES     += $$SRCPATH/Windows/PrivateCpuUsage.cpp

OTHER_FILES += $$SRCROOT/CMakeLists.txt
OTHER_FILES += $$SRCROOT/CiosParallel.bat
OTHER_FILES += $$SRCROOT/install.bat
OTHER_FILES += $$SRCROOT/libparallel.pc.in
OTHER_FILES += $$SRCROOT/VERSION.txt

OTHER_FILES += $$SRCROOT/projects/*

OTHER_FILES += $$SRCROOT/docs/index.html
OTHER_FILES += $$SRCROOT/docs/*.txt
OTHER_FILES += $$SRCROOT/docs/tw/*
OTHER_FILES += $$SRCROOT/docs/en/*
OTHER_FILES += $$SRCROOT/docs/cn/*
OTHER_FILES += $$SRCROOT/docs/css/*.css

OTHER_FILES += $$SRCROOT/Html/en/*
OTHER_FILES += $$SRCROOT/Html/tw/*
OTHER_FILES += $$SRCROOT/Html/cn/*

OTHER_FILES += $$SRCROOT/JS/*
OTHER_FILES += $$SRCROOT/PHP/*
OTHER_FILES += $$SRCROOT/Python/*
OTHER_FILES += $$SRCROOT/Ruby/*
OTHER_FILES += $$SRCROOT/scripts/*
