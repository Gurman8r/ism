@echo off
set INPUT="toolkit.png"
set OUTPUT="toolkit.ico"
magick.exe convert -background transparent %INPUT% -define icon:auto-resize=16,24,32,48,64,72,96,128,256 %OUTPUT%