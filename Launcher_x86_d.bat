@echo off
cd .\bin\x86\Debug\
start sandbox.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )