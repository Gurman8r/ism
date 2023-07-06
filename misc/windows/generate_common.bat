@echo off

if not exist .ism ( mkdir .ism )
if not exist .ism\temporary ( mkdir .ism\temporary )
if not exist .ism\workspace ( mkdir .ism\workspace )
attrib +H .ism

rem pause