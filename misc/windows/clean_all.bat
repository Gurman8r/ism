@echo off

rem CLEAN ALL

call .\clean_other.bat
call .\clean_temporary.bat
call .\clean_workspace.bat

if exist ..\..\.ism rd /s /q ..\..\.ism

if %ERRORLEVEL% NEQ 0 ( pause ) else ( echo CLEAN ALL OK )