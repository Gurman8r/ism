@echo off

call .\clean_binaries.bat
call .\clean_projects.bat
call .\clean_temps.bat

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo all OK )