mkdir oggvorbis
mkdir oggvorbis\lib\
mkdir oggvorbis\lib\Debug
mkdir oggvorbis\lib\Debug\Win32
mkdir oggvorbis\lib\Debug\x64
mkdir oggvorbis\lib\Release
mkdir oggvorbis\lib\Release\Win32
mkdir oggvorbis\lib\Release\x64

for %%P in (Win32 x64) do for %%C in (Debug Release) do copy libogg\win32\VS2012\%%P\%%C\libogg_static.lib oggvorbis\lib\%%C\%%P\
for %%P in (Win32 x64) do for %%C in (Debug Release) do copy libogg\win32\VS2012\%%P\%%C\vc110.pdb         oggvorbis\lib\%%C\%%P\

for %%F in (vorbis vorbisfile) do for %%P in (Win32 x64) do for %%C in (Debug Release) do copy libvorbis\win32\VS2012\%%P\%%C\lib%%F_static.lib oggvorbis\lib\%%C\%%P\
for %%F in (vorbis vorbisfile) do for %%P in (Win32 x64) do for %%C in (Debug Release) do copy libvorbis\win32\VS2012\lib%%F\%%P\%%C\vc110.pdb  oggvorbis\lib\%%C\%%P\

pause
