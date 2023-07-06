@echo off

rem CLEAN WORKSPACE

if exist ..\..\.ism\workspace rd /s /q ..\..\.ism\workspace
if exist ..\..\.vs rd /s /q ..\..\.vs
if exist ..\..\ism_vs2017.sln del /q ..\..\ism_vs2017.sln
if exist ..\..\ism_vs2019.sln del /q ..\..\ism_vs2019.sln
if exist ..\..\ism_vs2022.sln del /q ..\..\ism_vs2022.sln
if exist "..\..\Visual Studio 2019" rd /s /q "..\..\Visual Studio 2019"
if exist "..\..\Visual Studio 2019Templates" rd /s /q "..\..\Visual Studio 2019Templates"

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo CLEAN WORKSPACE OK )