@echo off

set ARCHX=%1
set BTYPE=%2
set DLLX=%3
set TARGET=%NAME%-%ARCHX%-%BTYPE%-%DLLX%
set ARCHPATH=%ARCHX%\%DLLX%\%BTYPE%
set LIBPLACE=%CIOS%\lib\%ARCHPATH%
set PKGPLACE=%CIOS%\Packages\%ARCHPATH%

7z x %STARDATE%\Packages\%TARGET%.7z

cd %COMPILEPATH%\%TARGET%\lib\packages

fart %NAME%.pc "$${INCLUDEPATH}" "%CIOSPATH%/include"
fart %NAME%.pc "$${LIBPATH}" "%CIOSPATH%/lib/%ARCHX%/%DLLX%/%BTYPE%"

cd %COMPILEPATH%\%TARGET%

xcopy /Y %COMPILEPATH%\%TARGET%\lib\%NAME%.lib %LIBPLACE%\%NAME%.lib
xcopy /Y %COMPILEPATH%\%TARGET%\lib\packages\%NAME%.pc %PKGPLACE%\%NAME%.pc
xcopy /Y %COMPILEPATH%\%TARGET%\bin\%NAME%.dll %LIBPLACE%\DLL\%NAME%.dll

cd %COMPILEPATH%

rd /S /Q %COMPILEPATH%\%TARGET%
exit
