@echo off
cd .\bin\x86\Release\
start sandbox.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )