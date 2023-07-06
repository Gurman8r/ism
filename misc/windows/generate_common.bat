@echo off

set dir=.tmp
if not exist %dir% ( mkdir %dir% )
attrib +H %dir%

set dir=.wks
if not exist %dir% ( mkdir %dir% )
attrib +H %dir%

rem pause