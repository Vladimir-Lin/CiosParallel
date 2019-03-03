@echo off

set ARCHX=x86
set COMPILEPATH=%1
set BTYPE=%2
set DLLX=%3
set DOSTATIC=%4
set DOSHARED=%5
set TARGET=%NAME%-%ARCHX%-%BTYPE%-%DLLX%

call %~dp0\..\..\..\settings.bat
call %VC_PATH%\vcvars32.bat

mkdir %TARGET%
mkdir %NAME%-compile-%ARCHX%-%BTYPE%-%DLLX%

cd %COMPILEPATH%\%NAME%-compile-%ARCHX%-%BTYPE%-%DLLX%

cmake %STARDATE% -DCMAKE_INSTALL_PREFIX=%COMPILEPATH%\%TARGET% -DCMAKE_CONFIGURATION_TYPES=%BTYPE% -DCMAKE_BUILD_TYPE=%BTYPE% -DBUILD_SHARED_LIBS=%DOSHARED%

devenv %NAME%.sln /Build
devenv %NAME%.sln /Build %BTYPE% /Project INSTALL

cd ..
7z a %TARGET%.7z %TARGET%
del /Q %STARDATE%\Packages\%TARGET%.7z
move /Y %TARGET%.7z %STARDATE%\Packages
rd /S /Q %TARGET%
rd /S /Q %NAME%-compile-%ARCHX%-%BTYPE%-%DLLX%
exit
