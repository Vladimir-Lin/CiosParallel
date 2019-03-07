@echo off

set NAME=libparallel
set PARALLEL=%~dp0
set ORIGINAL=%CD%
set COMPILEPATH=%CD%\Parallel
set /p VERSION=<%PARALLEL%\VERSION.txt

mkdir %COMPILEPATH%
cd %COMPILEPATH%

cmd /A /Q /K %PARALLEL%\scripts\install.bat x86 release static
cmd /A /Q /K %PARALLEL%\scripts\install.bat x86 release shared
cmd /A /Q /K %PARALLEL%\scripts\install.bat x86 debug   static
cmd /A /Q /K %PARALLEL%\scripts\install.bat x86 debug   shared
cmd /A /Q /K %PARALLEL%\scripts\install.bat x64 release static
cmd /A /Q /K %PARALLEL%\scripts\install.bat x64 release shared
cmd /A /Q /K %PARALLEL%\scripts\install.bat x64 debug   static
cmd /A /Q /K %PARALLEL%\scripts\install.bat x64 debug   shared

cd %ORIGINAL%
rd /S /Q %COMPILEPATH%
echo Compile Done
rem exit
