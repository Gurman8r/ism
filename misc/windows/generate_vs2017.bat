@echo off
cd ..\..\
call .\misc\windows\premake5.exe vs2017 --gfxapi=opengl --glapi=glew
if %ERRORLEVEL% NEQ 0 ( pause )
rem pause