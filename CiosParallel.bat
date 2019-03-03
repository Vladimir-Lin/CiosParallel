@echo off

set NAME=libstardate
set STARDATE=%~dp0
set ORIGINAL=%CD%
set COMPILEPATH=%CD%\StarDate
set /p VERSION=<%STARDATE%\VERSION.txt

mkdir %COMPILEPATH%
cd %COMPILEPATH%

cmd /A /Q /K %STARDATE%\scripts\x86.bat %COMPILEPATH% Release static 1 0
cmd /A /Q /K %STARDATE%\scripts\x86.bat %COMPILEPATH% Release shared 0 1
cmd /A /Q /K %STARDATE%\scripts\x86.bat %COMPILEPATH% Debug   static 1 0
cmd /A /Q /K %STARDATE%\scripts\x86.bat %COMPILEPATH% Debug   shared 0 1
cmd /A /Q /K %STARDATE%\scripts\x64.bat %COMPILEPATH% Release static 1 0
cmd /A /Q /K %STARDATE%\scripts\x64.bat %COMPILEPATH% Release shared 0 1
cmd /A /Q /K %STARDATE%\scripts\x64.bat %COMPILEPATH% Debug   static 1 0
cmd /A /Q /K %STARDATE%\scripts\x64.bat %COMPILEPATH% Debug   shared 0 1

cd %ORIGINAL%
rd /S /Q %COMPILEPATH%
echo Compile Done
rem exit
