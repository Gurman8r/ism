@echo off
cd ..\..\
call .\misc\windows\premake5.exe vs2022 --gfxapi=opengl --gl_loader=glew
if %ERRORLEVEL% NEQ 0 ( pause )
rem pause