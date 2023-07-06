@echo off

rem CLEAN TEMPS

if exist ..\..\.tmp rd /s /q ..\..\.tmp

if exist ..\..\build_windows_x86_Debug rd /s /q ..\..\build_windows_x86_Debug
if exist ..\..\build_windows_x64_Debug rd /s /q ..\..\build_windows_x64_Debug
if exist ..\..\build_windows_x86_Release rd /s /q ..\..\build_windows_x86_Release
if exist ..\..\build_windows_x64_Release rd /s /q ..\..\build_windows_x64_Release

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo clean binaries OK )