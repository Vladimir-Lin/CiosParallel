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

HEADERS     += $$SRCROOT/include/parallel.hpp

SOURCES     += $$SRCPATH/parallel.cpp
SOURCES     += $$SRCPATH/Windows/ThreadData.cpp
SOURCES     += $$SRCPATH/Windows/Thread.cpp

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
