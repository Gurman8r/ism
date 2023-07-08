@echo off
cd ..\..\
call .\misc\windows\make_common.bat
call .\misc\windows\premake5.exe vs2019
if %ERRORLEVEL% NEQ 0 ( pause )
rem pause