REM
REM MultiplayerOnlineGame - multiplayer game project
REM Copyright (C) 2008-2014 Michael Fink
REM
REM Build.cmd - Build script
REM

REM Set up environment
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\VsDevCmd.bat"

REM Build BuildData tool
msbuild data\tools\BuildData\BuildData.csproj /p:Configuration=Release /p:Platform="Any CPU"

REM Build all C++ projects
msbuild src\MultiplayerOnlineGame.sln /m:4 /p:Configuration=Release /target:Rebuild
