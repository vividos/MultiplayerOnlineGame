REM
REM MultiplayerOnlineGame - multiplayer game project
REM Copyright (C) 2008-2019 Michael Fink
REM
REM file RunCppCheck.hpp Runs cppcheck
REM
REM Arguments: call RunCppCheck.cmd "Output-Dir\" [xml]
REM Output file is "Output-Dir\"cppcheck.txt
REM When xml is passed as second parameter, the output file is "cppcheck-Results.xml"
REM and can be used for further processing, e.g. running Jenkins and using a plugin.
REM

REM when started via command line, assume "no xml" and "current folder"
set INTDIR=%1
if "%INTDIR%" == "" set PATH=%PATH%;"C:\Program Files\Cppcheck\"
if "%INTDIR%" == "" set INTDIR=%CD%\

REM when xml is passed as second param, set format and output file
set FORMAT=
set OUTFILE=%INTDIR%cppcheck.txt
if "%2" == "xml" set FORMAT=--xml
if "%2" == "xml" set OUTFILE=%INTDIR%\cppcheck-Results.xml

REM run cppcheck
REM -I <dir>            Include path
REM -i <dir>            Ignore path
REM -D<define>          Define a symbol
REM --suppressions-list=<file>   File with suppressed warnings
REM -j 4                Multithreading
REM --platform=win32W   Platform specific types
REM --language=c++      Language (file extensions)
REM --std=c++11         Language (syntax)
REM --enable=all        Enable warnings
REM --template=vs       Output format for warnings; vs=Visual Studio style
REM --check-config      Checks config instead of checking source files
cppcheck.exe ..\..\ -i ..\..\Thirdparty ^
	-DWIN32 -D_WINDOWS -DNDEBUG -D_UNICODE -DUNICODE -DCPPCHECK -D_MSC_VER=1900 ^
	--suppressions-list=cppcheck-suppress.txt -j 4 ^
	--platform=win32W --language=c++ --std=c++11 %FORMAT% ^
	--enable=all --template=vs 2> %OUTFILE%
