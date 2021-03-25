@echo off
cd .\bin\x64\Release\
start sandbox.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )