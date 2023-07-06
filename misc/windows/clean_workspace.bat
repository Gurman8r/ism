@echo off

rem CLEANUP PROJECT FILES

if exist ..\..\ism_sdk_vs2017.sln del /q ..\..\ism_sdk_vs2017.sln
if exist ..\..\ism_sdk_vs2019.sln del /q ..\..\ism_sdk_vs2019.sln
if exist ..\..\ism_sdk_vs2022.sln del /q ..\..\ism_sdk_vs2022.sln

if exist ..\..\.vs rd /s /q ..\..\.vs
if exist ..\..\.wks rd /s /q ..\..\.wks

if exist "..\..\Visual Studio 2019" rd /s /q "..\..\Visual Studio 2019"
if exist "..\..\Visual Studio 2019Templates" rd /s /q "..\..\Visual Studio 2019Templates"

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo clean projects OK )