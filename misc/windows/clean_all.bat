@echo off

rem RUN ALL CLEANERS

call .\clean_other.bat
call .\clean_temporary.bat
call .\clean_workspace.bat

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo all OK )