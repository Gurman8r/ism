@echo off

if exist ..\..\bin rd /s /q ..\..\bin
if exist ..\..\bin-lib rd /s /q ..\..\bin-lib
if exist ..\..\bin-obj rd /s /q ..\..\bin-obj
if exist ..\..\build rd /s /q ..\..\build
if exist ..\..\temps rd /s /q ..\..\temps

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo clean binaries OK )