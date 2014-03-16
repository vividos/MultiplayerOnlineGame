REM 7z.exe {command} file.zip {options}
REM -r recurse
REM -x!{wildchard} exclude

"C:\Program Files\7-Zip\7z.exe" a MultiplayerOnlineGame-backup.zip .\* -r -x!.git\* -x!bin\* -x!intermediate\* -x!lib\* -x!downloads\* -x!doc\html\* -x!*.sdf -x!*.opensdf -x!*.suo -x!*.user -x!*.ncb -x!*.ipch

pause