@echo off

rem RUN ALL CLEANERS

call .\clean_build.bat
call .\clean_workspace.bat
call .\clean_other.bat

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo all OK )