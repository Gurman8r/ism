@echo off

call .\clean_build.bat
call .\clean_etc.bat
call .\clean_workspace.bat

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo all OK )