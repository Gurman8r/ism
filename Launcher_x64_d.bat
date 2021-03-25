@echo off
cd .\bin\x64\Debug\
start sandbox.exe %*
if %ERRORLEVEL% NEQ 0 ( pause )